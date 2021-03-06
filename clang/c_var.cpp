#include "c_parser.hh"
#include "c_lex.hh"

//Creates a new variable
void CParser::buildVarDec(int type, Token id, bool is_ptr, bool init_default) {
	auto *vd = new AstVarDec(id.id);
	DataType d_type = token2type(type);
	vd->set_type(d_type);
	vd->is_ptr = is_ptr;
	
	if (vars.find(vd->get_name()) != vars.end())
		syntax->fatalError("Variable previously declared in scope.");
				
	Var v;
	v.name = vd->get_name();
	v.type = d_type;
	v.is_array = false;
	v.is_param = false;
	v.is_ptr = is_ptr;
	v.scope_level = scope_level;
	vars[v.name] = v;
	
	if (init_default) {
		topNodes.top()->children.push_back(vd);
		AstNode *dnode;
		
		switch (d_type) {
			case DataType::Char: dnode = new AstChar(0); break;
			case DataType::Int: dnode = new AstInt(0); break;
			
			case DataType::Float: dnode = new AstFloat(0); break;
			case DataType::Double: dnode = new AstDouble(0); break;
		}
		
		vd->children.push_back(dnode);
	} else {
		buildVarAssign(vd);
	}
}

//Builds a variable increment statement
void CParser::buildVarIncrement(std::string name, bool add_end) {
	auto *va = new AstVarAssign(name);
	Var v = vars[va->get_name()];
	va->set_type(v.type);
		
	va->children.push_back(new AstNode(AstType::Inc));
	
	if (add_end)
		blockEnds.push(va);
	else
		topNodes.top()->children.push_back(va);
}

//Builds a variable declaration
void CParser::buildVarAssign(AstVarDec *vd, int stop, bool add_end) {
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
	std::vector<AstNode *> nodes;
	
	while (next.type != stop) {
		nodes.push_back(buildNode(next, float2dbl, true));
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
			auto ai = new AstInt(0);
			arr->children.push_back(ai);
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

