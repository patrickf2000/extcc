#include "asm_x64.hh"

//Store an integer constant to a memory location
void X64::build_istorec(PasmNode *ln) {
	auto store = static_cast<IStoreConst *>(ln);
	
	writer << "\tmov DWORD PTR [rbp-" << store->pos << "], ";
	writer << store->val << std::endl;
}
