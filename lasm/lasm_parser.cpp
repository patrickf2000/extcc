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
			case AsmTokenType::Lbl: buildLabel(); break;
			
			case AsmTokenType::Func: buildFunc(); break;
			case AsmTokenType::Extern: buildExtern(); break;
			case AsmTokenType::Call: buildFuncCall(); break;
			case AsmTokenType::Ret: buildRet(); break;
			case AsmTokenType::PushArg: buildPushArg(); break;
			case AsmTokenType::String: buildString(); break;
			case AsmTokenType::Float: buildFloat(); break;
			case AsmTokenType::Var: buildVar(); break;
			case AsmTokenType::Ptr: buildVar(true); break;
			case AsmTokenType::Ldr: buildLdr(); break;
			case AsmTokenType::FLdr: buildLdr(RegType::Flt); break;
			case AsmTokenType::Str: buildStr(); break;
			case AsmTokenType::FStr: buildStr(RegType::Flt); break;
			case AsmTokenType::StrRet: buildStr(RegType::Ret); break;

			case AsmTokenType::F64_Ldr: buildLdr(RegType::Flt64); break;
			case AsmTokenType::F64_Str: buildStr(RegType::Flt64); break;
			
			case AsmTokenType::Vldr: buildVLdr(1); break;
			case AsmTokenType::Vldri: buildVLdr(2); break;
			case AsmTokenType::Vstr: buildVLdr(3); break;
			
			case AsmTokenType::IAdd:
			case AsmTokenType::ISub:
			case AsmTokenType::IMul:
			case AsmTokenType::IDiv: 
			case AsmTokenType::IMod: buildMath(1, type); break;
			
			case AsmTokenType::VIAdd:
			case AsmTokenType::VISub:
			case AsmTokenType::VIMul:
			case AsmTokenType::VIDiv: buildMath(4, type); break;
			
			case AsmTokenType::F32_Add:
			case AsmTokenType::F32_Sub:
			case AsmTokenType::F32_Mul:
			case AsmTokenType::F32_Div: buildMath(2, type); break;
			
			case AsmTokenType::F32_VAdd:
			case AsmTokenType::F32_VSub:
			case AsmTokenType::F32_VMul:
			case AsmTokenType::F32_VDiv: buildMath(5, type); break;

			case AsmTokenType::F64_Add:
			case AsmTokenType::F64_Sub:
			case AsmTokenType::F64_Mul:
			case AsmTokenType::F64_Div: buildMath(3, type); break;
			
			case AsmTokenType::ArraySet: buildArraySet(); break;
			case AsmTokenType::ArrayAcc: buildArrayAcc(); break;
			
			case AsmTokenType::ICmp: buildCmp(); break;
			case AsmTokenType::Jl: buildJmp(type); break;
			
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

//Builds a label
void AsmParser::buildLabel() {
	checkCode();
	Token next = scan->getNext();
	
	if (next.type != AsmTokenType::Name)
		syntax->fatalError("Expected label name.");
		
	auto lbl = new LtacLabel;
	lbl->name = next.id;
	file->code->children.push_back(lbl);
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

//Builds a float declaration
void AsmParser::buildFloat() {
	checkData();
	
	Token next = scan->getNext();
	Token flt = scan->getNext();
	
	if (next.type != AsmTokenType::Name)
		syntax->fatalError("Expected variable name.");
	else if (flt.type != AsmTokenType::FloatL)
		syntax->fatalError("Expected float literal.");
		
	buildFloat(next.id, flt.id);
}

LtacFloat *AsmParser::buildFloat(std::string name, std::string val) {
	if (name == "") {
		name = "FLT_" + std::to_string(flt_count);
		++flt_count;
	}

	auto *l_flt = new LtacFloat;
		
	float f = std::stod(val);
	char buf[32];
	sprintf(buf, "%d", *(unsigned int*)&f);
	l_flt->i_val = std::stoi(std::string(buf));
	
	l_flt->val = f;
	l_flt->name = name;
	file->data->children.push_back(l_flt);
	
	auto *l_flt2 = new LtacFloat;
	l_flt2->name = name;
	l_flt2->val = f;
	return l_flt2;
}

//Builds a double declaration
void AsmParser::buildDouble() {
	checkData();

	Token next = scan->getNext();
	Token flt = scan->getNext();

	if (next.type != AsmTokenType::Name)
		syntax->fatalError("Expected variable name.");
	else if (flt.type != AsmTokenType::FloatL)
		syntax->fatalError("Expected float literal.");

	buildDouble(next.id, flt.id);
}

LtacDouble *AsmParser::buildDouble(std::string name, std::string val) {
	if (name == "") {
		name = "FLT_" + std::to_string(flt_count);
		++flt_count;
	}

	auto *l_dbl = new LtacDouble;

	double d = std::stod(val);
	char buf[64];
	sprintf(buf, "%lu", *(uint64_t*)&d);
	l_dbl->i_val = std::string(buf);

	l_dbl->val = d;
	l_dbl->name = name;
	file->data->children.push_back(l_dbl);

	auto *l_dbl2 = new LtacDouble;
	l_dbl2->name = name;
	l_dbl2->val = d;
	return l_dbl2;
}

//Builds a variable declaration
void AsmParser::buildVar(bool is_ptr) {
	auto *var = new LtacVar;
	var->is_ptr = is_ptr;
	file->code->children.push_back(var);
	
	Token name = scan->getNext();
	
	if (name.type != AsmTokenType::Name)
		syntax->fatalError("Expected variable name.");
	
	//Build the type
	Token type = scan->getNext();
	DataType dt = DataType::None;
	
	switch (type.type) {
		case AsmTokenType::Int: dt = DataType::Int; break;
		case AsmTokenType::Float: dt = DataType::Float; break;
		case AsmTokenType::Double: dt = DataType::Double; break;
		case AsmTokenType::String: dt = DataType::Str; break;
		
		//TODO: Add rest		
	}
	
	if (vars[name.id] == 0) {
		types[name.id] = dt;
		pointers[name.id] = is_ptr;

		//Build the rest
		if (!is_ptr) {
			scan->unget(type);
			addChildren(var, true);
		} else {
			stack_pos += 8;
		}

		vars[name.id] = stack_pos;
		var->pos = stack_pos;
	} else {
		if (!is_ptr) {
			scan->unget(type);
			addChildren(var, false);
		}
		
		var->pos = vars[name.id];
	}
	
	var->d_type = dt;
}

//Builds a single math command
// 1-> Integer
// 2-> Single precision float
// 3-> Double precision float
// 4-> Vector-Integer
// 5-> Vector-Single precision float
void AsmParser::buildMath(int type, int op) {
	checkCode();
	LtacOp *math = new LtacOp;

	if (type == 1)
		math = new LtacIMath;
	else if (type == 2)
		math = new LtacF32Math;
	else if (type == 4)
		math = new LtacVIMath;
	else if (type == 5)
		math = new LtacVF32Math;
	
	switch (op) {
		case AsmTokenType::IAdd:
		case AsmTokenType::VIAdd:
		case AsmTokenType::F32_Add: 
		case AsmTokenType::F32_VAdd: math->op = Operator::Add; break;
		
		case AsmTokenType::ISub:
		case AsmTokenType::VISub: 
		case AsmTokenType::F32_Sub: 
		case AsmTokenType::F32_VSub: math->op = Operator::Sub; break;
		
		case AsmTokenType::IMul:
		case AsmTokenType::VIMul:
		case AsmTokenType::F32_Mul:
		case AsmTokenType::F32_VMul: math->op = Operator::Mul; break;
		
		case AsmTokenType::IDiv:
		case AsmTokenType::VIDiv:
		case AsmTokenType::F32_Div: 
		case AsmTokenType::F32_VDiv: math->op = Operator::Div; break;
		
		case AsmTokenType::IMod: math->op = Operator::Mod; break;
	}
	
	addChildren(math, false);
	addChildren(math, false);
	
	file->code->children.push_back(math);
}

//Adds children to a parent node
LtacNode *AsmParser::addChildren(LtacNode *parent, bool inc_stack) {
	checkCode();
	
	Token type = scan->getNext();
	Token name = scan->getNext();
	LtacNode *ret;
	
	//Construct the argument
	switch (type.type) {
		//Registers
		case AsmTokenType::Reg: {
			int val = std::stoi(name.id);
			auto *reg = new LtacReg;
			reg->pos = val;
			//parent->children.push_back(reg);
			ret = reg;
		} break;
		
		//Variables
		case AsmTokenType::Var: {
			auto *var = new LtacVar;
			var->pos = vars[name.id];
			var->d_type = types[name.id];
			var->is_ptr = pointers[name.id];
			//parent->children.push_back(var);
			ret = var;
		} break;
	
		//Integers
		case AsmTokenType::Int: {
			int val = std::stoi(name.id);
			auto *i = new LtacInt(val);
			//parent->children.push_back(i);
			ret = i;
			
			if (inc_stack) stack_pos += 4;
		} break;
		
		//Float literals
		case AsmTokenType::FloatL: {
			ret = buildFloat("", name.id);
			if (inc_stack) stack_pos += 4;
		} break;

		//Double float literals
		case AsmTokenType::Double: {
			if (name.type == AsmTokenType::Name) {
				auto *d = new LtacDouble;
				d->name = name.id;
				ret = d;
			} else {
				ret = buildDouble("", name.id);
			}

			if (inc_stack) stack_pos += 8;
		} break;
		
		//Floats
		case AsmTokenType::Float: {
			if (name.type == AsmTokenType::Name) {
				auto *f = new LtacFloat;
				f->name = name.id;
				//parent->children.push_back(f);
				ret = f;
			} else {
				ret = buildFloat("", name.id);
			}
			
			if (inc_stack) stack_pos += 4;
		} break;
	
		//Strings
		case AsmTokenType::String: {
			auto *str = new LtacString;
			str->name = name.id;
			//parent->children.push_back(str);
			ret = str;
			
			if (inc_stack) stack_pos += 8;
		} break;
		
		default: syntax->fatalError("Unknown type.");
	}
	
	if (parent != nullptr)
		parent->children.push_back(ret);
		
	return ret;
}

DataType AsmParser::getDataType(Token t) {
	switch (t.type) {
		case AsmTokenType::Int: return DataType::Int;
		case AsmTokenType::Float: return DataType::Float;
		case AsmTokenType::Void: return DataType::Void;
		case AsmTokenType::String: return DataType::Str;
		case AsmTokenType::Any: return DataType::Any;
	}
	
	syntax->fatalError("Unknown data type!");
	return DataType::None;
}



