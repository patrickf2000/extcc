#include <pasm/pasm.hh>

namespace PASM {

std::string getType(DType type) {
	switch (type) {
		case DType::Byte: return "b";
		case DType::Short: return "s";
		case DType::Int: return "i";
		case DType::Float32: return "f32";
		case DType::Float64: return "f64";
		case DType::String: return "str";
		case DType::Ptr: return "ptr";
	}

	return "";
}

std::string getMath(MathType type) {
	switch (type) {
		case MathType::Add: return "add";
		case MathType::Sub: return "sub";
		case MathType::Mul: return "mul";
		case MathType::Div: return "div";
	}
	
	return "";
}

//Generates a string from PASM tree
std::string unwrite(PasmFile *file) {
	std::string ret = "";
	
	//Build the data section
	ret += "section data\n";
	
	for (auto ln : file->data) {
		switch (ln->type) {
			//Strings
			case pasm::String: {
				auto str = static_cast<PasmString *>(ln);
				ret += "\tstring " + str->name + " \"" + str->val + "\"\n";
			} break;
		}
	}
	
	//Build the code section
	ret += "\nsection text\n";
	
	for (auto ln : file->code) {
		switch (ln->type) {
			//Space
			case pasm::Space: {
				ret += "\n";
			} break;
		
			//Functions
			case pasm::Func: {
				auto func = static_cast<Func *>(ln);
				ret += "func " + func->name;
				
				if (func->stackSize > 0)
					ret += "(" + std::to_string(func->stackSize) + ")";
					
				ret += "\n";
			} break;
			
			//Return
			case pasm::Ret: ret += "\tret\n\n"; break;
			
			//Pusharg
			case pasm::IPushArg: {
				auto pusharg = static_cast<IPushArg *>(ln);
				ret += "\ti.pusharg_";
				
				switch (pusharg->opType) {
					case Operand::Var: ret += "v VAR"; break;
					case Operand::Reg: ret += "r "; break;
					case Operand::Const: ret += "c "; break;
				}
				
				ret += std::to_string(pusharg->val) + "\n";
			} break;
			
			case pasm::StrPushArg: {
				auto pusharg = static_cast<StrPushArg *>(ln);
				ret += "\tstr.pusharg " + pusharg->name + "\n";
			} break;
			
			//Function call
			case pasm::FuncCall: {
				auto call = static_cast<FuncCall *>(ln);
				ret += "\tcall " + call->name + "\n";
			} break;
			
			//Load integer function argument
			case pasm::ILdArg: {
				auto arg = static_cast<ILdArg *>(ln);
				ret += "\ti.ldarg VAR" + std::to_string(arg->pos) + "\n";
			} break;
			
			//Load/Store
			//Store constant
			case pasm::IStoreC: {
				auto store = static_cast<IStoreConst *>(ln);
				ret += "\ti.store_c VAR" + std::to_string(store->pos);
				ret += " " + std::to_string(store->val) + "\n";
			} break;
			
			//Move one var to another
			case pasm::MoveVV: {
				auto move = static_cast<MoveVV *>(ln);
				ret += "\t" + getType(move->dType) + ".move_v VAR";
				ret += std::to_string(move->pos1) + " VAR" + std::to_string(move->pos2);
				ret += "\n";
			} break;
			
			//Load variable to register
			case pasm::LdrV: {
				auto ldrv = static_cast<LdrV *>(ln);
				ret += "\t" + getType(ldrv->dType) + ".ldr_v r";
				ret += std::to_string(ldrv->reg) + " VAR" + std::to_string(ldrv->pos);
				ret += "\n";
			} break;
			
			//Store a register to variable
			case pasm::Str: {
				auto str = static_cast<Str *>(ln);
				ret += "\t" + getType(str->dType) + ".str VAR";
				ret += std::to_string(str->pos) + " r" + std::to_string(str->reg);
				ret += "\n";
			} break;
			
			//Load integer to return register
			case pasm::ILdRet: {
				auto store = static_cast<ILdRet *>(ln);
				ret += "\ti.ldret_";
				
				switch (store->opType) {
					case Operand::Var: ret += "v VAR"; break;
					case Operand::Reg: ret += "r "; break;
					case Operand::Const: ret += "c "; break;
				}
				
				ret += std::to_string(store->val) + "\n";
			} break;
			
			//Store integer from return register
			case pasm::IStrRet: {
				auto store = static_cast<IStrRet *>(ln);
				ret += "\ti.stret_";
				
				switch (store->opType) {
					case Operand::Var: ret += "v VAR"; break;
					case Operand::Reg: ret += "r r"; break;
				}
				
				ret += std::to_string(store->val) + "\n";
			} break;
			
			//Math- register <- immediate
			case pasm::IMathRI: {
				auto math = static_cast<IMathRI *>(ln);
				ret += "\ti." + getMath(math->mType)  + "_ri r" + std::to_string(math->reg);
				ret += " " + std::to_string(math->val) + "\n";
			} break;
			
			//Math- register <- variable
			case pasm::IMathRV: {
				auto math = static_cast<IMathRV *>(ln);
				ret += "\ti." + getMath(math->mType) + "_rv r" + std::to_string(math->reg);
				ret += " VAR" + std::to_string(math->pos) + "\n";
			} break;
			
			//System call arguments
			case pasm::ISysarg: {
				auto arg = static_cast<ISysArg *>(ln);
				ret += "\ti.sysarg " + std::to_string(arg->arg) + "\n";
			} break;
			
			case pasm::StrSysarg: {
				auto arg = static_cast<StrSysArg *>(ln);
				ret += "\tstr.sysarg " + arg->arg + "\n";
			} break;
			
			//The syscall command
			case pasm::Syscall: ret += "\tsyscall\n\n"; break;
		}
	}
	
	return ret;
}

}
