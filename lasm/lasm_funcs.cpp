#include "lasm_parser.hh"

using namespace LtacAsm;

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

//Builds an extern statement
void AsmParser::buildExtern() {
	checkCode();
	auto *ext = new LtacExtern;
	file->code->children.push_back(ext);
	
	//Build the return type
	Token next = scan->getNext();
	ext->ret_type = getDataType(next);
	
	//Build the name
	next = scan->getNext();
	if (next.type != AsmTokenType::Name)
		syntax->fatalError("Expected name.");
		
	ext->name = next.id;
	
	//Build the parameters
	next = scan->getNext();
	
	while (next.type != AsmTokenType::NewLn) {
		ext->params.push_back(getDataType(next));
		next = scan->getNext();
	}
}

//Builds a load-arg statement
void AsmParser::buildLdArg() {
	checkCode();
	auto *arg = new LtacVar;

	Token name = scan->getNext();
	Token type = scan->getNext();

	if (name.type != AsmTokenType::Name)
		syntax->fatalError("Expected variable name in ldarg.");

	//Determine the type and increment the stack
	switch (type.type) {
		case AsmTokenType::Int: {
			stack_pos += 4;
			arg->d_type = DataType::Int;
		} break;

		case AsmTokenType::Float: {
			stack_pos += 4;
			arg->d_type = DataType::Float;
		} break;

		case AsmTokenType::Double: {
			stack_pos += 8;
			arg->d_type = DataType::Double;
		} break;
	}

	vars[name.id] = stack_pos;
	types[name.id] = arg->d_type;
	arg->pos = stack_pos;
	arg->is_ptr = false;
	arg->is_ref = false;

	topNode.top()->children.push_back(arg);
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
	
	scan->unget(next);
	addChildren(ret);
}

