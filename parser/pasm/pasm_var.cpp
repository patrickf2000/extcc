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
			
			switch (node->get_type()) {
				case DataType::Int: {
					auto store = new IStrRet(pos);
					store->opType = Operand::Var;
					file->code.push_back(store);
				} break;
				
				//TODO: Add rest
			}
		} break;
		
		//TODO: Add the rest
	}
}


