#include "ltac_build.hh"

//Translates integer math to instructions
void LTAC_Builder::build_int_math(LtacVar *var, AstNode *node, int r) {
	auto reg = new LtacReg;
	reg->pos = r;
	
	//Load the first value to a register
	auto ast_first = node->children[0];
	auto first = convert_ast_var(ast_first);
	
	if (ast_first->type == AstType::FuncCall) {
		file->code->children.push_back(first);
		
		auto ret = new LtacRetReg;
		reg->children.push_back(ret);
	} else {
		reg->children.push_back(first);
	}
	
	file->code->children.push_back(reg);
	
	//Now assemble the math
	for (int i = 1; i<node->children.size(); i+=2) {
		auto op = node->children[i];
		auto next = node->children[i+1];
		
		auto reg2 = new LtacReg;
		reg2->pos = r;
		auto math = new LtacIMath;
		math->children.push_back(reg2);
		
		//Build the operator
		switch (op->type) {
			case AstType::Add: math->op = Operator::Add; break;
			case AstType::Sub: math->op = Operator::Sub; break;
			case AstType::Mul: math->op = Operator::Mul; break;
			case AstType::Div: math->op = Operator::Div; break;
			case AstType::Mod: math->op = Operator::Mod; break;
		}
		
		//Build the source
		//If we have a function call, we need to store and restore the register
		if (next->type == AstType::FuncCall) {
			//Save the register
			auto var2 = new LtacVar;
			var2->pos = var->pos;
			file->code->children.push_back(var2);
			
			auto reg3 = new LtacReg;
			reg3->pos = r;
			var2->children.push_back(reg3);
			
			//Build the function call
			auto src = convert_ast_var(next);
			file->code->children.push_back(src);
			
			auto rreg = new LtacRetReg;
			math->children.push_back(rreg);
			
			//Restore the register
			auto reg4 = new LtacReg;
			reg4->pos = r;
			file->code->children.push_back(reg4);
			
			auto var3 = new LtacVar;
			var3->pos = var->pos;
			reg4->children.push_back(var3);
		} else {
			auto src = convert_ast_var(next);
			math->children.push_back(src);
		}
		
		file->code->children.push_back(math);
	}
	
	//Store the register back to memory
	auto dest_reg = new LtacReg;
	dest_reg->pos = r;
	var->children.push_back(dest_reg);
	file->code->children.push_back(var);
}
