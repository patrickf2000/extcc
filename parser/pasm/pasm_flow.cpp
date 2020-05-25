#include "pasm_builder.hh"

using namespace PASM;

//Return operand type for AST node
Operand PasmBuilder::getOpType(AstNode *node) {
	switch (node->type) {
		case AstType::Int:
		case AstType::Float:
		case AstType::Double: return Operand::Const;
		
		case AstType::Id: return Operand::Var;
	}
	
	return Operand::None;
}

//Returns the operand for an AST node
int PasmBuilder::getOp(AstNode *node) {
	switch (node->type) {
		case AstType::Int: {
			auto i = static_cast<AstInt *>(node);
			return i->get_val();
		}
		
		case AstType::Id: {
			auto id = static_cast<AstID *>(node);
			return varPos[id->get_name()];
		}
	}
	
	return 0;
}

//Builds a comparison statement
void PasmBuilder::buildCmp(AstNode *node, bool isLoop) {
	auto cond = static_cast<AstCond *>(node);
	
	//First, determine type; this is done with l-val
	switch (cond->lval->type) {
		case AstType::Int: buildICmp(cond); break;
		
		case AstType::Id: {
			auto id = static_cast<AstID *>(cond->lval);
			auto info = vars[id->get_name()];
			
			switch (info.type) {
				case DType::Int: buildICmp(cond); break;
				
				//TODO: Add rest
			}
		} break;
		
		//TODO: Add rest
	}
	
	if (isLoop)
		buildLoopCmp(cond);
	else
		buildCond(cond);
}

//Build an integer comparsion
void PasmBuilder::buildICmp(AstCond *cond) {
	auto icmp = new ICmp;
	file->code.push_back(icmp);
	
	//l-val
	icmp->op1 = getOpType(cond->lval);
	icmp->pos1 = getOp(cond->lval);
	
	//r-val
	icmp->op2 = getOpType(cond->rval);
	icmp->pos2 = getOp(cond->rval);
}

//Builds a branch statement for conditionals
void PasmBuilder::buildCond(AstCond *cond) {
	auto jmp = new Branch(labels.top());
	auto defaultJmp = new Branch(end_lbls.top());
	
	labels.pop();
	
	switch (cond->get_op()) {
		case CondOp::Equals: jmp->jmp = JmpType::Neq; break;
		case CondOp::NotEquals: jmp->jmp = JmpType::Eq; break;
		case CondOp::Greater: jmp->jmp = JmpType::Le; break;
		case CondOp::GreaterEq: jmp->jmp = JmpType::L; break;
		case CondOp::Less: jmp->jmp = JmpType::Ge; break;
		case CondOp::LessEq: jmp->jmp = JmpType::G; break;
	}
	
	file->code.push_back(jmp);
	assemble(cond);
	file->code.push_back(defaultJmp);
}

//Builds a loop
void PasmBuilder::buildLoop(AstNode *node) {
	//Generate the label names
	std::string top_lbl = "L" + std::to_string(lbl_count);
	++lbl_count;
	labels.push(top_lbl);

	std::string cmp_lbl = "L" + std::to_string(lbl_count);
	++lbl_count;

	//Jump to the comparison label
	auto jmp = new Branch(cmp_lbl);
	file->code.push_back(jmp);

	auto lbl = new Label(top_lbl);
	file->code.push_back(lbl);

	//Assemble the body
	assemble(node);

	//Insert the comparison label
	lbl = new Label(cmp_lbl);
	file->code.push_back(lbl);

	//Build the comparison
	buildCmp(node, true);
}

//Builds the conditional statement for a loop
void PasmBuilder::buildLoopCmp(AstCond *cond) {
	auto jmp = new Branch(labels.top());
	labels.pop();

	switch (cond->get_op()) {
		case CondOp::Equals: jmp->jmp = JmpType::Eq; break;
		case CondOp::NotEquals: jmp->jmp = JmpType::Neq; break;
		case CondOp::Greater: jmp->jmp = JmpType::G; break;
		case CondOp::GreaterEq: jmp->jmp = JmpType::Ge; break;
		case CondOp::Less: jmp->jmp = JmpType::L; break;
		case CondOp::LessEq: jmp->jmp = JmpType::Le; break;
	}
	
	file->code.push_back(jmp);
}

	
