#include "pasm_builder.hh"

using namespace PASM;

//Builds a function declaration
Func *PasmBuilder::buildFunc(AstNode *node) {
	auto *fd = static_cast<AstFuncDec *>(node);
	auto *func = new Func(fd->get_name());
	file->code.push_back(func);
	
	retType = fd->rtype;
	
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
			
			//Floats
			case DataType::Float: {
				stackPos += 4;
				VarInfo v;
				v.pos = stackPos;
				v.type = DType::Float32;
				v.size = 4;
				
				varPos[arg.name] = stackPos;
				vars[arg.name] = v;
				
				auto farg = new F32_LdArg(stackPos);
				file->code.push_back(farg);
			} break;
			
			//TODO: Add rest
		}
	}
	
	return func;
}

//Builds a function call
void PasmBuilder::buildFuncCall(AstNode *node) {
	auto *fc = static_cast<AstFuncCall *>(node);
	int arg_pos = 0;
	
	if (fc->get_name() == "syscall") {
		for (auto arg : fc->children) {
			if (arg_pos == 0)
				file->code.push_back(new PasmSpace);
			++arg_pos;
				
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
		bool f32Promote = false;
		if (fc->get_name() == "printf") f32Promote = true;
	
		for (auto arg : fc->children) {
			if (arg_pos == 0)
				file->code.push_back(new PasmSpace);
			++arg_pos;
		
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
				
				//Raw float-32
				case AstType::Float: {
					auto name = buildFloat(arg);
					auto pusharg = new F32_PushArg(name);
					pusharg->opType = Operand::Const;
					pusharg->promote = f32Promote;
					file->code.push_back(pusharg);
				} break;
				
				//Raw float-64
				case AstType::Double: {
					auto name = buildDouble(arg);
					auto pusharg = new F64_PushArg(name);
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
						
						case DType::Float32: {
							auto pusharg = new F32_PushArg(var.pos);
							pusharg->opType = Operand::Var;
							pusharg->promote = f32Promote;
							file->code.push_back(pusharg);
						} break;
						
						case DType::Float64: {
							auto pusharg = new F64_PushArg(var.pos);
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
	
	arg_pos = 0;
	file->code.push_back(new PasmSpace);
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
				case AstType::Id: {
					auto id = static_cast<AstID *>(child);
					int pos = varPos[id->get_name()];
					
					auto arg = new ILdRet(pos);
					arg->opType = Operand::Var;
					file->code.push_back(arg);
				} break;
				
				//Math
				case AstType::Math: {
					if (retType == DataType::Int) {
						auto math = static_cast<AstMath *>(child);
						VarInfo info;
						info.pos = -1;
						info.type = DType::Int;
						info.size = 4;
						
						buildIMath(math, info);
					} else {
						//TODO: Die
					}
				} break;
				
				//TODO: Add rest
			}
		}
	}
	
	file->code.push_back(new Ret);
}

