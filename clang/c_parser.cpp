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
			case CTokenType::Char:
			case CTokenType::Int:
			case CTokenType::Float: 
			case CTokenType::Double: {
				Token idToken = scan->getNext();
				Token symToken = scan->getNext();
				bool ptr = false;
				
				//See if we have a pointer
				if (idToken.type == CTokenType::Mul) {
					ptr = true;
					idToken = symToken;
					symToken = scan->getNext();
				}
				
				//Function declaration
				if (symToken.type == CTokenType::LeftParen) {
					auto *fd = new AstFuncDec(idToken.id);
					fd->rtype = token2type(type);
					current_rval = fd->rtype;
					buildFuncDec(fd);
					
				//Variable declaration
				} else if (symToken.type == CTokenType::Assign) {
					buildVarDec(type, idToken, ptr);
					
				//Uninitalized variable declaration
				} else if (symToken.type == CTokenType::SemiColon) {
					buildVarDec(type, idToken, ptr, true);
					
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
					v.scope_level = scope_level;
					vars[idToken.id] = v;
					
				//Syntax error
				} else {
					syntax->fatalError("Invalid variable, array, or function declaration.");
				}
			} break;
			
			//ID tokens
			case CTokenType::Id: {
				Token next = scan->getNext();
				
				//Function call
				if (next.type == CTokenType::LeftParen) {
					auto *fc = new AstFuncCall(t.id);
					buildFuncCall(fc);
					
				//Structure modification
				} else if (next.type == CTokenType::Dot) {
					buildStructMod(t.id);
					
				//Variable assignment
				} else if (next.type == CTokenType::Assign) {
					auto *va = new AstVarAssign(t.id);
					Var v = vars[va->get_name()];
					va->set_type(v.type);
					buildVarAssign(va);
					
				//Variable increment
				} else if (next.type == CTokenType::Plus) {
					next = scan->getNext();
					
					if (next.type != CTokenType::Plus)
						syntax->fatalError("Expected increment operator.");
						
					buildVarIncrement(t.id);
					
				//Array assignment
				} else if (next.type == CTokenType::LBracket) {
					auto *arr = new AstArrayAssign;
					
					next = scan->getNext();
					arr->index = buildNode(next);
					
					next = scan->getNext();
					next = scan->getNext();
				
					Var v = vars[t.id];
					arr->set_name(t.id);
					arr->set_type(v.type);
					buildVarAssign(arr);
					
				//Ignore the preprocessor
				// TODO: REMOVE THIS!!!
				} else if (t.id == "#include") {
					if (next.type != CTokenType::String) {
						scan->getNext();
						scan->getNext();
						scan->getNext();
					}
					
				// Syntax error
				} else {
					syntax->fatalError("Invalid redefinition syntax");
				}
			} break;
			
			//Return
			case CTokenType::Return: buildReturn(); break;
			
			//Structures
			case CTokenType::Struct: {
				Token idToken = scan->getNext();
				Token symToken = scan->getNext();
				
				//Structure declaration
				if (symToken.type == CTokenType::LeftCBrace) {
					buildStruct(idToken.id);
					
				//Structure variable
				} else if (symToken.type == CTokenType::Id) {
					Token next = scan->getNext();
					
					//Function declaration that returns a struct
					if (next.type == CTokenType::LeftParen) {
						auto *fd = new AstFuncDec(symToken.id);
						fd->rtype = DataType::Pointer;
						current_rval = DataType::Pointer;
						buildFuncDec(fd);
					
					//Structure variable
					} else {
						scan->unget(next);
						buildStructVar(idToken.id, symToken.id);
					}
					
				//Syntax error
				} else {
					syntax->fatalError("Invalid struct syntax.");
				}
			} break;
			
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
				--scope_level;
				
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
				
				//Update the scope
				for (auto const& var : vars) {
					if (var.second.scope_level >= scope_level) {
						vars.erase(var.first);
					}
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
		case CTokenType::Struct: return DataType::Pointer;
	}

	return DataType::None;
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
AstNode *CParser::buildNode(Token t, bool float2dbl, bool inMath) {
	switch (t.type) {
		//Chars
		case CTokenType::CharL: {
			auto *ci = new AstChar(t.id[0]);
			return ci;
		} break;
	
		//Integers
		case CTokenType::No: {
			int i = std::stoi(t.id);
			auto *ai = new AstInt(i);
			return ai;
		} break;
		
		//Floats
		case CTokenType::FloatL: {
			if (float2dbl) {
				double f = std::stod(t.id);
				auto *ad = new AstDouble(f);
				return ad;
			} else {
				float f = std::stod(t.id);
				auto *af = new AstFloat(f);
				return af;
			}
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
				
			//Struct access
			} else if (next.type == CTokenType::Dot) {
				auto *acc = new AstStructAcc;
				acc->str_name = t.id;
				
				next = scan->getNext();
				
				if (next.type != CTokenType::Id)
					syntax->fatalError("Invalid structure access: Identifier required.");
					
				acc->var_name = next.id;
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
		
		//Multiplication or pointer
		case CTokenType::Mul: {
			if (!inMath) {
				Token next = scan->getNext();
				
				if (next.type == CTokenType::Id) {
					Var v = vars[next.id];
					
					if (v.is_ptr) {
						auto *id = new AstID(next.id);
						id->is_ptr = true;
						return id;
					}
				}
				
				scan->unget(next);
			}
			
			return new AstNode(AstType::Mul);
		} break;

		//Operators
		case CTokenType::Plus: {
			Token next = scan->getNext();
			
			if (next.type == CTokenType::Plus) {
				return new AstNode(AstType::Inc);
			} else {
				scan->unget(next);
				return new AstNode(AstType::Add);
			}
		}
		
		case CTokenType::Minus: return new AstNode(AstType::Sub);
		case CTokenType::Div: return new AstNode(AstType::Div);
		case CTokenType::Mod: return new AstNode(AstType::Mod);
		
		//TODO: Add the rest
	}
	
	return new AstNode;
}



