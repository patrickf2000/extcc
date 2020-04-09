#include "asm_arm7.hh"

//Builds a variable operation
void Asm_Arm7::build_var(LtacNode *node) {
	auto var = static_cast<LtacVar *>(node);
	
	//Load the source into a register
	auto src = var->children[0];
	
	switch (src->type) {
		//Integers
		case ltac::Int: {
			auto i = static_cast<LtacInt *>(src);
			writer << "\tmov r3, #" << i->val << std::endl;
		} break;
		
		//Other variables
		case ltac::Var: {
			auto var2 = static_cast<LtacVar *>(src);
			
			writer << "\tldr r3, [fp, #-" << var2->pos << "]" << std::endl;
		} break;
		
		//TODO: Add the rest
	}
	
	//Store the value to the register
	writer << "\tstr r3, [fp, #-" << var->pos << "]" << std::endl;
}
