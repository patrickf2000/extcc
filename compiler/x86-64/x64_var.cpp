#include "asm_x64.hh"

//Store an integer constant to a memory location
void X64::build_istorec(PasmNode *ln) {
	auto store = static_cast<IStoreConst *>(ln);
	
	writer << "\tmov DWORD PTR [rbp-" << store->pos << "], ";
	writer << store->val << std::endl;
}

//Move one variable to another
void X64::build_move_vv(PasmNode *ln) {
	auto move = static_cast<MoveVV *>(ln);
	
	std::string v1 = "[rbp-" + std::to_string(move->pos1) + "]";
	std::string v2 = "[rbp-" + std::to_string(move->pos2) + "]";
	
	switch (move->dType) {
		//Integers
		case DType::Int: {
			writer << "\tmov eax, DWORD PTR " << v2 << std::endl;
			writer << "\tmov DWORD PTR " << v1 << ", eax" << std::endl;
		} break;
		
		//TODO: Add rest
	}
	
	writer << std::endl;
}
