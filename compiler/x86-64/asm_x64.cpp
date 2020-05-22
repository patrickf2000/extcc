#include "asm_x64.hh"

//Opens the file
X64::X64(std::string name) {
	writer = std::ofstream(name);
	
	writer << ".intel_syntax noprefix" << std::endl;
}

//The main loop to generate code for the text section
void X64::build_code(PasmFile *file) {
	writer << ".text" << std::endl;
	writer << std::endl;

	for (auto ln : file->code) {
		switch (ln->type) {
			//Functions
			case pasm::Func: build_func(ln); break;
			
			//Syscall arguments
			case pasm::ISysarg: build_isysarg(ln); break;
			case pasm::StrSysarg: build_str_sysarg(ln); break;
			
			//Syscalls
			case pasm::Syscall: build_syscall(ln); break;
		}
	}
}

//Close out the file
void X64::write() {
	writer.close();
}
