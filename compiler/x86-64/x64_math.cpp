#include "asm_x64.hh"
#include "x64_registers.hh"

//Integer math- register <-> immediate
void X64::build_imath_ri(PasmNode *ln) {
	auto math = static_cast<IMathRI *>(ln);
	auto reg = registers32[math->reg];
	bool is_div = false;
	
	switch (math->mType) {
		case MathType::Add: writer << "\tadd "; break;
		case MathType::Sub: writer << "\tsub "; break;
		case MathType::Mul: writer << "\timul "; break;
		case MathType::Div: is_div = true; break;
	}
	
	if (is_div) {
		writer << "\tcdq" << std::endl;
		writer << "\tmov eax, " << reg << std::endl;
		writer << "\tmov " << reg << ", " << math->val << std::endl;
		writer << "\tidiv " << reg << std::endl;
		writer << "\tmov " << reg << ", eax" << std::endl;
	} else {
		writer << reg << ", " << math->val << std::endl;
	}
}
