#include "lasm_parser.hh"

using namespace LtacAsm;

//The integer store operation
//Stores an immediate value to an integer variable
void AsmParser::buildIStoreI() {
	checkCode();
	auto store = new LtacIStrImm;
	file->code->children.push_back(store);
	
	Token name = scan->getNext();
	Token val = scan->getNext();
	
	//Syntax check
	if (name.type != AsmTokenType::Name)
		syntax->fatalError("[i.storei] Expected variable name.");
	else if (val.type != AsmTokenType::IntL)
		syntax->fatalError("[i.storei] Expected integer literal.");
		
	store->pos = vars[name.id];
	store->val = std::stoi(val.id);
}
