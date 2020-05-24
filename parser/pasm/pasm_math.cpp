#include "pasm_builder.hh"

using namespace PASM;

//Builds an integer math chain
void PasmBuilder::buildIMath(AstMath *math, VarInfo dest) {
	//Add a formatting space
	file->code.push_back(new PasmSpace);

	//Load the first element
	auto op1 = math->children[0];
	
	switch (op1->type) {
		//Variable
		case AstType::Id: {
			auto id = static_cast<AstID *>(op1);
			auto var = vars[id->get_name()];
			
			auto load = new ILdr(1, var.pos);
			load->opType = Operand::Var;
			file->code.push_back(load);
		} break;
		
		//Constants
		case AstType::Int: {
			auto i = static_cast<AstInt *>(op1);
			
			auto load = new ILdr(1, i->get_val());
			load->opType = Operand::Const;
			file->code.push_back(load);
		} break;
		
		//TODO: Add rest
	}
	
	//Iterate through the chain
	for (int i = 1; i<math->children.size(); i+=2) {
		auto op = math->children[i];
		auto next = math->children[i+1];
		MathType mType = MathType::None;
		
		switch (op->type) {
			case AstType::Add: mType = MathType::Add; break;
			case AstType::Sub: mType = MathType::Sub; break;
			case AstType::Mul: mType = MathType::Mul; break;
			case AstType::Div: mType = MathType::Div; break;
		}
		
		switch (next->type) {
			//Immediate
			case AstType::Int: {
				auto i = static_cast<AstInt *>(next);
				auto math = new IMathRI(mType, 1, i->get_val());
				file->code.push_back(math);
			} break;
			
			//Variable
			case AstType::Id: {
				auto id = static_cast<AstID *>(next);
				int pos = varPos[id->get_name()];
				
				auto math = new IMathRV(mType, 1, pos);
				file->code.push_back(math);
			} break;
		}
	}
	
	//Store back to the destination
	auto str = new Str(1, dest.pos);
	str->dType = dest.type;
	file->code.push_back(str);
	file->code.push_back(new PasmSpace);
}

