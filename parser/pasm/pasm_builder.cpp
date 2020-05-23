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
				auto func = buildFunc(node); 
				assemble(node);
				
				int stackSize = 0;
				if (stackPos > 0) {
					while (stackSize < (stackPos + 1))
						stackSize += 16;
				}
				
				func->stackSize = stackSize;
				stackPos = 0;
			} break;
			
			case AstType::FuncCall: buildFuncCall(node); break;
			case AstType::Return: buildRet(node); break;
			
			case AstType::VarDec: buildVarDec(node); break;
			case AstType::VarAssign: buildVarAssign(node); break;
		}
	}
}

//Builds a string and adds to the data section
std::string PasmBuilder::buildString(AstNode *node) {
	auto str = static_cast<AstString *>(node);
	auto name = "STR" + std::to_string(strPos);
	++strPos;
	
	auto pstr = new PasmString(name, str->get_val());
	file->data.push_back(pstr);
	
	return name;
}


