#include "asm_x64.hh"

//Stores an integer immediate to a memory location
void Asm_x64::build_istore_imm(LtacNode *node) {
	auto store = static_cast<LtacIStrImm *>(node);
	
	writer << "\tmov DWORD PTR [rbp-" << store->pos << "], ";
	writer << store->val << std::endl;
}
