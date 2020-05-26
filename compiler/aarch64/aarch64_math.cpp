#include "asm_aarch64.hh"
#include "aarch64_registers.hh"

//Integer math- register <- immediate
void AArch64::build_imath_ri(PasmNode *ln) {
	auto math = static_cast<IMathRI *>(ln);
	//auto reg = registers32[math->reg];
	bool is_div = false;
	
	switch (math->mType) {
		case MathType::Add: break;
		case MathType::Sub: break;
		case MathType::Mul: break;
		case MathType::Div: break;
	}
}

//Integer math- register <- variable
void AArch64::build_imath_rv(PasmNode *ln) {
	auto math = static_cast<IMathRV *>(ln);
	//auto reg = registers32[math->reg];
	bool is_div = false;
	
	switch (math->mType) {
		case MathType::Add: break;
		case MathType::Sub: break;
		case MathType::Mul: break;
		case MathType::Div: break;
	}
}

//Integer math- register <- register
// -1- Return register (eax)
// -2- Pointer register (eax)
void AArch64::build_imath_rr(PasmNode *ln) {
	auto math = static_cast<IMathRR *>(ln);
	//auto reg = registers32[math->reg];
	bool is_div = false;
	
	switch (math->mType) {
		case MathType::Add: break;
		case MathType::Sub: break;
		case MathType::Mul: break;
		case MathType::Div: break;
	}
}

//Integer math- variable <- immediate
void AArch64::build_imath_vi(PasmNode *ln) {
	auto math = static_cast<IMathVI *>(ln);
	//auto reg = registers32[math->reg];
	bool is_div = false;
	
	switch (math->mType) {
		case MathType::Add: break;
		case MathType::Sub: break;
		case MathType::Mul: break;
		case MathType::Div: break;
	}
}


