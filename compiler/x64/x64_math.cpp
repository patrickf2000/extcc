#include "asm_x64.hh"
#include "registers.hh"

//Builds an integer math statement
void Asm_x64::build_imath(LtacNode *node) {
	auto math = static_cast<LtacIMath *>(node);
	
	auto dest = math->children[0];
	auto src = math->children[1];
	bool src_mem = false;
	
	//Memory-to-memory is impossible
	if (dest->type == ltac::Var && src->type == ltac::Var) {
		auto var = static_cast<LtacVar *>(src);
		
		writer << "\tmov r9d, DWORD PTR [rbp-" << var->pos;
		writer << "]" << std::endl;
		
		src_mem = true;
	}
	
	//Determine the instruction
	switch (math->op) {
		case Operator::Add: writer << "\tadd "; break;
		case Operator::Sub: writer << "\tsub "; break;
		case Operator::Mul: writer << "\timul "; break;
		case Operator::Div:
		case Operator::Mod: {
			//TODO: Add me
			return;
		} break;
	}
	
	//Build the destination
	switch (dest->type) {
		//Register
		case ltac::Reg: {
			auto reg = static_cast<LtacReg *>(dest);
			int pos = reg->pos - 1;
			
			writer << registers[pos] << ", ";
		} break;
		
		//Variable
		case ltac::Var: {
			//TODO: Add me
		} break;
	}
	
	//Build the source
	switch (src->type) {
		//Register
		case ltac::Reg: {
			auto reg = static_cast<LtacReg *>(src);
			int pos = reg->pos - 1;
			
			writer << registers[pos] << std::endl;
		} break;
		
		//Variable
		case ltac::Var: {
			//TODO: Add me
		} break;
		
		//Integer
		case ltac::Int: {
			//TODO: Add me
		} break;
	}
}
