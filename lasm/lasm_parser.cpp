#include "lasm_parser.hh"

using namespace LtacAsm;

//The main assembly parse file
void AsmParser::parse() {
	int type = 0;
	
	while (type != -1) {
		Token next = scan->getNext();
		type = next.type;
		bool nl = false;
		
		switch (type) {
			case AsmTokenType::Section: buildSection(); break;
			case AsmTokenType::Func: buildFunc(); break;
			case AsmTokenType::Ret: buildRet(); break;
			case AsmTokenType::String: buildString(); break;
			case AsmTokenType::NewLn: break;
		}
	}
}

//Make sure the code section was declared
void AsmParser::checkCode() {
	if (file->code == nullptr)
		syntax->fatalError("Not in code section.");
}

//Make sure the data section was declared
void AsmParser::checkData() {
	if (file->data == nullptr)
		syntax->fatalError("Not in data section.");
}

//Builds a section
void AsmParser::buildSection() {
	Token next = scan->getNext();
	
	if (next.type == AsmTokenType::Data)
		file->data = new LtacDataSec;
	else if (next.type == AsmTokenType::Code)
		file->code = new LtacCodeSec;
	else
		syntax->fatalError("Invalid section name.");
}

//Builds a function
void AsmParser::buildFunc() {
	Token next = scan->getNext();
	
	if (next.type != AsmTokenType::Name)
		syntax->fatalError("Expected function name.");
		
	auto *func = new LtacFunc(next.id);
	
	checkCode();
	file->code->children.push_back(func);
}

//Builds a return statement
void AsmParser::buildRet() {
	checkCode();
	
	auto *ret = new LtacRet;
	file->code->children.push_back(ret);
	
	//If the next token is a new line, we are returning void
	Token next = scan->getNext();
	
	if (next.type == AsmTokenType::NewLn)
		return;
}

//Builds a string declaration
void AsmParser::buildString() {
	checkData();

	Token next = scan->getNext();
	Token str = scan->getNext();
	
	if (next.type != AsmTokenType::Name)
		syntax->fatalError("Expected variable name.");
	else if (str.type != AsmTokenType::StringL)
		syntax->fatalError("Expected string literal.");
		
	auto *lstr = new LtacString(next.id, str.id);
	file->data->children.push_back(lstr);
}



