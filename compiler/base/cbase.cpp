#include <iostream>
#include <cstdlib>

#include "cbase.hh"

//The main loop to generate code for the text section
void CompilerBase::build_code(PasmFile *file) {
	code_init();

	for (auto ln : file->code) {
		switch (ln->type) {
			//Formatting
			case pasm::Space: build_space(); break;
			
			//Labels
			case pasm::Lbl: build_label(ln); break;
		
			//Functions
			case pasm::Func: build_func(ln); break;
			case pasm::ILdArg: build_ildarg(ln); break;
			case pasm::F32_LdArg: build_f32_ldarg(ln); break;
			case pasm::F64_LdArg: build_f64_ldarg(ln); break;
			case pasm::ILdRet: build_ildret(ln); break;
			case pasm::IStrRet: build_istret(ln); break;
			case pasm::Ret: build_ret(); break;
			case pasm::StrPushArg: build_str_pusharg(ln); break;
			case pasm::IPushArg: build_ipusharg(ln); break;
			case pasm::F32_PushArg: build_f32_pusharg(ln); break;
			case pasm::F64_PushArg: build_f64_pusharg(ln); break;
			case pasm::FuncCall: build_call(ln); break;
			
			//Store
			case pasm::IStoreC: build_istorec(ln); break;
			case pasm::F32_StoreC: build_f32_storec(ln); break;
			case pasm::F64_StoreC: build_f64_storec(ln); break;
			case pasm::MoveVV: build_move_vv(ln); break;
			case pasm::ILdr: build_ildr(ln); break;
			case pasm::Str: build_str(ln); break;
			
			//Math
			case pasm::IMathRI: build_imath_ri(ln); break;
			case pasm::IMathRV: build_imath_rv(ln); break;
			case pasm::IMathVI: build_imath_vi(ln); break;
			
			//Flow
			case pasm::ICmp: build_icmp(ln); break;
			case pasm::Br: build_br(ln); break;
			
			//Syscall arguments
			case pasm::ISysarg: build_isysarg(ln); break;
			case pasm::StrSysarg: build_str_sysarg(ln); break;
			
			//Syscalls
			case pasm::Syscall: build_syscall(ln); break;
		}
	}
}

//Throws a fatal error
void CompilerBase::fatalError(std::string msg) {
	std::cout << "[Fatal] " << msg << std::endl;
	std::exit(1);
}

//Displays a warning
void CompilerBase::warning(std::string msg) {
	std::cout << "[Warning] " << msg << std::endl;
}
