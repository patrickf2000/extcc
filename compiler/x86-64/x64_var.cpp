#include "asm_x64.hh"
#include "x64_registers.hh"

//Store an integer constant to a memory location
void X64::build_istorec(PasmNode *ln) {
	auto store = static_cast<IStoreConst *>(ln);
	
	writer << "\tmov DWORD PTR [rbp-" << store->pos << "], ";
	writer << store->val << std::endl;
}

//Store a float constant to a memory location
void X64::build_f32_storec(PasmNode *ln) {
	auto store = static_cast<F32_StoreConst *>(ln);
	
	writer << "\tmovss xmm1, DWORD PTR " << store->name << "[rip]" << std::endl;
	
	writer << "\tmovss DWORD PTR [rbp-" << store->pos << "], ";
	writer << "xmm1" << std::endl;
}

//Move one variable to another
void X64::build_move_vv(PasmNode *ln) {
	auto move = static_cast<MoveVV *>(ln);
	
	std::string v1 = "[rbp-" + std::to_string(move->pos1) + "]";
	std::string v2 = "[rbp-" + std::to_string(move->pos2) + "]";
	
	switch (move->dType) {
		//Integers
		case DType::Int: {
			writer << "\tmov eax, DWORD PTR " << v2 << std::endl;
			writer << "\tmov DWORD PTR " << v1 << ", eax" << std::endl;
		} break;
		
		//Float-32
		case DType::Float32: {
			writer << "\tmovss xmm1, DWORD PTR " << v2 << std::endl;
			writer << "\tmovss DWORD PTR " << v1 << ", xmm1" << std::endl;
		} break;
		
		//TODO: Add rest
	}
	
	writer << std::endl;
}

//Loads an integer register
void X64::build_ildr(PasmNode *ln) {
	auto load = static_cast<ILdr *>(ln);
	auto reg = registers32[load->reg];
	
	writer << "\tmov " << reg << ", ";
	
	switch (load->opType) {
		case Operand::Var: writer << "DWORD PTR [rbp-" << load->pos << "]"; break;
		case Operand::Const: writer << load->pos; break;
	}
	
	writer << std::endl;
}

//Store register to variable
void X64::build_str(PasmNode *ln) {
	auto str = static_cast<Str *>(ln);
	int reg = str->reg;
	std::string var = "[rbp-" + std::to_string(str->pos) + "]";
	
	switch (str->dType) {
		//Integers
		case DType::Int: {
			writer << "\tmov DWORD PTR " << var << ", " << registers32[reg];
			writer << std::endl;
		} break;
		
		//TODO: Add rest
	}
}

