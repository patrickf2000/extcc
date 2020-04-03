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
			case CTokenType::If: buildIf(false); break;
			case CTokenType::Else: buildElse(); break;
			
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
				} else {
					//TODO: Syntax error
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
				
				auto last_type = topNodes.top()->type;
				topNodes.pop();
				
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
	
	//Make sure the next token is a parentheses
	//TODO: Add support for arguments
	Token next = scan->getNext();
	
	if (next.type != CTokenType::RightParen) {
		syntax->addError("Syntax error: Expected \')\'");
	}
	
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
void CParser::buildFuncCall(AstFuncCall *fc) {
	topNodes.top()->children.push_back(fc);
	addChildren(fc, CTokenType::RightParen);
}

//Builds a return statement
void CParser::buildReturn() {
	auto ret = new AstReturn;
	topNodes.top()->children.push_back(ret);
	addChildren(ret);
}

//Builds a variable declaration
void CParser::buildVarAssign(AstVarDec *vd) {
	topNodes.top()->children.push_back(vd);
	std::vector<AstNode *> nodes;
	
	//Get all tokens until the semi-colon
	Token next = scan->getNext();
	
	while (next.type != CTokenType::SemiColon) {
		nodes.push_back(buildNode(next));
		next = scan->getNext();
	}
	
	//See if we have a math expression
	if (nodes.size() == 1) {
		vd->children.push_back(nodes.at(0));
	} if (nodes.size() == 0) {
		syntax->addError("Invalid syntax when assigning a variable.");
	} else {
		auto math = new AstMath;
		vd->children.push_back(math);
		
		for (auto child : nodes) {
			math->children.push_back(child);
		}
	}
}

//Builds an If statement
void CParser::buildIf(bool elif) {
	Token next = scan->getNext();
	
	if (next.type != CTokenType::LeftParen) {
		syntax->addError("Expected \'(\' in conditional statement.");
	}
	
	//Start constructing the AST node
	AstCond *cond = new AstIf;
	if (elif) cond = new AstElif;
	
	//Scan and build
	Token lval = scan->getNext();
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
	if (elif)
		topNodes.top()->children.pop_back();
	
	topNodes.top()->children.push_back(cond);
	topNodes.push(cond);
	
	//TODO: Final syntax check
}

//Builds an else statement
void CParser::buildElse() {
	Token next = scan->getNext();
	
	if (next.type == CTokenType::If) {
		buildIf(true);
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
		
		//Strings
		case CTokenType::String: {
			auto *str = new AstString;
			str->set_val(t.id);
			return str;
		} break;
		
		//Other variables
		case CTokenType::Id: {
			auto *id = new AstID(t.id);
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



