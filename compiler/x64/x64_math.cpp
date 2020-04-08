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
	
	//If the source is a function call, that must be built first
	if (src->type == ltac::FuncCall)
		build_func_call(src);
	
	//Determine the instruction
	switch (math->op) {
		case Operator::Add: writer << "\tadd "; break;
		case Operator::Sub: writer << "\tsub "; break;
		case Operator::Mul: writer << "\timul "; break;
		case Operator::Div:
		case Operator::Mod: {
			build_idiv(math);
			return;
		} break;
	}
	
	//Build the destination
	switch (dest->type) {
		//Register
		case ltac::Reg: {
			auto reg = static_cast<LtacReg *>(dest);
			int pos = reg->pos - 1;
			
			writer << registers32[pos] << ", ";
		} break;
		
		//Variable
		case ltac::Var: {
			auto var = static_cast<LtacVar *>(dest);
			
			writer << "DWORD PTR [rbp-" << var->pos;
			writer << "], ";
		} break;
	}
	
	//Build the source
	switch (src->type) {
		//Register
		case ltac::Reg: {
			auto reg = static_cast<LtacReg *>(src);
			int pos = reg->pos - 1;
			
			writer << registers32[pos] << std::endl;
		} break;
		
		//Variable
		case ltac::Var: {
			auto var = static_cast<LtacVar *>(src);
			
			if (src_mem) {
				writer << "r9d" << std::endl;
			} else {
				writer << "DWORD PTR [rbp-" << var->pos;
				writer << "]" << std::endl;
			}
		} break;
		
		//Integer
		case ltac::Int: {
			auto i = static_cast<LtacInt *>(src);
			writer << i->val << std::endl;
		} break;
		
		//Return register- for function calls
		case ltac::RetReg: {
			writer << "eax" << std::endl;
		} break;
	}
}

//Builds integer division/modulus
void Asm_x64::build_idiv(LtacIMath *math) {
	auto dest = math->children[0];
	auto src = math->children[1];
	std::string loco = "";
	
	//The first value (dest) must be in eax
	switch (dest->type) {
		//Register
		case ltac::Reg: {
			auto reg = static_cast<LtacReg *>(dest);
			int pos = reg->pos - 1;
		
			writer << "\tmov eax, " << registers32[pos] << std::endl;
			loco = registers32[pos];
		} break;
		
		//Variable
		case ltac::Var: {
			auto var = static_cast<LtacVar *>(dest);
			
			writer << "\tmov eax, DWORD PTR [rbp-" << var->pos;
			writer << "]" << std::endl;
			loco = "DWORD PTR [rbp-" + std::to_string(var->pos) + "]";
		} break;
	}
	
	//Generate the division instructions
	writer << "\tcdq" << std::endl;
	writer << "\tidiv ";
	
	//Generate the source instruction
	switch (src->type) {
		//Register
		case ltac::Reg: {
			auto reg = static_cast<LtacReg *>(src);
			int pos = reg->pos - 1;
			
			writer << registers32[pos] << std::endl;
		} break;
		
		//Variable
		case ltac::Var: {
			auto var = static_cast<LtacVar *>(src);
			writer << "[rbp-" << var->pos << "]" << std::endl;
		} break;
	}
	
	//Store the result
	if (math->op == Operator::Mod)
		writer << "\tmov " << loco << ", edx" << std::endl;
	else
		writer << "\tmov " << loco << ", eax" << std::endl;
}

