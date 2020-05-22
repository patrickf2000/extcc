#include "asm_i386.hh"

//Build the data section
void Asm_i386::build_data(LtacDataSec *data) {
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
void Asm_i386::build_code(LtacCodeSec *code) {
	for (auto ln : code->children) {
		switch (ln->type) {
			case ltac::Func: build_func(ln); break;
		}
	}
}

//Builds a function declaration
void Asm_i386::build_func(LtacNode *node) {
	writer << ".global main" << std::endl;
	writer << "main:" << std::endl;
}
