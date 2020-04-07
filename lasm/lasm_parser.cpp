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
			case AsmTokenType::Call: buildFuncCall(); break;
			case AsmTokenType::Ret: buildRet(); break;
			case AsmTokenType::PushArg: buildPushArg(); break;
			case AsmTokenType::String: buildString(); break;
			case AsmTokenType::Var: buildVar(); break;
			case AsmTokenType::NewLn: break;
		}
	}
	
	calcStackSize();
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
	checkCode();
	Token next = scan->getNext();
	
	if (next.type != AsmTokenType::Name)
		syntax->fatalError("Expected function name.");
		
	auto *func = new LtacFunc(next.id);

	if (topNode.size() > 0)
		calcStackSize();
	
	file->code->children.push_back(func);
	topNode.push(func);
}

//Calculates the stack position of the last function
void AsmParser::calcStackSize() {
	int stack_size = 0;
	if (stack_pos > 0) {
		while (stack_size < (stack_pos + 1))
			stack_size += 16;
	}
	
	auto func = static_cast<LtacFunc *>(topNode.top());
	topNode.pop();
	
	func->stack_size = stack_size;
	stack_pos = 0;
}

//Builds a push-arg statement
void AsmParser::buildPushArg() {
	auto *push = new LtacPushArg;
	file->code->children.push_back(push);
	
	addChildren(push);
}

//Builds a function call
void AsmParser::buildFuncCall() {
	Token next = scan->getNext();
	
	if (next.type != AsmTokenType::Name)
		syntax->fatalError("Expected function name.");
		
	auto *func = new LtacFuncCall(next.id);
	
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

//Builds a variable declaration
void AsmParser::buildVar() {
	auto *var = new LtacVar;
	file->code->children.push_back(var);
	
	Token name = scan->getNext();
	
	if (name.type != AsmTokenType::Name)
		syntax->fatalError("Expected variable name.");
	
	addChildren(var, true);
	
	vars[name.id] = stack_pos;
	var->pos = stack_pos;
}

//Adds children to a parent node
void AsmParser::addChildren(LtacNode *parent, bool inc_stack) {
	checkCode();
	
	Token type = scan->getNext();
	Token name = scan->getNext();
	
	//Construct the argument
	switch (type.type) {
		//Push an integer
		case AsmTokenType::Int: {
			int val = std::stoi(name.id);
			auto *i = new LtacInt(val);
			parent->children.push_back(i);
			
			if (inc_stack) stack_pos += 4;
		} break;
	
		//Push a string
		case AsmTokenType::String: {
			auto *str = new LtacString;
			str->name = name.id;
			parent->children.push_back(str);
			
			if (inc_stack) stack_pos += 8;
		} break;
		
		default: syntax->fatalError("Unknown type.");
	}
}


