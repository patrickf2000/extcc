#include <iostream>
#include <cstdlib>

#include "asm_x64.hh"
#include "registers.hh"

//Build a vector load operation
void Asm_x64::build_vload(LtacNode *node) {
	auto vload = static_cast<LtacVLoad *>(node);
	int reg = vload->reg - 1;
	
	writer << "\tmov rax, [rbp-" << vload->pos << "]" << std::endl;
	writer << "\tvmovups " << vector_registers[reg] << ", [rax]" << std::endl;
}

//Load a vector register from an index
void Asm_x64::build_vloadi(LtacNode *node) {
	auto vload = static_cast<LtacVLoad *>(node);
	auto index = vload->children[0];
	int reg = vload->reg - 1;
	
	writer << "\tmov rax, [rbp-" << vload->pos << "]" << std::endl;
	
	switch (index->type) {
		//Integer
		case ltac::Int: {
			auto li = static_cast<LtacInt *>(index);
			int val = li->val * 4;
			
			writer << "\tadd rax, " << val << std::endl;
		} break;
		
		//Variable
		case ltac::Var: {
		
		} break;
		
		//Other- This is an error
		default: {
			std::cout << "[Arch error] Vector loading from an index is only valid "
				<< "with integers and variables." << std::endl;
			std::exit(1);
		}
	}
	
	writer << "\tvmovups " << vector_registers[reg] << ", [rax]" << std::endl;
}

//Build a vector store operation
void Asm_x64::build_vstore(LtacNode *node) {
	auto vstore = static_cast<LtacVStore *>(node);
	int reg = vstore->reg - 1;
	
	writer << "\tmov rax, [rbp-" << vstore->pos << "]" << std::endl;
	writer << "\tvmovups [rax], ";
	writer << vector_registers[reg] << std::endl;
}

//Builds vector math
void Asm_x64::build_vmath(LtacNode *node) {
	auto math = static_cast<LtacOp *>(node);
	
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
		case Operator::Add: {
			if (node->type == ltac::VIMath)
				writer << "\tvpaddd ";
			else if (node->type == ltac::VF32Math)
				writer << "\tvaddps ";
		} break;
		
		case Operator::Sub: {
			if (node->type == ltac::VIMath)
				writer << "\tvpsubd ";
			else if (node->type == ltac::VF32Math)
				writer << "\tvsubps ";
		} break;
		
		case Operator::Mul: {
			if (node->type == ltac::VIMath)
				writer << "\tvpmulld ";
			else if (node->type == ltac::VF32Math)
				writer << "\tvmulps "; 
		} break;
		
		case Operator::Div: {
			if (node->type == ltac::VIMath)
				writer << "\tvdivps ";
			else if (node->type == ltac::VF32Math)
				writer << "\tvdivps ";
		} break;
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

