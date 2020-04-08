#include "ltac_build.hh"

//Translates integer math to instructions
void LTAC_Builder::build_int_math(LtacVar *var, AstNode *node, int r) {
	auto reg = new LtacReg;
	reg->pos = r;
	file->code->children.push_back(reg);
	
	//Load the first value to a register
	auto first = convert_ast_var(node->children[0]);
	reg->children.push_back(first);
	
	//Now assemble the math
	for (int i = 1; i<node->children.size(); i+=2) {
		auto op = node->children[i];
		auto next = node->children[i+1];
		
		auto reg2 = new LtacReg;
		reg2->pos = r;
		auto math = new LtacIMath;
		math->children.push_back(reg2);
		file->code->children.push_back(math);
		
		//Build the operator
		switch (op->type) {
			case AstType::Add: math->op = Operator::Add; break;
			case AstType::Sub: math->op = Operator::Sub; break;
			case AstType::Mul: math->op = Operator::Mul; break;
			case AstType::Div: math->op = Operator::Div; break;
			case AstType::Mod: math->op = Operator::Mod; break;
		}
		
		//Build the source
		auto src = convert_ast_var(next);
		math->children.push_back(src);
	}
	
	//Store the register back to memory
	auto dest_reg = new LtacReg;
	dest_reg->pos = r;
	var->children.push_back(dest_reg);
	file->code->children.push_back(var);
}
