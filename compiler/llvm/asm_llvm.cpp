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
			
			case ltac::Var: build_var(ln); break;
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

//Builds a variable
void Asm_LLVM::build_var(LtacNode *node) {
	auto var = static_cast<LtacVar *>(node);
	int pos = var->pos;
	int llvm_pos = vars[pos];
	std::string type = "";
	
	//Determine the type
	switch (var->d_type) {
		case DataType::Int: type = "i32"; break;
		
		//TODO: Add rest
	}
	
	//Declare the variable and allocate space
	if (llvm_pos <= 0) {
		vars[llvm_reg_pos] = pos;
		llvm_pos = llvm_reg_pos;
		++llvm_reg_pos;
		
		writer << "\t%" << llvm_pos << " = alloca " + type + ", align 4";
		writer << std::endl;
	}
	
	//Assign the variable
	auto child = var->children[0];
	
	switch (child->type) {
		//Integers
		case ltac::Int: {
			auto li = static_cast<LtacInt *>(child);
			
			writer << "\tstore " << type << " " << li->val << ", ";
			writer << type << "* %" << llvm_pos << ", align 4";
			writer << std::endl;
		} break;
		
		//TODO: Add the rest
	}
	
	writer << std::endl;
}

