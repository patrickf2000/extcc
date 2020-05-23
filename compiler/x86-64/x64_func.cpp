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

//Load an integer value to a return register
void X64::build_ildret(PasmNode *ln) {
	auto store = static_cast<ILdRet *>(ln);
	int val = store->val;
	
	switch (store->opType) {
		case Operand::Var: {
			writer << "\tmov eax, DWORD PTR [rbp-" << val << "]" << std::endl;
		} break;
		
		case Operand::Reg: {
			warning("i.stret_r not supported yet.");
		} break;
		
		case Operand::Const: {
			writer << "\tmov eax, " << val << std::endl;
		} break;
		
		default: fatalError("Unknown command.");
	}
}

//On x86, this is the simplest command :)
void X64::build_ret() {
	writer << "\tleave" << std::endl;
	writer << "\tret" << std::endl;
	writer << std::endl;
}
