#include "asm_llvm.hh"

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
		vars[pos] = llvm_reg_pos;
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
