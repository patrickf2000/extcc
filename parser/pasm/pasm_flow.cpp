#include "pasm_builder.hh"

using namespace PASM;

//Builds a comparison statement
void PasmBuilder::buildCmp(AstNode *node) {
	auto cond = static_cast<AstCond *>(node);
	
	buildCond(cond);
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
