#include "c_parser.hh"
#include "c_lex.hh"

//Builds a structure
// Note that this is not the same as the variable
void CParser::buildStruct(std::string name) {
	auto *dec = new AstStructDec(name);
	topNodes.top()->children.push_back(dec);
	
	Token next = scan->getNext();
	Var v;
	
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
