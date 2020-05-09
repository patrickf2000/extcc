#include <iostream>

#include "asm_x64.hh"

std::string var_regs[] = {
	"r8d",
	"r9d",
	"r10d",
	"r11d",
	"r12d",
	"r13d",
	"r14d",
	"r15d"
};

//Build the data section
void Asm_x64::build_data(LtacDataSec *data) {
	for (auto ln : data->children) {
		switch (ln->type) {
			//Floats
			case ltac::Float: {
				auto lf = static_cast<LtacFloat *>(ln);
				writer << "\t" << lf->name << ": .long ";
				writer << std::to_string(lf->i_val) << std::endl;
			} break;
			
			//Doubles
			case ltac::Double: {
				auto ld = static_cast<LtacDouble *>(ln);
				writer << "\t" << ld->name << ": .quad ";
				writer << ld->i_val << std::endl;
			} break;
		
			//Strings
			case ltac::String: {
				auto lstr = static_cast<LtacString *>(ln);
				writer << "\t" << lstr->name << ": .string \"";
				writer << lstr->val << "\"\n";
			} break;
		}
	}
}

//Build the code section
void Asm_x64::build_code(LtacCodeSec *code) {
	for (auto ln : code->children) {
		switch (ln->type) {
			case ltac::Label: {
				auto label = static_cast<LtacLabel *>(ln);
				writer << label->name << ":" << std::endl;
			} break;
			
			case ltac::Func: build_func(ln); break;
			case ltac::PushArg: build_push_arg(ln); break;
			case ltac::FuncCall: build_func_call(ln); break;
			case ltac::Ret: build_ret(ln); break;
			
			case ltac::Array: build_array(ln); break;
			case ltac::ArraySet: build_array_set(ln); break;
			
			case ltac::Reg: build_reg(ln); break;
			case ltac::Var: build_var(ln); break;
			
			case ltac::ICmp: build_icmp(ln); break;
			case ltac::Jmp: build_jmp(ln); break;
			
			case ltac::IMath: build_imath(ln); break;
			case ltac::F32Math: build_f32math(ln); break;
			
			case ltac::VLoad: build_vload(ln); break;
			case ltac::VStore: build_vstore(ln); break;
			case ltac::VIMath: build_vimath(ln); break;
		}
	}
}


