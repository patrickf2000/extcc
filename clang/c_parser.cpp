#include <iostream>

#include "c_parser.hh"
#include "c_lex.hh"

//The main parse function
void CParser::parse() {
	int type = 0;
	
	//Call the scanner and loop until we go through the file
	while (type != -1) {
		Token t = scan->getNext();
		type = t.type;
		
		//Act upon what kind of type we have
		switch (type) {
			case CTokenType::Extern: buildExtern(); break;
			
			//Conditional tokens
			case CTokenType::If: buildCond(CondType::If); break;
			case CTokenType::Else: buildElse(); break;
			case CTokenType::While: buildCond(CondType::While); break;
			case CTokenType::For: buildFor(); break;
			
			//Data type tokens- can be either function declarations or variables
			case CTokenType::Void:
			case CTokenType::Int: {
				Token idToken = scan->getNext();
				Token symToken = scan->getNext();
				
				//Function declaration
				if (symToken.type == CTokenType::LeftParen) {
					auto *fd = new AstFuncDec(idToken.id);
					fd->rtype = token2type(type);
					buildFuncDec(fd);
					
				//Variable declaration
				} else if (symToken.type == CTokenType::Assign) {
					auto *vd = new AstVarDec(idToken.id);
					vd->set_type(token2type(type));
					buildVarAssign(vd);
					
				//Array declaration
				} else if (symToken.type == CTokenType::LBracket) {
					auto *arr = new AstArrayDec;
					arr->set_name(idToken.id);
					arr->set_type(token2type(type));
					buildArrayDec(arr);
					
					Var v;
					v.type = token2type(type);
					v.name = idToken.id;
					v.is_array = true;
					vars[idToken.id] = v;
					
				//Syntax error
				} else {
					//TODO: Rest in peace
				}
			} break;
			
			//ID tokens
			case CTokenType::Id: {
				Token next = scan->getNext();
				
				//Function call
				if (next.type == CTokenType::LeftParen) {
					auto *fc = new AstFuncCall(t.id);
					buildFuncCall(fc);
				//Variable assignment
				} else if (next.type == CTokenType::Assign) {
					auto *va = new AstVarAssign(t.id);
					buildVarAssign(va);
				} else {
					//TODO: Syntax error
				}
			} break;
			
			//Return
			case CTokenType::Return: buildReturn(); break;
			
			//If we have a right-facing curly brace, adjust the top node
			case CTokenType::RightCBrace: {
				if (add_ret && topNodes.size() == 2) {
					auto ret = new AstReturn;
					topNodes.top()->children.push_back(ret);
					add_ret = false;
				}
				
				//If we are at the end of a function, do a few things
				if (topNodes.size() == 2) {
					vars.clear();
					auto last = topNodes.top()->children[topNodes.top()->children.size()-1]->type;
					
					if (last != AstType::Return) {
						auto *fd = static_cast<AstFuncDec *>(topNodes.top());
						if (fd->rtype != DataType::Void) {
							std::string error = "Fatal: Missing return value at function: ";
							error += fd->get_name();
							
							syntax->addError(error);
							return;
						}
					}
				}
				
				auto last_type = topNodes.top()->type;
				
				//Add nodes that need to be at the end of the block
				if (last_type == AstType::For) {
					auto node = blockEnds.top();
					blockEnds.pop();
					topNodes.top()->children.push_back(node);
				}
				
				topNodes.pop();
				
				//Determine if an endif is necessary
				auto endif = new AstNode(AstType::EndIf);
				
				switch (last_type) {
					case AstType::If: {
						topNodes.top()->children.push_back(endif);
					} break;
					case AstType::Elif:
					case AstType::Else: {
						topNodes.top()->children.push_back(endif);
					} break;
				}
			} break;
		}
	}
}

//Converts a token type to a datatype
DataType CParser::token2type(int token) {
	switch (token) {
		case CTokenType::Void: return DataType::Void;
		case CTokenType::Char: return DataType::Char;
		case CTokenType::Short: return DataType::Short;
		case CTokenType::Int: return DataType::Int;
		case CTokenType::Float: return DataType::Float;
		case CTokenType::Double: return DataType::Double;
	}

	return DataType::None;
}

//Build an external function
void CParser::buildExtern() {
	auto *fd = new AstExternFunc;
	top->children.push_back(fd);
	
	//Get the tokens we need
	Token fType = scan->getNext();		//The function type
	Token fName = scan->getNext();		//The function name
	
	//Set the information
	fd->set_name(fName.id);
}

//Builds a function declaration
void CParser::buildFuncDec(AstFuncDec *fd) {
	top->children.push_back(fd);
	
	//Parse the arguments
	Token next = scan->getNext();
	int argc = 0;
	Var v;
	v.is_param = true;
	
	while (next.type != CTokenType::RightParen) {
		switch (next.type) {
			case CTokenType::Void:
			case CTokenType::Char:
			case CTokenType::Short:
			case CTokenType::Int:
			case CTokenType::Float:
			case CTokenType::Double: v.type = token2type(next.type); break;
			
			case CTokenType::Id: v.name = next.id; break;
			
			case CTokenType::Comma: fd->args.push_back(v); break;
			
			//TODO: Add memory reference/pointers
		}
		
		next = scan->getNext();
		++argc;
	}
	
	if (argc > 1)
		fd->args.push_back(v);
	
	//The next token should be a curly brace
	next = scan->getNext();
	
	if (next.type != CTokenType::LeftCBrace) {
		syntax->addError("Syntax error: Expected \'{\'");
	}
	
	//The new current top is the function declaration
	topNodes.push(fd);
	
	//If its a void function, we need to make sure a return
	// statement is added at the end
	if (fd->rtype == DataType::Void)
		add_ret = true;
}

//Builds a function call
void CParser::buildFuncCall(AstFuncCall *fc, bool add_top) {
	addChildren(fc, CTokenType::RightParen);
	
	if (add_top)
		topNodes.top()->children.push_back(fc);
}

//Builds a return statement
void CParser::buildReturn() {
	auto ret = new AstReturn;
	topNodes.top()->children.push_back(ret);
	addChildren(ret);
}

//Builds a variable declaration
void CParser::buildVarAssign(AstVarDec *vd, int stop, bool add_end) {
	std::vector<AstNode *> nodes;
	
	if (add_end)
		blockEnds.push(vd);
	else
		topNodes.top()->children.push_back(vd);
	
	//Get all tokens until the semi-colon
	Token next = scan->getNext();
	
	while (next.type != stop) {
		nodes.push_back(buildNode(next));
		next = scan->getNext();
	}
	
	//See if we have a math expression
	if (nodes.size() == 1) {
		vd->children.push_back(nodes.at(0));
	} else if (nodes.size() == 0) {
		syntax->addError("Invalid syntax when assigning a variable.");
	} else {
		auto math = new AstMath;
		vd->children.push_back(math);
		
		for (auto child : nodes) {
			math->children.push_back(child);
		}
	}
}

//Builds an array declaration
void CParser::buildArrayDec(AstArrayDec *arr) {
	topNodes.top()->children.push_back(arr);
	
	//Set the size
	Token next = scan->getNext();
	
	if (next.type != CTokenType::No) {
		syntax->addError("Invalid array size.");
		return;
	}
		
	int size = std::stoi(next.id);
	arr->set_size(size);
	
	//Syntax check
	next = scan->getNext();
	
	if (next.type != CTokenType::RBracket) {
		syntax->addError("Invalid array syntax.");
		return;
	}
	
	//Build the rest of the declaration
	next = scan->getNext();
	
	if (next.type == CTokenType::SemiColon) {
		for (int i = 0; i<size; i++) {
			auto i = new AstInt(0);
			arr->children.push_back(i);
		}
		
		return;
	}
	
	//Do some more syntax check
	if (next.type != CTokenType::Assign)
		syntax->addError("Invalid array syntax.");
		
	next = scan->getNext();
	
	if (next.type != CTokenType::LeftCBrace)
		syntax->addError("Invalid array syntax.");
		
	//Load the elements
	next = scan->getNext();
	
	while (next.type != CTokenType::RightCBrace) {
		if (next.type != CTokenType::Comma)
			arr->children.push_back(buildNode(next));
		next = scan->getNext();
	}
	
	//Final syntax check
	next = scan->getNext();
	
	if (next.type != CTokenType::SemiColon)
		syntax->addError("Expected terminating token.");
}

//Builds an If statement
void CParser::buildCond(CondType type) {
	Token next = scan->getNext();
	
	if (next.type != CTokenType::LeftParen && type != CondType::For) {
		syntax->addError("Expected \'(\' in conditional statement.");
	}
	
	//Start constructing the AST node
	AstCond *cond = new AstIf;
	if (type == CondType::Elif) cond = new AstElif;
	else if (type == CondType::While) cond = new AstWhile;
	else if (type == CondType::For) cond = new AstFor;
	
	//Scan and build
	Token lval;
	if (type == CondType::For)
		lval = next;
	else
		lval = scan->getNext();
	
	cond->lval = buildNode(lval);
	
	Token op = scan->getNext();
	Token rval = scan->getNext();
	
	if (op.type == CTokenType::Assign) {
		if (rval.type == CTokenType::Assign) {
			cond->set_op(CondOp::Equals);
			rval = scan->getNext();
		} else {
			syntax->addError("Assignment invalid in a conditional.");
		}
	} else if (op.type == CTokenType::Greater) {
		if (rval.type == CTokenType::Assign) {
			cond->set_op(CondOp::GreaterEq);
			rval = scan->getNext();
		} else {
			cond->set_op(CondOp::Greater);
		}
	} else if (op.type == CTokenType::Less) {
		if (rval.type == CTokenType::Assign) {
			cond->set_op(CondOp::LessEq);
			rval = scan->getNext();
		} else {
			cond->set_op(CondOp::Less);
		}
		
	//TODO: Add not equals
	} else {
		syntax->addError("Unknown conditional operator");
	}
	
	cond->rval = buildNode(rval);

	//Add to the tree and update the stack
	if (type == CondType::Elif)
		topNodes.top()->children.pop_back();
	
	topNodes.top()->children.push_back(cond);
	topNodes.push(cond);
	
	//TODO: Final syntax check
}

//Builds an else statement
void CParser::buildElse() {
	Token next = scan->getNext();
	
	if (next.type == CTokenType::If) {
		buildCond(CondType::Elif);
		return;
	} else if (next.type != CTokenType::LeftCBrace) {
		syntax->addError("Expected curly brace after else");
		return;
	}
	
	topNodes.top()->children.pop_back();
	
	auto e = new AstElse;
	topNodes.top()->children.push_back(e);
	topNodes.push(e);
}

//Builds a for loop
void CParser::buildFor() {
	Token next = scan->getNext();
	
	if (next.type != CTokenType::LeftParen)
		syntax->addError("Expected token");
		
	//Build the first part
	next = scan->getNext();
	
	switch (next.type) {
		//We are resetting another variable
		case CTokenType::Id: {
			Token id = scan->getNext();
			next = scan->getNext();
			
			if (next.type == CTokenType::Assign) {
				auto *va = new AstVarAssign(id.id);
				buildVarAssign(va);
			} else {
				syntax->addError("Invalid for-loop syntax.");
			}
		} break;
		
		//We are declaring a new variable
		case CTokenType::Char:
		case CTokenType::Short:
		case CTokenType::Int:
		case CTokenType::Float:
		case CTokenType::Double: {
			int type = next.type;
			Token id = scan->getNext();
			next = scan->getNext();
			
			if (next.type == CTokenType::Assign) {
				auto *vd = new AstVarDec(id.id);
				vd->set_type(token2type(type));
				buildVarAssign(vd);
			} else {
				syntax->addError("Invalid for-loop syntax.");
			}
		} break;
		
		//We have a syntax error
		default: syntax->addError("Invalid for-loop syntax.");
	}
	
	//Build the second part
	buildCond(CondType::For);
	
	next = scan->getNext();
	if (next.type != CTokenType::SemiColon)
		syntax->addError("Invalid for-loop syntax.");
	
	//Build the final part	
	Token id = scan->getNext();
	next = scan->getNext();
	
	if (next.type == CTokenType::Assign) {
		auto *va = new AstVarAssign(id.id);
		buildVarAssign(va, CTokenType::RightParen, true);
	} else {
		syntax->addError("Expected variable expression.");
	}
}

//Scans the source until we have a semicolon, and creates
// nodes in the process
void CParser::addChildren(AstNode *parent, int stop) {
	Token next = scan->getNext();
	
	while (next.type != stop) {
		switch (next.type) {			
			//TODO: Fix this
			case CTokenType::Comma: break;
			
			default: {
				auto node = buildNode(next);
				parent->children.push_back(node);
			}
		}
		
		next = scan->getNext();
	}
}

//Builds a single node based on a token
AstNode *CParser::buildNode(Token t) {
	switch (t.type) {
		//Integers
		case CTokenType::No: {
			int i = std::stoi(t.id);
			auto *ai = new AstInt(i);
			return ai;
		} break;
		
		//Floats
		case CTokenType::FloatL: {
			float f = std::stod(t.id);
			auto *af = new AstFloat(f);
			return af;
		} break;
		
		//Strings
		case CTokenType::String: {
			auto *str = new AstString;
			str->set_val(t.id);
			return str;
		} break;
		
		//Other variables
		case CTokenType::Id: {
			Token next = scan->getNext();
			
			//Array access
			if (next.type == CTokenType::LBracket) {
				Var v = vars[t.id];
				auto *acc = new AstArrayAcc(t.id);
				acc->set_type(v.type);
				
				next = scan->getNext();
				acc->children.push_back(buildNode(next));
				
				next = scan->getNext();
				
				if (next.type != CTokenType::RBracket)
					syntax->addError("Invalid array access syntax.");
				
				return acc;
				
			//Function call
			} else if (next.type == CTokenType::LeftParen) {
				auto *fd = new AstFuncCall(t.id);
				buildFuncCall(fd, false);
				return fd;
			}
			
			//Variable
			scan->unget(next);
			auto *id = new AstID(t.id);
			return id;
		} break;
		
		//Memory reference
		case CTokenType::Amp: {
			Token next = scan->getNext();
			
			auto *id = new AstID(next.id);
			id->is_ref = true;
			return id;
		} break;
		
		//Operators
		case CTokenType::Plus: return new AstNode(AstType::Add);
		case CTokenType::Minus: return new AstNode(AstType::Sub);
		case CTokenType::Mul: return new AstNode(AstType::Mul);
		case CTokenType::Div: return new AstNode(AstType::Div);
		case CTokenType::Mod: return new AstNode(AstType::Mod);
		
		//TODO: Add the rest
	}
	
	return new AstNode;
}



