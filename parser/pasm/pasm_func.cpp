#include "pasm_builder.hh"
#include <iostream>

using namespace PASM;

//Builds a function declaration
Func *PasmBuilder::buildFunc(AstNode *node) {
	auto *fd = static_cast<AstFuncDec *>(node);
	auto *func = new Func(fd->get_name());
	file->code.push_back(func);
	
	//Arguments
	for (auto arg : fd->args) {
		switch (arg.type) {
			//Integers
			case DataType::Int: {
				stackPos += 4;
				VarInfo v;
				v.pos = stackPos;
				v.type = DType::Int;
				v.size = 4;
				
				varPos[arg.name] = stackPos;
				vars[arg.name] = v;
				
				auto iarg = new ILdArg(stackPos);
				file->code.push_back(iarg);
			} break;
			
			//TODO: Add rest
		}
	}
	
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
					auto pusharg = new StrPushArg(name);
					file->code.push_back(pusharg);
				} break;
				
				//Raw integers
				case AstType::Int: {
					auto i = static_cast<AstInt *>(arg);
					auto pusharg = new IPushArg(i->get_val());
					pusharg->opType = Operand::Const;
					file->code.push_back(pusharg);
				} break;
				
				//Variables
				case AstType::Id: {
					auto id = static_cast<AstID *>(arg);
					auto var = vars[id->get_name()];
					
					switch (var.type) {
						case DType::Int: {
							auto pusharg = new IPushArg(var.pos);
							pusharg->opType = Operand::Var;
							file->code.push_back(pusharg);
						} break;
						
						//TODO: Add rest
					}
				} break;
				
				//TODO: Add rest
			}
		}
		
		auto call = new FuncCall(fc->get_name());
		file->code.push_back(call);
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

