#include "lasm_parser.hh"

using namespace LtacAsm;

//The main assembly parse file
void AsmParser::parse() {
	int type = 0;
	
	while (type != -1) {
		Token next = scan->getNext();
		type = next.type;
		
		switch (type) {
			case AsmTokenType::Section: buildSection(); break;
			case AsmTokenType::Func: buildFunc(); break;
			case AsmTokenType::Call: buildFuncCall(); break;
			case AsmTokenType::Ret: buildRet(); break;
			case AsmTokenType::PushArg: buildPushArg(); break;
			case AsmTokenType::String: buildString(); break;
			case AsmTokenType::Var: buildVar(); break;
			case AsmTokenType::Ldr: buildLdr(); break;
			
			case AsmTokenType::IAdd:
			case AsmTokenType::ISub:
			case AsmTokenType::IMul:
			case AsmTokenType::IDiv: buildMath(1, type); break;
			
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

//Builds the load register command
void AsmParser::buildLdr() {
	auto *reg = new LtacReg;
	file->code->children.push_back(reg);
	
	Token no = scan->getNext();
	Token val = scan->getNext();
	
	if (no.type != AsmTokenType::IntL)
		syntax->fatalError("Expected register number.");
	
	int v = std::stoi(no.id);
	
	if (v <= 0)
		syntax->fatalError("Invalid register; A register must be a number greater than 0.");
		
	reg->pos = v;
	
	//Build the value being loaded
	switch (val.type) {
		//Other variables
		case AsmTokenType::Name: {
			auto *lv = new LtacVar;
			lv->pos = vars[val.id];
			reg->children.push_back(lv);
		} break;
		
		//TODO: Add rest
	}
}

//Builds a single math command
// 1-> Integer
// 2-> Single precision float
// 3-> Double precision float
void AsmParser::buildMath(int type, int op) {
	checkCode();
	LtacOp *math = new LtacOp;

	if (type == 1)
		math = new LtacIMath;
	
	switch (op) {
		case AsmTokenType::IAdd: math->op = Operator::Add; break;
		case AsmTokenType::ISub: math->op = Operator::Sub; break;
		case AsmTokenType::IMul: math->op = Operator::Mul; break;
		case AsmTokenType::IDiv: math->op = Operator::Div; break;
	}
	
	addChildren(math, false);
	addChildren(math, false);
	
	file->code->children.push_back(math);
}

//Adds children to a parent node
void AsmParser::addChildren(LtacNode *parent, bool inc_stack) {
	checkCode();
	
	Token type = scan->getNext();
	Token name = scan->getNext();
	
	//Construct the argument
	switch (type.type) {
		//Registers
		case AsmTokenType::Reg: {
			int val = std::stoi(name.id);
			auto *reg = new LtacReg;
			reg->pos = val;
			parent->children.push_back(reg);
		} break;
	
		//Integers
		case AsmTokenType::Int: {
			int val = std::stoi(name.id);
			auto *i = new LtacInt(val);
			parent->children.push_back(i);
			
			if (inc_stack) stack_pos += 4;
		} break;
	
		//Strings
		case AsmTokenType::String: {
			auto *str = new LtacString;
			str->name = name.id;
			parent->children.push_back(str);
			
			if (inc_stack) stack_pos += 8;
		} break;
		
		default: syntax->fatalError("Unknown type.");
	}
}


