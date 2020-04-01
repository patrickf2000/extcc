#include "c_parser.hh"
#include "c_lex.hh"

//The main parse function
void CParser::parse() {
	currentTop = top;
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
				} else if (symToken.type == CTokenType::Assign) {
					//TODO: Variable declaration
				} else {
					//TODO: Syntax error
				}
			} break;
		}
	}
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
}

