#include "asm_x64.hh"
#include "x64_registers.hh"

//Integer math- register <- immediate
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

//Integer math- register <- variable
void X64::build_imath_rv(PasmNode *ln) {
	auto math = static_cast<IMathRV *>(ln);
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
		
		writer << "\tmov " << reg << ", DWORD PTR [rbp-";
		writer << math->pos << "]" << std::endl;
		
		writer << "\tidiv " << reg << std::endl;
		writer << "\tmov " << reg << ", eax" << std::endl;
	} else {
		writer << reg << ", DWORD PTR [rbp-" << math->pos << "]" << std::endl;
	}
}

//Integer math- variable <- immediate
void X64::build_imath_vi(PasmNode *ln) {
	auto math = static_cast<IMathVI *>(ln);
	bool is_div = false;
	bool single = false;
	
	switch (math->mType) {
		case MathType::Add: {
			if (math->val == 1) {
				single = true;
				writer << "\tinc ";
			} else {
				writer << "\tadd ";
			}
		} break;
		
		case MathType::Sub: {
			if (math->val == 1) {
				single = true;
				writer << "\tdec ";
			} else {
				writer << "\tsub ";
			}
		} break;
		
		case MathType::Mul: writer << "\timul "; break;
		case MathType::Div: is_div = true; break;
	}
	
	if (is_div) {
		writer << "\tcdq" << std::endl;
		writer << "\tmov eax, DWORD PTR [rbp-" << math->pos << "]" << std::endl;
		
		writer << "\tmov r9d, " << math->val << std::endl;
		
		writer << "\tidiv r9d" << std::endl;
		writer << "\tmov DWORD PTR [rbp-" << math->pos << "], eax" << std::endl;
	} else {
		writer << "DWORD PTR [rbp-" << math->pos << "]";
		
		if (single) {
			writer << std::endl;
		} else {
			writer << ", " << math->val << std::endl;
		}
	}
}


