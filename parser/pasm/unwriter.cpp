#include <pasm/pasm.hh>

namespace PASM {

//Generates a string from PASM tree
std::string unwrite(PasmFile *file) {
	std::string ret = "";
	
	for (auto ln : file->code) {
		switch (ln->type) {
			//Functions
			case pasm::Func: {
				auto func = static_cast<Func *>(ln);
				ret += "func " + func->name;
				
				if (func->stackSize > 0)
					ret += "(" + std::to_string(func->stackSize) + ")";
					
				ret += "\n";
			} break;
			
			//Return
			case pasm::Ret: ret += "ret\n\n"; break;
			
			//Load/Store
			//Store constant
			case pasm::IStoreC: {
				auto store = static_cast<IStoreConst *>(ln);
				ret += "\ti.store_c VAR" + std::to_string(store->pos);
				ret += " " + std::to_string(store->val) + "\n";
			} break;
			
			//Store integer to return register
			case pasm::IStrRet: {
				auto store = static_cast<IStrRet *>(ln);
				ret += "\ti.stret_";
				
				switch (store->opType) {
					case Operand::Var: ret += "v VAR"; break;
					case Operand::Reg: ret += "r "; break;
					case Operand::Const: ret += "c "; break;
				}
				
				ret += std::to_string(store->val) + "\n";
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
