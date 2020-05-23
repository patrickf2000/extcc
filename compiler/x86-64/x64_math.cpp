#include "asm_x64.hh"
#include "x64_registers.hh"

//Integer math- register <-> immediate
void X64::build_imath_ri(PasmNode *ln) {
	auto math = static_cast<IMathRI *>(ln);
	auto reg = registers32[math->reg];
	
	switch (math->mType) {
		case MathType::Add: writer << "\tadd "; break;
		case MathType::Sub: writer << "\tsub "; break;
		case MathType::Mul: writer << "\timul "; break;
		case MathType::Div: writer << "\tidiv "; break;
	}
	
	writer << reg << ", " << math->val << std::endl;
}
