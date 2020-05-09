#include "lasm_parser.hh"

using namespace LtacAsm;

//Builds a comparison statement
void AsmParser::buildCmp() {
	auto cmp = new LtacICmp;
	file->code->children.push_back(cmp);
	
	cmp->lval = addChildren(nullptr, false);
	cmp->rval = addChildren(nullptr, false);
}

//Builds a jump statement
void AsmParser::buildJmp(int type) {
	auto jmp = new LtacJmp;
	file->code->children.push_back(jmp);
	
	Token dest = scan->getNext();
	jmp->dest = dest.id;
	
	if (dest.type != AsmTokenType::Name)
		syntax->fatalError("Expected label name.");
		
	switch (type) {
		case AsmTokenType::Jl: jmp->op = Operator::Less; break;
		
		//TODO: Add rest
	}
}
