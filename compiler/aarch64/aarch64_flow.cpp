#include "asm_aarch64.hh"

//Generate for the integer comparison instruction
void AArch64::build_icmp(PasmNode *ln) {
	auto cmp = static_cast<ICmp *>(ln);
}

//Generate the branch instruction
void AArch64::build_br(PasmNode *ln) {
	auto br = static_cast<Branch *>(ln);
	
	switch (br->jmp) {
		case JmpType::Eq: break;
		case JmpType::Neq: break;
		case JmpType::Z: break;
		case JmpType::Nz: break;
		case JmpType::G: break;
		case JmpType::Ge: break;
		case JmpType::L: break;
		case JmpType::Le: break;
		default: break;
	}
	
	writer << br->label << std::endl;
}
