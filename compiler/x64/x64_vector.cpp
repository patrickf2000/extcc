#include <iostream>
#include <cstdlib>

#include "asm_x64.hh"
#include "registers.hh"

//Build a vector load operation
void Asm_x64::build_vload(LtacNode *node) {
	auto vload = static_cast<LtacVLoad *>(node);
	int reg = vload->reg - 1;
	
	writer << "\tvmovdqu " << vector_registers[reg];
	writer << ", [rbp-" << vload->pos << "]" << std::endl;
}

//Build a vector store operation
void Asm_x64::build_vstore(LtacNode *node) {
	auto vstore = static_cast<LtacVStore *>(node);
	int reg = vstore->reg - 1;
	
	writer << "\tvmovdqu [rbp-" << vstore->pos << "], ";
	writer << vector_registers[reg] << std::endl;
}

//Builds vector integer math
void Asm_x64::build_vimath(LtacNode *node) {
	auto math = static_cast<LtacVIMath *>(node);
	
	auto dest = math->children[0];
	auto src = math->children[1];
	
	//Syntax check
	//On AVX, you can have register-register or memory-register
	if (dest->type != ltac::Reg) {
		std::cout << "[Arch error] Intel AVX vector extensions require destination to be a register.";
		std::cout << std::endl;
		std::exit(1);
	}

	//Determine the destination register
	auto lreg = static_cast<LtacReg *>(dest);
	int rpos = lreg->pos - 1;
	auto reg = vector_registers[rpos];
	
	//Determine the operation
	switch (math->op) {
		case Operator::Add: writer << "\tvpaddd "; break;
		case Operator::Sub: writer << "\tvpsubd "; break;
		case Operator::Mul: writer << "\tvpmulld "; break;
		case Operator::Div: break;
	}
	
	writer << reg << ", " << reg << ", ";
	
	//Determine the source
	if (src->type == ltac::Reg) {
		auto lr = static_cast<LtacReg *>(src);
		rpos = lr->pos - 1;
		writer << vector_registers[rpos];
	} else if (src->type == ltac::Var) {
		auto lvar = static_cast<LtacVar *>(src);
		writer << "[rbp-" << lvar->pos << "]";
	} else {
		std::cout << "[Arch error] Invalid source operand." << std::endl;
		std::cout << "Intel AVX vector extensions require source to be a register or memory location.";
		std::cout << std::endl;
		std::exit(1);
	}
	
	writer << std::endl;
}

