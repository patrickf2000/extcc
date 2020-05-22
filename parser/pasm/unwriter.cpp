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
				ret += "func " + func->name + "\n";
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
