#include "asm_x64.hh"
#include "x64_registers.hh"

//Call registers
//64-bit
std::string call_regs[] = {
	"rdi",
	"rsi",
	"rdx",
	"rcx",
	"r8",
	"r9"
};

//32-bit
std::string call_regs32[] = {
	"edi",
	"esi",
	"edx",
	"ecx",
	"r8d",
	"r9d"
};

//Floating-point
std::string call_regs_flt[] = {
	"xmm0",
	"xmm1",
	"xmm2",
	"xmm3",
	"xmm4",
	"xmm5",
	"xmm6",
	"xmm7"
};

//The position for calling args
int call_pos = 0;
int flt_call_pos = 0;

//The position for loading args
int arg_pos = 0;
int flt_arg_pos = 0;

//Builds a function declaration
void X64::build_func(PasmNode *ln) {
	auto func = static_cast<Func *>(ln);
	
	writer << std::endl;
	writer << ".globl " << func->name << std::endl;
	
	writer << std::endl;
	writer << func->name << ":" << std::endl;
	
	//Set up the stack
	writer << "\tpush rbp" << std::endl;
	writer << "\tmov rbp, rsp" << std::endl;
	
	if (func->stackSize > 0)
		writer << "\tsub rsp, " << func->stackSize << std::endl;
		
	writer << std::endl;
	
	arg_pos = 0;
	flt_arg_pos = 0;
}

//Loads an byte argument parameter
void X64::build_bldarg(PasmNode *ln) {
	auto ldarg = static_cast<BLdArg *>(ln);
	
	writer << "\tmov eax, " << call_regs32[arg_pos] << std::endl;
	writer << "\tmov BYTE PTR [rbp-" << ldarg->pos << "], al" << std::endl;;
	
	++arg_pos;
}

//Loads an integer argument parameter
void X64::build_ildarg(PasmNode *ln) {
	auto ldarg = static_cast<ILdArg *>(ln);
	
	writer << "\tmov DWORD PTR [rbp-" << ldarg->pos << "], ";
	writer << call_regs32[arg_pos] << std::endl;
	
	++arg_pos;
}

//Loads a float-32 argument parameter
void X64::build_f32_ldarg(PasmNode *ln) {
	auto ldarg = static_cast<F32_LdArg *>(ln);
	
	writer << "\tmovss DWORD PTR [rbp-" << ldarg->pos << "], ";
	writer << call_regs_flt[flt_arg_pos] << std::endl;
	
	++flt_arg_pos;
}

//Loads a float-64 argument parameter
void X64::build_f64_ldarg(PasmNode *ln) {
	auto ldarg = static_cast<F64_LdArg *>(ln);
	
	writer << "\tmovsd QWORD PTR [rbp-" << ldarg->pos << "], ";
	writer << call_regs_flt[flt_arg_pos] << std::endl;
	
	++flt_arg_pos;
}

//Loads a pointer argument parameter
void X64::build_ptr_ldarg(PasmNode *ln) {
	auto ldarg = static_cast<Ptr_LdArg *>(ln);
	
	writer << "\tmov QWORD PTR [rbp-" << ldarg->pos << "], ";
	writer << call_regs[arg_pos] << std::endl;
	
	++arg_pos;
}

//Load an integer value to a return register
void X64::build_ildret(PasmNode *ln) {
	auto store = static_cast<ILdRet *>(ln);
	int val = store->val;
	
	switch (store->opType) {
		case Operand::Var: {
			writer << "\tmov eax, DWORD PTR [rbp-" << val << "]" << std::endl;
		} break;
		
		case Operand::Reg: {
			auto reg = registers32[val];
			writer << "\tmov eax, " << reg << std::endl;
		} break;
		
		case Operand::Const: {
			writer << "\tmov eax, " << val << std::endl;
		} break;
		
		default: fatalError("Unknown command.");
	}
}

//Store a return value to integer
void X64::build_istret(PasmNode *ln) {
	auto store = static_cast<IStrRet *>(ln);
	int val = store->val;
	
	switch (store->opType) {
		case Operand::Var: {
			writer << "\tmov DWORD PTR [rbp-" << val << "], eax" << std::endl;
		} break;
		
		case Operand::Reg: {
			writer << "\tmov " << registers32[val] << ", eax" << std::endl;
		} break;
		
		default: fatalError("Unknown i.stret instruction.");
	}
}

//Store a return value to pointer
void X64::build_ptr_stret(PasmNode *ln) {
	auto store = static_cast<Ptr_StrRet *>(ln);
	int val = store->val;
	
	switch (store->opType) {
		case Operand::Var: {
			writer << "\tmov QWORD PTR [rbp-" << val << "], rax" << std::endl;
		} break;
		
		case Operand::Reg: {
			writer << "\tmov " << registers[val] << ", rax" << std::endl;
		} break;
		
		default: fatalError("Unknown ptr.stret instruction.");
	}
}

//Store pointer result to integer
void X64::build_istrptr(PasmNode *ln) {
	auto store = static_cast<IStrPtr *>(ln);
	int val = store->val;
	
	switch (store->opType) {
		case Operand::Var: {
			writer << "\tmov DWORD PTR [rbp-" << val << "], eax" << std::endl;
		} break;
		
		case Operand::Reg: {
			auto reg = registers32[val];
			writer << "\tmov " << reg << ", eax" << std::endl;
		} break;
		
		default: fatalError("Unknown i.strptr instruction.");
	}
}

//On x86, this is the simplest command :)
void X64::build_ret() {
	writer << "\tleave" << std::endl;
	writer << "\tret" << std::endl;
	writer << std::endl;
}

//Push string argument
void X64::build_str_pusharg(PasmNode *ln) {
	auto pusharg = static_cast<StrPushArg *>(ln);
		
	auto reg = call_regs[call_pos];
	++call_pos;
	
	writer << "\tmov " << reg << ", OFFSET FLAT:" << pusharg->name;
	writer << std::endl;
}

//Push byte argument
void X64::build_bpusharg(PasmNode *ln) {
	auto pusharg = static_cast<BPushArg *>(ln);
	int val = pusharg->val;
		
	auto reg = call_regs32[call_pos];
	++call_pos;
	
	switch (pusharg->opType) {
		case Operand::Var: {
			writer << "\tmovsx " << reg << ", BYTE PTR [rbp-";
			writer << val << "]" << std::endl;
		} break;
		
		case Operand::Reg: {
			warning("b.pusharg_r not yet supported.");
		} break;
		
		case Operand::Const: {
			writer << "\tmov " << reg << ", " << val << std::endl;
		} break;
		
		default: fatalError("Unknown command.");
	}
}

//Push integer argument
void X64::build_ipusharg(PasmNode *ln) {
	auto pusharg = static_cast<IPushArg *>(ln);
	int val = pusharg->val;
		
	auto reg = call_regs32[call_pos];
	++call_pos;
	
	switch (pusharg->opType) {
		case Operand::Var: {
			writer << "\tmov " << reg << ", DWORD PTR [rbp-";
			writer << val << "]" << std::endl;
		} break;
		
		case Operand::Reg: {
			if (val == -1 || val == -2) {
				writer << "\tmov " << reg << ", eax" << std::endl;
			} else {
				writer << "\tmov " << reg << ", " << registers32[val] << std::endl;
			}
		} break;
		
		case Operand::Const: {
			writer << "\tmov " << reg << ", " << val << std::endl;
		} break;
		
		default: fatalError("Unknown command.");
	}
}

//Push float-32 argument
void X64::build_f32_pusharg(PasmNode *ln) {
	auto pusharg = static_cast<F32_PushArg *>(ln);
	
	auto reg = call_regs_flt[flt_call_pos];
	++flt_call_pos;
	
	switch (pusharg->opType) {
		case Operand::Var: {
			if (pusharg->promote)
				writer << "\tcvtss2sd ";
			else
				writer << "\tmovss ";
				
			writer << reg << ", DWORD PTR [rbp-";
			writer << pusharg->pos << "]" << std::endl;
		} break;
		
		case Operand::Reg: {
			warning("f32.pusharg_r not supported yet.");
		} break;
		
		case Operand::Const: {
			if (pusharg->promote)
				writer << "\tcvtss2sd ";
			else
				writer << "\tmovss ";
				
			writer << reg << ", " << pusharg->val << "[rip]" << std::endl;
		} break;
		
		default: fatalError("Unknown command.");
	}
}

//Push float-64 argument
void X64::build_f64_pusharg(PasmNode *ln) {
	auto pusharg = static_cast<F64_PushArg *>(ln);
	
	auto reg = call_regs_flt[flt_call_pos];
	++flt_call_pos;
	
	switch (pusharg->opType) {
		case Operand::Var: {
			writer << "\tmovsd " << reg << ", QWORD PTR [rbp-";
			writer << pusharg->pos << "]" << std::endl;
		} break;
		
		case Operand::Reg: {
			warning("f64.pusharg_r not supported yet.");
		} break;
		
		case Operand::Const: {
			writer << "\tmovsd " << reg << ", " << pusharg->val << "[rip]" << std::endl;
		} break;
		
		default: fatalError("Unknown command.");
	}
}

//Push pointer argument
void X64::build_ptr_pusharg(PasmNode *ln) {
	auto pusharg = static_cast<Ptr_PushArg *>(ln);
	
	auto reg = call_regs[call_pos];
	++call_pos;
	
	writer << "\tmov " << reg << ", QWORD PTR [rbp-" << pusharg->pos << "]" << std::endl;
}

//Call a function
void X64::build_call(PasmNode *ln) {
	auto fc = static_cast<FuncCall *>(ln);
	writer << "\tcall " << fc->name << std::endl;

	call_pos = 0;
	flt_call_pos = 0;
}

	
