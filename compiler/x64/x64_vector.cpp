#include "asm_x64.hh"
#include "registers.hh"

//Build a vector load operation
void Asm_x64::build_vload(LtacNode *node) {
	auto vload = static_cast<LtacVLoad *>(node);
	int reg = vload->reg - 1;
	
	writer << "\tvmovups " << vector_registers[reg];
	writer << ", [rbp-" << vload->pos << "]" << std::endl;
}
