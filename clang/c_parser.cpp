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
			
			//Data type tokens- can be either function declarations or variables
			case CTokenType::Void:
			case CTokenType::Int: {
				Token idToken = scan->getNext();
				Token symToken = scan->getNext();
				
				//Function declaration
				if (symToken.type == CTokenType::LeftParen) {
					auto *fd = new AstFuncDec(idToken.id);
					buildFuncDec(fd);
				//Variable declaration
				} else if (symToken.type == CTokenType::Assign) {
					auto *vd = new AstVarDec(idToken.id);
					vd->set_type(token2type(type));
					buildVarDec(vd);
				} else {
					//TODO: Syntax error
				}
			} break;
			
			//If we have a right-facing curly brace, adjust the top node
			case CTokenType::RightCBrace: topNodes.pop(); break;
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
		//TODO: Syntax error
		std::cout << "Syntax error: Expected \')\'" << std::endl;
	}
	
	//The next token should be a curly brace
	next = scan->getNext();
	
	if (next.type != CTokenType::LeftCBrace) {
		//TODO: Syntax error
		std::cout << "Syntax error: Expected \'{\'" << std::endl;
	}
	
	//The new current top is the function declaration
	topNodes.push(fd);
}

//Builds a variable declaration
void CParser::buildVarDec(AstVarDec *vd) {
	topNodes.top()->children.push_back(vd);
	Token next = scan->getNext();
	
	while (next.type != CTokenType::SemiColon) {
		switch (next.type) {
			//Integers
			case CTokenType::No: {
				int i = std::stoi(next.id);
				auto *ai = new AstInt(i);
				vd->children.push_back(ai);
			} break;
			
			//Other variables
			case CTokenType::Id: {
				auto *id = new AstID(next.id);
				vd->children.push_back(id);
			} break;
			
			//TODO: Add the rest
		}
		
		next = scan->getNext();
	}
}


