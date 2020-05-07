#include "asm_llvm.hh"

//Build the data section
void Asm_LLVM::build_data(LtacDataSec *data) {
	for (auto ln : data->children) {
		switch (ln->type) {
			//Floats
			case ltac::Float: {
				auto lf = static_cast<LtacFloat *>(ln);

			} break;
			
			//Doubles
			case ltac::Double: {
				auto ld = static_cast<LtacDouble *>(ln);

			} break;
		
			//Strings
			case ltac::String: {
				auto lstr = static_cast<LtacString *>(ln);

			} break;
		}
	}
}

//Build the code section
void Asm_LLVM::build_code(LtacCodeSec *code) {
	for (auto ln : code->children) {
		switch (ln->type) {
			case ltac::Func: build_func(ln); break;
			case ltac::Ret: build_ret(ln); break;
		}
	}
}

//Builds a function declaration
void Asm_LLVM::build_func(LtacNode *node) {
	auto func = static_cast<LtacFunc *>(node);
	
	writer << "define i32 @" << func->name << "() {";
	writer << std::endl;
}

//Builds a function return
void Asm_LLVM::build_ret(LtacNode *node) {
	writer << "\tret i32 0" << std::endl;
	writer << "}" << std::endl;
}
