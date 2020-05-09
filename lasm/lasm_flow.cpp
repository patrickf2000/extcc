#include "lasm_parser.hh"

//Builds a comparison statement
void AsmParser::buildCmp() {
	auto cmp = new LtacICmp;
	file->code->children.push_back(cmp);
	
	cmp->lval = addChildren(nullptr, false);
	cmp->rval = addChildren(nullptr, false);
}

//Builds a jump statement
void AsmParser::buildJmp(int type) {

}
