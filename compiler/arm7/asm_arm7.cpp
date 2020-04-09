#include "asm_arm7.hh"

//Build the data section
void Asm_Arm7::build_data(LtacDataSec *data) {
	for (auto ln : data->children) {
		switch (ln->type) {
			case ltac::String: {
				auto str = static_cast<LtacString *>(ln);
				std::string name = "dstr_" + str->name;
				
				writer << "\t" << name << ": .ascii \"";
				writer << str->val << "\\000\"" << std::endl;
				
				std::string d_ln = str->name + ": .word " + name;
				data_labels.push_back(d_ln);
			} break;
		}
	}
}

//Build the code section
void Asm_Arm7::build_code(LtacCodeSec *code) {
	for (auto ln : code->children) {
		switch (ln->type) {
			case ltac::Func: build_func(ln); break;
			case ltac::Ret: build_ret(ln); break;
			
			case ltac::Var: build_var(ln); break;
		}
	}
	
	//Generate the data labels
	writer << std::endl;
	
	for (auto ln : data_labels) {
		writer << ln << std::endl;
	}
	
	writer << std::endl;
}

