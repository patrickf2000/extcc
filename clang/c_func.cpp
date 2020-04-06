#include "c_parser.hh"
#include "c_lex.hh"

//Build an external function
void CParser::buildExtern() {
	auto *fd = new AstExternFunc;
	top->children.push_back(fd);
	
	//Get the tokens we need
	Token fType = scan->getNext();		//The function type
	Token fName = scan->getNext();		//The function name
	
	//Set the information
	fd->set_name(fName.id);
}

//Builds a function declaration
void CParser::buildFuncDec(AstFuncDec *fd) {
	top->children.push_back(fd);
	++scope_level;
	
	//Parse the arguments
	Token next = scan->getNext();
	int argc = 0;
	Var v;
	v.is_param = true;
	
	while (next.type != CTokenType::RightParen) {
		switch (next.type) {
			case CTokenType::Void:
			case CTokenType::Char:
			case CTokenType::Short:
			case CTokenType::Int:
			case CTokenType::Float:
			case CTokenType::Double: {
				v.type = token2type(next.type); 
				current_rval = v.type;
			} break;
			
			case CTokenType::Id: v.name = next.id; break;
			
			case CTokenType::Comma: {
				fd->args.push_back(v);
				v.scope_level = scope_level;
				vars[v.name] = v;
				v.is_ptr = false;
			} break;
			
			case CTokenType::Mul: v.is_ptr = true; break;
		}
		
		next = scan->getNext();
		++argc;
	}
	
	if (argc > 1)
		fd->args.push_back(v);
	
	//The next token should be a curly brace
	next = scan->getNext();
	
	if (next.type != CTokenType::LeftCBrace) {
		syntax->addError("Syntax error: Expected \'{\'");
	}
	
	//The new current top is the function declaration
	topNodes.push(fd);
	
	//If its a void function, we need to make sure a return
	// statement is added at the end
	if (fd->rtype == DataType::Void)
		add_ret = true;
}

//Builds a function call
void CParser::buildFuncCall(AstFuncCall *fc, bool add_top) {
	addChildren(fc, CTokenType::RightParen);
	
	if (add_top)
		topNodes.top()->children.push_back(fc);
}

//Builds a return statement
void CParser::buildReturn() {
	auto ret = new AstReturn;
	topNodes.top()->children.push_back(ret);
	
	//Load the children
	Token next = scan->getNext();
	std::vector<AstNode *> nodes;
	
	while (next.type != CTokenType::SemiColon) {
		nodes.push_back(buildNode(next));
		next = scan->getNext();
	}
	
	//See if we have a math expression
	if (nodes.size() == 1) {
		ret->children.push_back(nodes.at(0));
	} else {
		auto math = new AstMath;
		ret->children.push_back(math);
		
		bool is_float = false;
		if (current_rval == DataType::Float 
			|| current_rval == DataType::Double) {
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


