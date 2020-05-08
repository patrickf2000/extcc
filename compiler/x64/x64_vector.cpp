#include "asm_x64.hh"
#include "registers.hh"

//Build a vector load operation
void Asm_x64::build_vload(LtacNode *node) {
	auto vload = static_cast<LtacVLoad *>(node);
	int reg = vload->reg - 1;
	
	writer << "\tvmovups " << vector_registers[reg];
	writer << ", [rbp-" << vload->pos << "]" << std::endl;
}

//Build a vector store operation
void Asm_x64::build_vstore(LtacNode *node) {
	auto vstore = static_cast<LtacVStore *>(node);
	int reg = vstore->reg - 1;
	
	writer << "\tvmovups [rbp-" << vstore->pos << "], ";
	writer << vector_registers[reg] << std::endl;
}
