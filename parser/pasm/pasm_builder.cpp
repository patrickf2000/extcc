#include "pasm_builder.hh"

using namespace PASM;

//The constructor- set things up
PasmBuilder::PasmBuilder(std::string file) {
	this->file = new PasmFile;
	this->file->name = file;
}

//Build a PASM file
PasmFile *PasmBuilder::buildFile(AstNode *top) {
	assemble(top);
	return file;
}

//The main assembly function
void PasmBuilder::assemble(AstNode *top) {
	for (auto node : top->children) {
		switch (node->type) {
			case AstType::Scope: assemble(node); break;
			
			case AstType::FuncDec: {
				buildFunc(node); 
				assemble(node);
			} break;
			
			case AstType::FuncCall: buildFuncCall(node); break;
		}
	}
}

//Builds a function declaration
void PasmBuilder::buildFunc(AstNode *node) {
	auto *fd = static_cast<AstFuncDec *>(node);
	auto *func = new Func(fd->get_name());
	file->code.push_back(func);
}

//Builds a function call
void PasmBuilder::buildFuncCall(AstNode *node) {
	auto *fc = static_cast<AstFuncCall *>(node);
	
	if (fc->get_name() == "syscall") {
		for (auto arg : fc->children) {
			switch (arg->type) {
				case AstType::Int: {
					auto *i = static_cast<AstInt *>(arg);
					auto *sysarg = new ISysArg(i->get_val());
					file->code.push_back(sysarg);
				} break;
				
				//TODO: Add others
			}
		}
		
		auto syscall = new SysCall;
		file->code.push_back(syscall);
	} else {
		//TODO: Add regular function calls
	}
}

