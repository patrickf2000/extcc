#include <iostream>
#include <cstdlib>

#include "asm_x64.hh"

//Opens the file
X64::X64(std::string name) {
	writer = std::ofstream(name);
	
	writer << ".intel_syntax noprefix" << std::endl;
}

//The main loop to generate code for the data section
void X64::build_data(PasmFile *file) {
	writer << ".data" << std::endl;
	
	for (auto ln : file->data) {
		auto str = static_cast<PasmString *>(ln);
		std::string type = "";
		std::string dl = "";
		
		switch (ln->type) {
			//Strings
			case pasm::String: type = ".string"; dl = "\""; break;
			
			//Floats
			case pasm::Float: type = ".long"; break;
			
			//Doubles
			case pasm::Double: type = ".quad"; break;
		}
		
		writer << "\t" << str->name << ": " << type << " " << dl << str->val;
		writer << dl << std::endl;
	}
	
	writer << std::endl;
}

//The main loop to generate code for the text section
void X64::build_code(PasmFile *file) {
	writer << ".text" << std::endl;
	writer << std::endl;

	for (auto ln : file->code) {
		switch (ln->type) {
			//Formatting
			case pasm::Space: writer << std::endl; break;
			
			//Labels
			case pasm::Lbl: {
				auto lbl = static_cast<Label *>(ln);
				writer << lbl->name << ":" << std::endl;
			} break;
		
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

//Close out the file
void X64::write() {
	writer.close();
}

//Throws a fatal error
void X64::fatalError(std::string msg) {
	std::cout << "[Fatal] " << msg << std::endl;
	std::exit(1);
}

//Displays a warning
void X64::warning(std::string msg) {
	std::cout << "[Warning] " << msg << std::endl;
}

