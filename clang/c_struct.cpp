#include "c_parser.hh"
#include "c_lex.hh"

//Builds a structure
// Note that this is not the same as the variable
void CParser::buildStruct(std::string name) {
	auto *dec = new AstStructDec(name);
	topNodes.top()->children.push_back(dec);
	
	Token next = scan->getNext();
	Var v;
	v.is_ptr = false;
	
	while (next.type != CTokenType::RightCBrace) {
		switch (next.type) {
			//Type declaration
			case CTokenType::Char:
			case CTokenType::Short:
			case CTokenType::Int:
			case CTokenType::Float:
			case CTokenType::Double: v.type = token2type(next.type); break;
			
			//The variable name
			case CTokenType::Id: v.name = next.id; break;
			
			//Is it a pointer?
			case CTokenType::Mul: v.is_ptr = true; break;
			
			//End of the field
			case CTokenType::SemiColon: {
				dec->fields.push_back(v);
				
				v.type = DataType::None;
				v.name = "";
				v.is_ptr = false;
			} break;
			
			//Syntax error
			default: syntax->fatalError("Invalid token in structure declaration.");
		}
		
		next = scan->getNext();
	}
}

//Builds a structure variable
void CParser::buildStructVar(std::string name, std::string vname) {
	auto *sv = new AstStruct;
	sv->str_name = name;
	sv->var_name = vname;
	topNodes.top()->children.push_back(sv);
	
	Token next = scan->getNext();
	
	if (next.type != CTokenType::SemiColon)
		syntax->fatalError("Expected terminator.");
}

//Builds a structure modification
void CParser::buildStructMod(std::string name) {
	//Get the variable name we are referencing and make sure its an ID
	Token next = scan->getNext();
	
	if (next.type != CTokenType::Id)
		syntax->fatalError("Invalid structure modification syntax.");

	//Create the modification
	auto *smod = new AstStructMod;
	smod->str_name = name;
	smod->var_name = next.id;
	
	//The next token should be the assignment operator
	next = scan->getNext();
	
	if (next.type != CTokenType::Assign)
		syntax->fatalError("Expected assignment operator");
		
	//Add the children
	buildVarAssign(smod);
}


