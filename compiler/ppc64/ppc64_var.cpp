#include "asm_ppc64.hh"
#include "ppc64_registers.hh"

//Store a byte constant to a memory location
void PPC64::build_bstorec(PasmNode *ln) {
	auto store = static_cast<BStoreConst *>(ln);
}

//Store an integer constant to a memory location
void PPC64::build_istorec(PasmNode *ln) {
	auto store = static_cast<IStoreConst *>(ln);
}

//Store a float constant to a memory location
void PPC64::build_f32_storec(PasmNode *ln) {
	auto store = static_cast<F32_StoreConst *>(ln);
}

//Store a float-64 constant to a memory location
void PPC64::build_f64_storec(PasmNode *ln) {
	auto store = static_cast<F64_StoreConst *>(ln);
}

//Store a string constant to a memory location
void PPC64::build_str_storec(PasmNode *ln) {
	auto store = static_cast<Str_StoreConst *>(ln);
}

//Move one variable to another
void PPC64::build_move_vv(PasmNode *ln) {
	auto move = static_cast<MoveVV *>(ln);
	
	switch (move->dType) {
		//Integers
		case DType::Int: {
		} break;
		
		//Float-32
		case DType::Float32: {
		} break;
		
		//Float-64
		case DType::Float64: {
		} break;
		
		//TODO: Add rest
	}
}

//Loads an integer register
void PPC64::build_ildr(PasmNode *ln) {
	auto load = static_cast<ILdr *>(ln);
	
	switch (load->opType) {
		case Operand::Var: break;
		case Operand::Const: break;
	}
}

//Store register to variable
void PPC64::build_str(PasmNode *ln) {
	auto str = static_cast<Str *>(ln);
	int reg = str->reg;
	
	switch (str->dType) {
		//Integers
		case DType::Int: {
		} break;
		
		//TODO: Add rest
	}
}

//Load value from pointer
void PPC64::build_ldptr(PasmNode *ln) {
	auto load = static_cast<PtrLd *>(ln);
	int ptrPos = load->ptrPos;
	int pos = load->pos;
	int size = load->size;
	
	switch (load->posType) {
		case Operand::Var: {
		} break;
		
		case Operand::Const: {
			pos = pos * size;
		} break;
	}
}

//Store value to a pointer
void PPC64::build_ptr_str(PasmNode *ln) {
	auto store = static_cast<PtrStr *>(ln);
	int pos = store->pos;
	int src = store->src;
	
	switch (store->posType) {
		case Operand::Var: {
		} break;
		
		case Operand::Const: {
			pos = pos * store->size;
		} break;
	}
	
	switch (store->opType) {
		case Operand::Var: {
		} break;
		
		case Operand::Reg: {
		} break;
		
		case Operand::Const: {;
		} break;
	}
}


