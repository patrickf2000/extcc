#include "asm_arm7.hh"
#include "registers.hh"

using namespace Arm7;

//Builds a function declaration
void Asm_Arm7::build_func(LtacNode *node) {
	auto fd = static_cast<LtacFunc *>(node);
	
	if (fd->name == "main") {
		writer << ".global main" << std::endl;
	}
	
	writer << std::endl;
	writer << fd->name << ":" << std::endl;
	
	//Setup the stack
	writer << "\tpush {fp, lr}" << std::endl;
	writer << "\tadd fp, sp, #4" << std::endl;
	writer << "\tsub sp, sp, #" << fd->stack_size << std::endl;
	writer << std::endl;
}

//Builds a function return
void Asm_Arm7::build_ret(LtacNode *node) {
	writer << std::endl;
	
	if (node->children.size() > 0) {
		auto val = node->children[0];
		
		switch (val->type) {
			//Integers
			case ltac::Int: {
				auto i = static_cast<LtacInt *>(val);
				writer << "\tmov r0, #" << i->val << std::endl;
			} break;
			
			//TODO: Add the rest
		}
	}

	writer << "\tsub sp, fp, #4" << std::endl;
	writer << "\tpop {fp, pc}" << std::endl;
}

//Builds a pusharg command
void Asm_Arm7::build_pusharg(LtacNode *node, bool is_arg) {
	LtacNode *arg = node;
	
	if (!is_arg)
		arg = node->children[0];
	
	//Build the argument	
	switch (arg->type) {
		//Integers
		case ltac::Int: {
			auto i = static_cast<LtacInt *>(arg);
			
			writer << "\tmov " << call_regs[call_index] << ", #";
			writer << i->val << std::endl;
		} break;
		
		//Strings
		case ltac::String: {
			auto str = static_cast<LtacString *>(arg);
			
			writer << "\tldr " << call_regs[call_index] << ", ";
			writer << str->name << std::endl;
		} break;
		
		//TODO: Add the rest
	}
	
	++call_index;
}

//Builds a function call
void Asm_Arm7::build_func_call(LtacNode *node) {
	auto fc = static_cast<LtacFuncCall *>(node);
	
	writer << "\tbl " << fc->name << std::endl;
	writer << std::endl;
	
	call_index = 0;
}


