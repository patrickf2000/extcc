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
			
			//Comparisons
			case AstType::If: 
			case AstType::Elif: 
			case AstType::Else: {
				std::string name = "L" + std::to_string(lbl_count);
				++lbl_count;
				labels.push(name);
				
				if (node->type == AstType::If) {
					std::string end_name = "L" + std::to_string(lbl_count);
					++lbl_count;
					end_lbls.push(end_name);	
				}
				
				if (node->type != AstType::Else)
					buildCmp(node);
				else
					assemble(node);
				
				auto lbl = new Label(name);
				file->code.push_back(lbl);
			} break;
			
			case AstType::EndIf: {
				auto name = end_lbls.top();
				end_lbls.pop();
				
				auto lbl = new Label(name);
				file->code.push_back(lbl);
			} break;
		}
	}
}

void PasmBuilder::buildCmp(AstNode *node) {
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


