#include "asm_arm7.hh"
#include "registers.hh"

using namespace Arm7;

//Builds a register operation
void Asm_Arm7::build_reg(LtacNode *node) {
	auto reg = static_cast<LtacReg *>(node);
	int pos = reg->pos - 1;
	
	//Load the source
	auto src = reg->children[0];
	
	switch (src->type) {
		//Integers
		case ltac::Int: {
			auto i = static_cast<LtacInt *>(src);
			
			writer << "\tmov " << registers[pos] << ", #";
			writer << i->val << std::endl;
		} break;
		
		//Other variables
		case ltac::Var: {
			auto var = static_cast<LtacVar *>(src);
			
			writer << "\tldr " << registers[pos] << ", [fp, #-";
			writer << var->pos << "]" << std::endl;
		} break;
	}
}

//Builds a variable operation
void Asm_Arm7::build_var(LtacNode *node) {
	auto var = static_cast<LtacVar *>(node);
	
	//Load the source into a register
	auto src = var->children[0];
	std::string dest = "r3";
	
	switch (src->type) {
		//Integers
		case ltac::Int: {
			auto i = static_cast<LtacInt *>(src);
			writer << "\tmov r3, #" << i->val << std::endl;
		} break;
		
		//Registers
		case ltac::Reg: {
			auto reg = static_cast<LtacReg *>(src);
			int pos = reg->pos - 1;
			dest = registers[pos];
		} break;
		
		//Other variables
		case ltac::Var: {
			auto var2 = static_cast<LtacVar *>(src);
			
			writer << "\tldr r3, [fp, #-" << var2->pos << "]" << std::endl;
		} break;
		
		//TODO: Add the rest
	}
	
	//Store the value to the register
	writer << "\tstr " + dest + ", [fp, #-" << var->pos << "]" << std::endl;
}
