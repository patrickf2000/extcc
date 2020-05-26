#include "asm_x64.hh"

//Generate for the integer comparison instruction
void X64::build_icmp(PasmNode *ln) {
	auto cmp = static_cast<ICmp *>(ln);
	
	//Var - var
	if (cmp->op1 == Operand::Var && cmp->op2 == Operand::Var) {
		writer << "\tmov eax, DWORD PTR [rbp-" << cmp->pos1 << "]" << std::endl;
		writer << "\tcmp eax, DWORD PTR [rbp-" << cmp->pos2 << "]" << std::endl;
	
	// Var - immediate
	} else if (cmp->op1 == Operand::Var && cmp->op2 == Operand::Const) {
		writer << "\tcmp DWORD PTR [rbp-" << cmp->pos1 << "], " << cmp->pos2;
	
	//TODO: Add rest
	}
	
	writer << std::endl;
}

//Generate the branch instruction
void X64::build_br(PasmNode *ln) {
	auto br = static_cast<Branch *>(ln);
	
	switch (br->jmp) {
		case JmpType::Eq: writer << "\tje "; break;
		case JmpType::Neq: writer << "\tjne "; break;
		case JmpType::Z: writer << "\tjz "; break;
		case JmpType::Nz: writer << "\tjnz "; break;
		case JmpType::G: writer << "\tjg "; break;
		case JmpType::Ge: writer << "\tjge "; break;
		case JmpType::L: writer << "\tjl "; break;
		case JmpType::Le: writer << "\tjle "; break;
		default: writer << "\tjmp ";
	}
	
	writer << br->label << std::endl;
}
