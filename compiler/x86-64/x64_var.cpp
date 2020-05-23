#include "asm_x64.hh"

//Operation registers
std::string registers[] = {
	"rax",
	"rbx",
	"rcx",
	"r10",
	"r11",
	"r12",
	"r13",
	"r14",
	"r15"
};

std::string registers32[] = {
	"eax",
	"ebx",
	"ecx",
	"r10d",
	"r11d",
	"r12d",
	"r13d",
	"r14d",
	"r15d"
};

//Store an integer constant to a memory location
void X64::build_istorec(PasmNode *ln) {
	auto store = static_cast<IStoreConst *>(ln);
	
	writer << "\tmov DWORD PTR [rbp-" << store->pos << "], ";
	writer << store->val << std::endl;
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
		
		//TODO: Add rest
	}
	
	writer << std::endl;
}

//Load register to variable
void X64::build_ldrv(PasmNode *ln) {
	auto ldrv = static_cast<LdrV *>(ln);
	int reg = ldrv->reg;
	std::string var = "[rbp-" + std::to_string(ldrv->pos) + "]";
	
	switch (ldrv->dType) {
		//Integers
		case DType::Int: {
			writer << "\tmov " << registers32[reg] << ", DWORD PTR ";
			writer << var << std::endl;
		} break;
		
		//TODO: Add rest
	}
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

