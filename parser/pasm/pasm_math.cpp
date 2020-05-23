#include "pasm_builder.hh"

using namespace PASM;

//Builds an integer math chain
void PasmBuilder::buildIMath(AstMath *math, int destPos) {
	//Load the first element
	auto op1 = math->children[0];
	
	switch (op1->type) {
		//Variable
		case AstType::Id: {
			auto id = static_cast<AstID *>(op1);
			auto var = vars[id->get_name()];
			
			auto ldrv = new LdrV(1, var.pos);
			ldrv->dType = var.type;
			file->code.push_back(ldrv);
		} break;
		
		//TODO: Add rest
	}
}
