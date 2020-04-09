#include "asm_arm7.hh"

//Builds a function declaration
void Asm_Arm7::build_func(LtacNode *node) {
	auto fd = static_cast<LtacFunc *>(node);
	
	if (fd->name == "main") {
		writer << ".global main" << std::endl;
	}
	
	writer << std::endl;
	writer << fd->name << ":" << std::endl;
}

//Builds a function return
void Asm_Arm7::build_ret(LtacNode *node) {
	writer << "\tbx lr" << std::endl;
}
