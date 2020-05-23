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
				auto func = buildFunc(node); 
				assemble(node);
				
				int stackSize = 0;
				if (stackPos > 0) {
					while (stackSize < (stackPos + 1))
						stackSize += 16;
				}
				
				func->stackSize = stackSize;
				stackPos = 0;
			} break;
			
			case AstType::FuncCall: buildFuncCall(node); break;
			case AstType::Return: buildRet(node); break;
			
			case AstType::VarDec: buildVarDec(node); break;
			case AstType::VarAssign: buildVarAssign(node); break;
		}
	}
}

//Builds a string and adds to the data section
std::string PasmBuilder::buildString(AstNode *node) {
	auto str = static_cast<AstString *>(node);
	auto name = "STR" + std::to_string(strPos);
	++strPos;
	
	auto pstr = new PasmString(name, str->get_val());
	file->data.push_back(pstr);
	
	return name;
}

//Builds a function declaration
Func *PasmBuilder::buildFunc(AstNode *node) {
	auto *fd = static_cast<AstFuncDec *>(node);
	auto *func = new Func(fd->get_name());
	file->code.push_back(func);
	return func;
}

//Builds a function call
void PasmBuilder::buildFuncCall(AstNode *node) {
	auto *fc = static_cast<AstFuncCall *>(node);
	
	if (fc->get_name() == "syscall") {
		for (auto arg : fc->children) {
			switch (arg->type) {
				//Raw integers
				case AstType::Int: {
					auto *i = static_cast<AstInt *>(arg);
					auto *sysarg = new ISysArg(i->get_val());
					sysarg->opType = Operand::Const;
					file->code.push_back(sysarg);
				} break;
				
				//Variables
				case AstType::Id: {
					auto *id = static_cast<AstID *>(arg);
					auto var = vars[id->get_name()];
					
					if (var.type == DType::Int) {
						auto sysarg = new ISysArg(var.pos);
						sysarg->opType = Operand::Var;
						file->code.push_back(sysarg);
					} else if (var.type == DType::String) {
					
					} else {
					
					}
				} break;
				
				//TODO: Add others
			}
		}
		
		auto syscall = new SysCall;
		file->code.push_back(syscall);
	} else {
		for (auto arg : fc->children) {
			switch (arg->type) {
				//Raw strings
				case AstType::Str: {
					auto name = buildString(arg);
				} break;
				
				//TODO: Add rest
			}
		}
	}
}

//Builds a return statement
void PasmBuilder::buildRet(AstNode *node) {
	if (node->children.size() > 0) {
		for (auto child : node->children) {
			switch (child->type) {
				//Raw integers
				case AstType::Int: {
					auto i = static_cast<AstInt *>(child);
					auto arg = new ILdRet(i->get_val());
					arg->opType = Operand::Const;
					file->code.push_back(arg);
				} break;
				
				//Variables
				
				//TODO: Add rest
			}
		}
	}
	
	file->code.push_back(new Ret);
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


