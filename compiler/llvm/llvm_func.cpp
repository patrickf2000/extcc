#include "asm_llvm.hh"

//Builds an extern declaration
void Asm_LLVM::build_extern(LtacNode *node) {
	auto ext = static_cast<LtacExtern *>(node);
	
	//Build the signature
	std::string sig = type2str(ext->ret_type) + "(";
	
	for (int i = 0; i<ext->params.size(); i++) {
		if (i > 0)
			sig += ",";
			
		sig += type2str(ext->params[i]);
	}
	
	sig += ")";
	func_sigs[ext->name] = sig;
	
	//Build the declaration
	writer << "declare " << type2str(ext->ret_type) << " @";
	writer << ext->name << "(";
	
	for (int i = 0; i<ext->params.size(); i++) {
		if (i > 0)
			writer << ",";
			
		writer << type2str(ext->params[i]);
	}
	
	writer << ")" << std::endl;
}

//Builds a function declaration
void Asm_LLVM::build_func(LtacNode *node) {
	auto func = static_cast<LtacFunc *>(node);
	
	writer << std::endl;
	writer << "define i32 @" << func->name << "() {";
	writer << std::endl;
}

//Builds a function return
void Asm_LLVM::build_ret(LtacNode *node) {
	if (node->children.size() == 0) {
		writer << "\tret i32 0" << std::endl;
	} else {
		auto ret = node->children[0];
		
		switch (ret->type) {
			//Variables
			case ltac::Var: {
				auto var = static_cast<LtacVar *>(ret);
				int pos = vars[var->pos];
				std::string type = "";
				
				switch (var->d_type) {
					case DataType::Int: type = "i32"; break;
					
					//TODO: Add the rest
				}
				
				//Load the variable
				int pos2 = llvm_reg_pos;
				writer << "\t%" << pos2 << " = load " << type << ", " << type;
				writer << "* %" << pos << ", align 4" << std::endl;
				
				writer << "\tret " << type << " %" << pos2;
				writer << std::endl;
			} break;
		}
	}

	writer << "}" << std::endl;
}

//Builds a push-arg call
void Asm_LLVM::build_pusharg(LtacNode *node) {
	auto child = node->children[0];

	if (args.length() > 0)
		args += ", ";

	switch (child->type) {
		//Strings
		case ltac::String: {
			auto lstr = static_cast<LtacString *>(child);
			int len = strings[lstr->name];
		
			int pos2 = llvm_reg_pos;
			++llvm_reg_pos;
			
			writer << "\t%" << pos2 << " = getelementptr [";
			writer << len << " x i8], [";
			writer << len << " x i8]* @" << lstr->name;
			writer << ", i32 0, i32 0" << std::endl;
				
			args += "i8* %" + std::to_string(pos2);
		} break;
		
		//Variables
		case ltac::Var: {
			auto var = static_cast<LtacVar *>(child);
			
			switch (var->d_type) {
				//Integer variables
				case DataType::Int: {
					int pos = vars[var->pos];
				
					int pos2 = llvm_reg_pos;
					++llvm_reg_pos;
					
					writer << "\t%" << pos2 << " = load i32, i32* %";
					writer << pos << ", align 4" << std::endl;
					
					args += "i32 %" + std::to_string(pos2);
				} break;
				
				//TODO: Add the rest
			}
		} break;
		
		//TODO: Add the rest
	}
}

//Builds a function call
void Asm_LLVM::build_func_call(LtacNode *node) {
	auto fc = static_cast<LtacFuncCall *>(node);
	auto sig = func_sigs[fc->name];
	
	writer << "\tcall " + sig + " @" << fc->name << "(";
	writer << args << ") nounwind" << std::endl;
	writer << std::endl;
	
	args = "";
}

