#include "pasm_builder.hh"

using namespace PASM;

//The constructor- set things up
PasmBuilder::PasmBuilder(std::string file) {
	this->file = new PasmFile;
	this->file->name = file;
}

//Build a PASM file
PasmFile *PasmBuilder::buildFile(AstNode *top) {
	assemble(top);
	return file;
}

//The main assembly function
void PasmBuilder::assemble(AstNode *top) {
	for (auto node : top->children) {
		switch (node->type) {
			case AstType::Scope: assemble(node); break;
			
			case AstType::FuncDec: {
				buildFunc(node); 
				assemble(node);
			} break;
			
			case AstType::FuncCall: buildFuncCall(node); break;
			
			case AstType::VarDec: buildVarDec(node); break;
			case AstType::VarAssign: buildVarAssign(node); break;
		}
	}
}

//Builds a function declaration
void PasmBuilder::buildFunc(AstNode *node) {
	auto *fd = static_cast<AstFuncDec *>(node);
	auto *func = new Func(fd->get_name());
	file->code.push_back(func);
}

//Builds a function call
void PasmBuilder::buildFuncCall(AstNode *node) {
	auto *fc = static_cast<AstFuncCall *>(node);
	
	if (fc->get_name() == "syscall") {
		for (auto arg : fc->children) {
			switch (arg->type) {
				case AstType::Int: {
					auto *i = static_cast<AstInt *>(arg);
					auto *sysarg = new ISysArg(i->get_val());
					sysarg->opType = Operand::Const;
					file->code.push_back(sysarg);
				} break;
				
				//TODO: Add others
			}
		}
		
		auto syscall = new SysCall;
		file->code.push_back(syscall);
	} else {
		//TODO: Add regular function calls
	}
}

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
		
		//TODO: Add the rest
	}
}


