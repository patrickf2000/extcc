#include "asm_x64.hh"

//Builds a function declaration
void X64::build_func(PasmNode *ln) {
	auto func = static_cast<Func *>(ln);
	
	writer << std::endl;
	writer << ".globl " << func->name << std::endl;
	
	writer << std::endl;
	writer << func->name << ":" << std::endl;
	
	//Set up the stack
	writer << "\tpush rbp" << std::endl;
	writer << "\tmov rbp, rsp" << std::endl;
	
	if (func->stackSize > 0)
		writer << "\tsub rsp, " << func->stackSize << std::endl;
		
	writer << std::endl;
}
