#include "lasm_parser.hh"

using namespace LtacAsm;

//Builds the array-set command
//Syntax <name> <index ref> <value>
void AsmParser::buildArraySet() {
	checkCode();
	
	Token name = scan->getNext();
	Token index_type = scan->getNext();
	Token index_val = scan->getNext();
	
	auto arrayset = new LtacArraySet;
	file->code->children.push_back(arrayset);
	
	//Syntax check
	if (name.type != AsmTokenType::Name)
		syntax->fatalError("Expected ptr name in arrayset.");
		
	//Build the index node
	if (index_type.type == AsmTokenType::Int) {
		auto li = new LtacInt;
		li->val = std::stoi(index_val.id);
		
		arrayset->index = li;
	} else if (index_type.type == AsmTokenType::Var) {
		auto lvar = new LtacVar;
		lvar->pos = vars[index_val.id];
		lvar->d_type = types[index_val.id];
		
		arrayset->index = lvar;
	} else {
		syntax->fatalError("Unknown value for arrayset");
	}
	
	//Build the new value
	addChildren(arrayset, false);
	
	arrayset->pos = vars[name.id];
	arrayset->d_type = types[name.id];
	arrayset->type_size = 4;
	arrayset->is_ptr = true;
}

//Builds the array access command
void AsmParser::buildArrayAcc() {
	checkCode();
	
	Token name = scan->getNext();
	auto acc = new LtacArrayAcc;
	auto var = new LtacVar;
	
	var->children.push_back(acc);
	file->code->children.push_back(var);
	
	if (name.type != AsmTokenType::Name)
		syntax->fatalError("Expected variable name.");
		
	acc->pos = vars[name.id];
	acc->d_type = types[name.id];
	acc->is_ptr = pointers[name.id];
	acc->type_size = 4;
		
	addChildren(acc, false);
	
	//Get the variable name
	name = scan->getNext();
	
	if (name.type != AsmTokenType::Name)
		syntax->fatalError("Expected variable name.");
		
	var->pos = vars[name.id];
	var->d_type = types[name.id];
	var->is_ptr = pointers[name.id];
}

