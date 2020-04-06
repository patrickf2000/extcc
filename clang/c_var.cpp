#include "c_parser.hh"
#include "c_lex.hh"

//Builds a variable declaration
void CParser::buildVarAssign(AstVarDec *vd, int stop, bool add_end) {
	std::vector<AstNode *> nodes;
	
	if (add_end)
		blockEnds.push(vd);
	else
		topNodes.top()->children.push_back(vd);
		
	//Check to see if we need to convert float literals
	// to double-precision numbers
	bool float2dbl = false;
	if (vd->get_type() == DataType::Double)
		float2dbl = true;
	
	//Get all tokens until the semi-colon
	Token next = scan->getNext();
	
	while (next.type != stop) {
		nodes.push_back(buildNode(next, float2dbl));
		next = scan->getNext();
	}
	
	//See if we have a math expression
	if (nodes.size() == 1) {
		vd->children.push_back(nodes.at(0));
	} else if (nodes.size() == 0) {
		syntax->addError("Invalid syntax when assigning a variable.");
	} else {
		auto math = new AstMath;
		vd->children.push_back(math);
		
		bool is_float = false;
		if (vd->get_type() == DataType::Float 
			|| vd->get_type() == DataType::Double) {
			is_float = true;
		}
		
		for (auto child : nodes) {
			if (child->type == AstType::Int && is_float) {
				auto *i = static_cast<AstInt *>(child);
				auto *f = new AstFloat((float)i->get_val());
				math->children.push_back(f);
			} else {
				math->children.push_back(child);
			}
		}
	}
}

//Builds an array declaration
void CParser::buildArrayDec(AstArrayDec *arr) {
	topNodes.top()->children.push_back(arr);
	
	//Set the size
	Token next = scan->getNext();
	
	if (next.type != CTokenType::No) {
		syntax->addError("Invalid array size.");
		return;
	}
		
	int size = std::stoi(next.id);
	arr->set_size(size);
	
	//Syntax check
	next = scan->getNext();
	
	if (next.type != CTokenType::RBracket) {
		syntax->addError("Invalid array syntax.");
		return;
	}
	
	//Build the rest of the declaration
	next = scan->getNext();
	
	if (next.type == CTokenType::SemiColon) {
		for (int i = 0; i<size; i++) {
			auto i = new AstInt(0);
			arr->children.push_back(i);
		}
		
		return;
	}
	
	//Do some more syntax check
	if (next.type != CTokenType::Assign)
		syntax->addError("Invalid array syntax.");
		
	next = scan->getNext();
	
	if (next.type != CTokenType::LeftCBrace)
		syntax->addError("Invalid array syntax.");
		
	//Load the elements
	next = scan->getNext();
	
	while (next.type != CTokenType::RightCBrace) {
		if (next.type != CTokenType::Comma)
			arr->children.push_back(buildNode(next));
		next = scan->getNext();
	}
	
	//Final syntax check
	next = scan->getNext();
	
	if (next.type != CTokenType::SemiColon)
		syntax->addError("Expected terminating token.");
}

