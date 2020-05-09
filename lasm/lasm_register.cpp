#include "lasm_parser.hh"

using namespace LtacAsm;

//Builds the load register command
void AsmParser::buildLdr(RegType rtype) {
	auto *reg = new LtacReg;
	reg->rtype = rtype;
	file->code->children.push_back(reg);
	
	Token no = scan->getNext();
	Token val = scan->getNext();
	
	if (no.type != AsmTokenType::IntL)
		syntax->fatalError("Expected register number.");
	
	int v = std::stoi(no.id);
	
	if (v <= 0)
		syntax->fatalError("Invalid register; A register must be a number greater than 0.");
		
	reg->pos = v;
	
	//Build the value being loaded
	switch (val.type) {
		//Other variables
		case AsmTokenType::Name: {
			auto *lv = new LtacVar;
			lv->pos = vars[val.id];
			reg->children.push_back(lv);
		} break;
		
		//TODO: Add rest
	}
}

//Builds the store register command
void AsmParser::buildStr(RegType rtype) {
	checkCode();
	
	auto *var = new LtacVar;
	file->code->children.push_back(var);
	
	auto *reg = new LtacReg;
	reg->rtype = rtype;
	var->children.push_back(reg);
	
	if (rtype == RegType::Ret) {
		Token val = scan->getNext();
		
		if (val.type != AsmTokenType::Name)
			syntax->fatalError("Expected variable");
			
		var->pos = vars[val.id];
		var->d_type = types[val.id];
		var->is_ptr = pointers[val.id];
	} else {
		Token no = scan->getNext();
		Token val = scan->getNext();
		
		if (no.type != AsmTokenType::IntL)
			syntax->fatalError("Expected register number.");
			
		if (val.type != AsmTokenType::Name)
			syntax->fatalError("Expected variable.");
			
		int v = std::stoi(no.id);
		
		if (v <= 0)
			syntax->fatalError("Invalid register; A register must be greater than 0.");
			
		reg->pos = v;
		var->pos = vars[val.id];
	}
}

//Builds the load/store vector register command
//Types:
// 1- > vldr
// 2 -> vldri
// 3 -> vstr
void AsmParser::buildVLdr(int type) {
	checkCode();
	
	auto *vector = new LtacVOp;
	if (type == 1) vector = new LtacVLoad;
	else if (type == 2) vector = new LtacVLoadI;
	else if (type == 3) vector = new LtacVStore;
	
	file->code->children.push_back(vector);
	
	Token no = scan->getNext();
	Token val = scan->getNext();
	
	if (no.type != AsmTokenType::IntL)
		syntax->fatalError("Expected register number.");
		
	if (val.type != AsmTokenType::Name)
		syntax->fatalError("Expected variable.");
		
	int v = std::stoi(no.id);
	
	if (v <= 0)
		syntax->fatalError("Invalid register; A register must be greater than 0.");
		
	vector->reg = v;
	vector->pos = vars[val.id];
	vector->d_type = types[val.id];
	vector->is_ptr = pointers[val.id];
	
	if (type == 2) {
		addChildren(vector, false);
	}
}



