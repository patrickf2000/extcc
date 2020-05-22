#include "asm_x64.hh"

std::string sysargs[] = {
	"rdi",
	"rsi",
	"rdx",
	"r10",
	"r8",
	"r9"
};

std::string sysargs32[] = {
	"edi",
	"esi",
	"edx",
	"e10",
	"r8d",
	"r9d"
};

int sysarg_pos = 0;

//Integer system call argument
void X64::build_isysarg(PasmNode *ln) {
	auto arg = static_cast<ISysArg *>(ln);
	
	if (sysarg_pos == 0)
		writer << std::endl;
	
	std::string reg = sysargs32[sysarg_pos];
	++sysarg_pos;
	
	writer << "\tmov " << reg << ", " << arg->arg << std::endl;
}

//String system call argument
void X64::build_str_sysarg(PasmNode *ln) {

}

//Invoke the kernel
void X64::build_syscall(PasmNode *ln) {
	writer << "\tsyscall" << std::endl;
	writer << std::endl;
	
	sysarg_pos = 0;
}
