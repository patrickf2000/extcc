#include "asm_aarch64.hh"
#include "aarch64_registers.hh"

namespace ARM64 {

//Call registers
//64-bit
std::string call_regs[] = {
	""
	/*"rdi",
	"rsi",
	"rdx",
	"rcx",
	"r8",
	"r9"*/
};

//32-bit
std::string call_regs32[] = {
	""
	/*"edi",
	"esi",
	"edx",
	"ecx",
	"r8d",
	"r9d"*/
};

//Floating-point
std::string call_regs_flt[] = {
	""
	/*"xmm0",
	"xmm1",
	"xmm2",
	"xmm3",
	"xmm4",
	"xmm5",
	"xmm6",
	"xmm7"*/
};

//The position for calling args
int call_pos = 0;
int flt_call_pos = 0;

//The position for loading args
int arg_pos = 0;
int flt_arg_pos = 0;

}

using namespace ARM64;

//Builds a function declaration
void AArch64::build_func(PasmNode *ln) {
	auto func = static_cast<Func *>(ln);
	
	writer << std::endl;
	writer << ".globl " << func->name << std::endl;
	
	writer << std::endl;
	writer << func->name << ":" << std::endl;
	
	//Set up the stack
	
	if (func->stackSize > 0)
		//writer << "\tsub rsp, " << func->stackSize << std::endl;
		
	writer << std::endl;
	
	arg_pos = 0;
	flt_arg_pos = 0;
}

//Loads an byte argument parameter
void AArch64::build_bldarg(PasmNode *ln) {
	auto ldarg = static_cast<BLdArg *>(ln);
	++arg_pos;
}

//Loads an integer argument parameter
void AArch64::build_ildarg(PasmNode *ln) {
	auto ldarg = static_cast<ILdArg *>(ln);
	++arg_pos;
}

//Loads a float-32 argument parameter
void AArch64::build_f32_ldarg(PasmNode *ln) {
	auto ldarg = static_cast<F32_LdArg *>(ln);
	++flt_arg_pos;
}

//Loads a float-64 argument parameter
void AArch64::build_f64_ldarg(PasmNode *ln) {
	auto ldarg = static_cast<F64_LdArg *>(ln);
	++flt_arg_pos;
}

//Loads a pointer argument parameter
void AArch64::build_ptr_ldarg(PasmNode *ln) {
	auto ldarg = static_cast<Ptr_LdArg *>(ln);
	++arg_pos;
}

//Load an integer value to a return register
void AArch64::build_ildret(PasmNode *ln) {
	auto store = static_cast<ILdRet *>(ln);
	int val = store->val;
	
	switch (store->opType) {
		case Operand::Var: {
		} break;
		
		case Operand::Reg: {
		} break;
		
		case Operand::Const: {
		} break;
		
		default: fatalError("Unknown command.");
	}
}

//Store a return value to integer
void AArch64::build_istret(PasmNode *ln) {
	auto store = static_cast<IStrRet *>(ln);
	int val = store->val;
	
	switch (store->opType) {
		case Operand::Var: {
		} break;
		
		case Operand::Reg: {
		} break;
		
		default: fatalError("Unknown i.stret instruction.");
	}
}

//Store a return value to pointer
void AArch64::build_ptr_stret(PasmNode *ln) {
	auto store = static_cast<Ptr_StrRet *>(ln);
	int val = store->val;
	
	switch (store->opType) {
		case Operand::Var: {
		} break;
		
		case Operand::Reg: {
		} break;
		
		default: fatalError("Unknown ptr.stret instruction.");
	}
}

//Store pointer result to integer
void AArch64::build_istrptr(PasmNode *ln) {
	auto store = static_cast<IStrPtr *>(ln);
	int val = store->val;
	
	switch (store->opType) {
		case Operand::Var: {
		} break;
		
		case Operand::Reg: {
		} break;
		
		default: fatalError("Unknown i.strptr instruction.");
	}
}

//Return from function
void AArch64::build_ret() {
}

//Push string argument
void AArch64::build_str_pusharg(PasmNode *ln) {
	auto pusharg = static_cast<StrPushArg *>(ln);
		
	auto reg = call_regs[call_pos];
	++call_pos;
}

//Push byte argument
void AArch64::build_bpusharg(PasmNode *ln) {
	auto pusharg = static_cast<BPushArg *>(ln);
	int val = pusharg->val;
		
	auto reg = call_regs32[call_pos];
	++call_pos;
	
	switch (pusharg->opType) {
		case Operand::Var: {
		} break;
		
		case Operand::Reg: {
			warning("b.pusharg_r not yet supported.");
		} break;
		
		case Operand::Const: {
		} break;
		
		default: fatalError("Unknown command.");
	}
}

//Push integer argument
void AArch64::build_ipusharg(PasmNode *ln) {
	auto pusharg = static_cast<IPushArg *>(ln);
	int val = pusharg->val;
		
	auto reg = call_regs32[call_pos];
	++call_pos;
	
	switch (pusharg->opType) {
		case Operand::Var: {
		} break;
		
		case Operand::Reg: {
		} break;
		
		case Operand::Const: {
		} break;
		
		default: fatalError("Unknown command.");
	}
}

//Push float-32 argument
void AArch64::build_f32_pusharg(PasmNode *ln) {
	auto pusharg = static_cast<F32_PushArg *>(ln);
	
	auto reg = call_regs_flt[flt_call_pos];
	++flt_call_pos;
	
	switch (pusharg->opType) {
		case Operand::Var: {
		} break;
		
		case Operand::Reg: {
		} break;
		
		case Operand::Const: {
		} break;
		
		default: fatalError("Unknown command.");
	}
}

//Push float-64 argument
void AArch64::build_f64_pusharg(PasmNode *ln) {
	auto pusharg = static_cast<F64_PushArg *>(ln);
	
	auto reg = call_regs_flt[flt_call_pos];
	++flt_call_pos;
	
	switch (pusharg->opType) {
		case Operand::Var: {
		} break;
		
		case Operand::Reg: {
		} break;
		
		case Operand::Const: {
		} break;
		
		default: fatalError("Unknown command.");
	}
}

//Push pointer argument
void AArch64::build_ptr_pusharg(PasmNode *ln) {
	auto pusharg = static_cast<Ptr_PushArg *>(ln);
	
	auto reg = call_regs[call_pos];
	++call_pos;
}

//Call a function
void AArch64::build_call(PasmNode *ln) {
	auto fc = static_cast<FuncCall *>(ln);
	writer << "\tcall " << fc->name << std::endl;

	call_pos = 0;
	flt_call_pos = 0;
}

	
