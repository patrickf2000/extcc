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

//Store a float-64 constant to a memory location
void X64::build_f64_storec(PasmNode *ln) {
	auto store = static_cast<F64_StoreConst *>(ln);
	
	writer << "\tmovsd xmm1, QWORD PTR " << store->name << "[rip]" << std::endl;
	
	writer << "\tmovsd QWORD PTR [rbp-" << store->pos << "], ";
	writer << "xmm1" << std::endl;
}

//Store a string constant to a memory location
void X64::build_str_storec(PasmNode *ln) {
	auto store = static_cast<Str_StoreConst *>(ln);
	
	writer << "\tmov QWORD PTR [rbp-" << store->pos << "], ";
	writer << "OFFSET FLAT:" << store->name << std::endl;
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
		
		//Float-64
		case DType::Float64: {
			writer << "\tmovsd xmm1, QWORD PTR " << v2 << std::endl;
			writer << "\tmovsd QWORD PTR " << v1 << ", xmm1" << std::endl;
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

//Load value from pointer
void X64::build_ldptr(PasmNode *ln) {
	auto load = static_cast<PtrLd *>(ln);
	int ptrPos = load->ptrPos;
	int pos = load->pos;
	int size = load->size;
	
	writer << "\tmov rax, QWORD PTR [rbp-" << ptrPos << "]" << std::endl;
	
	switch (load->posType) {
		case Operand::Var: /*TODO*/ break;
		
		case Operand::Const: {
			pos = pos * size;
			writer << "\tmov eax, DWORD PTR [rax+" << pos << "]" << std::endl;
		} break;
	}
}

//Store value to integer pointer
void X64::build_iptr_str(PasmNode *ln) {
	auto store = static_cast<IPtrStr *>(ln);
	int pos = store->pos;
	int src = store->src;
	
	switch (store->posType) {
		case Operand::Var: {
			writer << "\tmov eax, DWORD PTR [rbp-" << pos << "]" << std::endl;
			writer << "\tcdqe" << std::endl;
			writer << "\tlea rdx, [0+rax*4]" << std::endl;
			writer << "\tmov rax, QWORD PTR [rbp-" << store->ptrPos << "]" << std::endl;
			writer << "\tadd rax, rdx" << std::endl;
		} break;
		
		case Operand::Const: {
			pos = pos * store->size;
			
			writer << "\tmov rax, QWORD PTR [rbp-" << store->ptrPos << "]" << std::endl;
			writer << "\tadd rax, " << pos << std::endl;
		} break;
	}
	
	switch (store->opType) {
		case Operand::Var: {
			writer << "\tmov edx, DWORD PTR [rbp-" << store->src << "]" << std::endl;
			writer << "\tmov DWORD PTR [rax], edx" << std::endl;
		} break;
		
		case Operand::Reg: {
			auto reg = registers32[store->src];
			writer << "\tmov DWORD PTR [rax], " << reg << std::endl;
		} break;
		
		case Operand::Const: {
			writer << "\tmov DWORD PTR [rax], " << store->src << std::endl;
		} break;
	}
}


