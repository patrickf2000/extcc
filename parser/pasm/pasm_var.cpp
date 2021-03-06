#include "pasm_builder.hh"

using namespace PASM;

//Builds a variable declaration
void PasmBuilder::buildVarDec(AstNode *node) {
	auto vd = static_cast<AstVarDec *>(node);
	std::string name = vd->get_name();
	VarInfo info;
	int stackUp = 0;
	
	switch (vd->get_type()) {
		case DataType::Char: {
			stackUp = 1;
			info.size = 1;
			info.type = DType::Byte;
		} break;
		
		case DataType::Short: {
			stackUp = 2;
			info.size = 2;
			info.type = DType::Short;
		} break;
		
		case DataType::Int: {
			stackUp = 4;
			info.size = 4;
			info.type = DType::Int;
		} break;
		
		case DataType::Float: {
			stackUp = 4;
			info.size = 4;
			info.type = DType::Float32;
		} break;
		
		case DataType::Double: {
			stackUp = 8;
			info.size = 8;
			info.type = DType::Float64;
		} break;
		
		case DataType::Str: {
			stackUp = 8;
			info.size = 8;
			info.type = DType::String;
		} break;
	}
	
	if (vd->is_ptr) {
		info.type = DType::Ptr;
		stackUp = 8;
	}
	
	stackPos += stackUp;
	info.pos = stackPos;
	
	varPos[name] = stackPos;
	vars[name] = info;
	
	buildVarAssign(node);
}

//Builds a variable assignment
void PasmBuilder::buildVarAssign(AstNode *node) {
	auto assign = static_cast<AstVarAssign *>(node);
	buildVarAssign(assign);
}

void PasmBuilder::buildVarAssign(AstVarAssign *node) {
	int pos = varPos[node->get_name()];
	auto child = node->children[0];
	
	switch (child->type) {
		//Character constants
		case AstType::Char: {
			auto c = static_cast<AstChar *>(child);
			auto store = new BStoreConst(pos, c->get_val());
			file->code.push_back(store);
		} break;
	
		//Integer constants
		case AstType::Int: {
			auto i = static_cast<AstInt *>(child);
			auto store = new IStoreConst(pos, i->get_val());
			file->code.push_back(store);
		} break;
		
		//Float-32 constants
		case AstType::Float: {
			std::string name = buildFloat(child);
			auto store = new F32_StoreConst(pos, name);
			file->code.push_back(store);
		} break;
		
		//Float-64 constants
		case AstType::Double: {
			std::string name = buildDouble(child);
			auto store = new F64_StoreConst(pos, name);
			file->code.push_back(store);
		} break;
		
		//String constants
		case AstType::Str: {
			std::string name = buildString(child);
			auto store = new Str_StoreConst(pos, name);
			file->code.push_back(store);
		} break;
		
		//Variables
		case AstType::Id: {
			auto var2 = static_cast<AstID *>(child);
			auto v2 = vars[var2->get_name()];
			
			auto move = new MoveVV(pos, v2.pos);
			move->dType = v2.type;
			file->code.push_back(move);
		} break;
		
		//Math
		case AstType::Math: {
			auto math = static_cast<AstMath *>(child);
			auto info = vars[node->get_name()];
			
			switch (node->get_type()) {
				case DataType::Int: buildIMath(math, info); break;
				
				//TODO: Add rest
			}
		} break;
		
		//Increment operator
		case AstType::Inc: {
			auto info = vars[node->get_name()];
			
			switch (node->get_type()) {
				//Integer increment
				case DataType::Int: {
					auto math = new IMathVI(MathType::Add, info.pos, 1);
					file->code.push_back(math);
				} break;
				
				//TODO: Add rest
			}
		} break;
		
		//Function call
		case AstType::FuncCall: {
			buildFuncCall(child);
			
			if (node->is_ptr) {
				auto store = new Ptr_StrRet(pos);
				store->opType = Operand::Var;
				file->code.push_back(store);
			} else {
				switch (node->get_type()) {
					case DataType::Int: {
						auto store = new IStrRet(pos);
						store->opType = Operand::Var;
						file->code.push_back(store);
					} break;
					
					//TODO: Add rest
				}
			}
		} break;
		
		//Array access
		case AstType::ArrayAccess: {
			file->code.push_back(new PasmSpace);
			buildArrayAcc(child);
			
			switch (node->get_type()) {
				case DataType::Int: {
					auto store = new IStrPtr(pos);
					store->opType = Operand::Var;
					file->code.push_back(store);
				} break;
				
				//TODO: Add rest
			}
			
			file->code.push_back(new PasmSpace);
		} break;
		
		//TODO: Add the rest
	}
}

//Build array access
void PasmBuilder::buildArrayAcc(AstNode *node) {
	auto acc = static_cast<AstArrayAcc *>(node);
	auto index = acc->children[0];
	
	int ptrPos = varPos[acc->get_name()];
	int dest = 0;
	int size = 0;
	Operand posType;
	
	switch (acc->get_type()) {
		case DataType::Char: size = 1; break;
		case DataType::Short: size = 2; break;
		case DataType::Int:
		case DataType::Float: size = 4; break;
		case DataType::Double: size = 8; break;
	}
	
	switch (index->type) {
		case AstType::Int: {
			auto i = static_cast<AstInt *>(index);
			dest = i->get_val();
			posType = Operand::Const;
		} break;
		
		case AstType::Id: {
			auto id = static_cast<AstID *>(index);
			dest = varPos[id->get_name()];
			posType = Operand::Var;
		} break;
		
		//TODO: Fatal error	
	}
	
	auto load = new PtrLd(ptrPos, dest, size);
	load->posType = posType;
	file->code.push_back(load);
}

//Build array set
void PasmBuilder::buildArraySet(AstNode *node) {
	auto set = static_cast<AstArrayAssign *>(node);
	auto index = set->index;
	auto child = set->children[0];
	
	int ptrPos = varPos[set->get_name()];
	int dest = 0;
	int size = 0;
	Operand posType;
	
	switch (set->get_type()) {
		case DataType::Char: size = 1; break;
		case DataType::Short: size = 2; break;
		case DataType::Int:
		case DataType::Float: size = 4; break;
		case DataType::Double: size = 8; break;
	}
	
	switch (index->type) {
		case AstType::Int: {
			auto i = static_cast<AstInt *>(index);
			dest = i->get_val();
			posType = Operand::Const;
		} break;
		
		case AstType::Id: {
			auto id = static_cast<AstID *>(index);
			dest = varPos[id->get_name()];
			posType = Operand::Var;
		} break;
		
		//TODO: Fatal error	
	}
	
	auto load = new PtrStr(ptrPos, dest, size);
	load->posType = posType;
	
	//Now for the element being assigned
	switch (child->type) {
		//Integer constant
		case AstType::Int: {
			auto i = static_cast<AstInt *>(child);
			load->src = i->get_val();
			load->opType = Operand::Const;
		} break;
		
		//Variables
		case AstType::Id: {
			auto id = static_cast<AstID *>(child);
			load->src = varPos[id->get_name()];
			load->opType = Operand::Var;
		} break;
		
		//Math
		case AstType::Math: {
			load->src = 1;
			load->opType = Operand::Reg;
		
			auto math = static_cast<AstMath *>(child);
			auto info = vars[set->get_name()];
			info.pos = -2;
			
			switch (set->get_type()) {
				case DataType::Int: buildIMath(math, info); break;
				
				//TODO: Add rest
			}
		} break;
	
		//TODO: Add rest
	}
	
	file->code.push_back(new PasmSpace);
	file->code.push_back(load);
	file->code.push_back(new PasmSpace);
}


