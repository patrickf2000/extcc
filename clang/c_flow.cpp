#include "c_parser.hh"
#include "c_lex.hh"

//Builds an If statement
void CParser::buildCond(CondType type) {
	Token next = scan->getNext();
	
	if (next.type != CTokenType::LeftParen && type != CondType::For) {
		syntax->addError("Expected \'(\' in conditional statement.");
	}
	
	//Start constructing the AST node
	AstCond *cond = new AstIf;
	if (type == CondType::Elif) cond = new AstElif;
	else if (type == CondType::While) cond = new AstWhile;
	else if (type == CondType::For) cond = new AstFor;
	
	//Scan and build
	Token lval;
	if (type == CondType::For)
		lval = next;
	else
		lval = scan->getNext();
	
	cond->lval = buildNode(lval);
	
	Token op = scan->getNext();
	Token rval = scan->getNext();
	
	if (op.type == CTokenType::Assign) {
		if (rval.type == CTokenType::Assign) {
			cond->set_op(CondOp::Equals);
			rval = scan->getNext();
		} else {
			syntax->addError("Assignment invalid in a conditional.");
		}
	} else if (op.type == CTokenType::Greater) {
		if (rval.type == CTokenType::Assign) {
			cond->set_op(CondOp::GreaterEq);
			rval = scan->getNext();
		} else {
			cond->set_op(CondOp::Greater);
		}
	} else if (op.type == CTokenType::Less) {
		if (rval.type == CTokenType::Assign) {
			cond->set_op(CondOp::LessEq);
			rval = scan->getNext();
		} else {
			cond->set_op(CondOp::Less);
		}
		
	//TODO: Add not equals
	} else {
		syntax->addError("Unknown conditional operator");
	}
	
	cond->rval = buildNode(rval);

	//Add to the tree and update the stack
	if (type == CondType::Elif)
		topNodes.top()->children.pop_back();
	
	topNodes.top()->children.push_back(cond);
	topNodes.push(cond);
	
	//TODO: Final syntax check
}

//Builds an else statement
void CParser::buildElse() {
	Token next = scan->getNext();
	
	if (next.type == CTokenType::If) {
		buildCond(CondType::Elif);
		return;
	} else if (next.type != CTokenType::LeftCBrace) {
		syntax->addError("Expected curly brace after else");
		return;
	}
	
	topNodes.top()->children.pop_back();
	
	auto e = new AstElse;
	topNodes.top()->children.push_back(e);
	topNodes.push(e);
}

//Builds a for loop
void CParser::buildFor() {
	Token next = scan->getNext();
	
	if (next.type != CTokenType::LeftParen)
		syntax->addError("Expected token");
		
	//Build the first part
	next = scan->getNext();
	
	switch (next.type) {
		//We are resetting another variable
		case CTokenType::Id: {
			Token id = scan->getNext();
			next = scan->getNext();
			
			if (next.type == CTokenType::Assign) {
				auto *va = new AstVarAssign(id.id);
				buildVarAssign(va);
			} else {
				syntax->addError("Invalid for-loop syntax.");
			}
		} break;
		
		//We are declaring a new variable
		case CTokenType::Char:
		case CTokenType::Short:
		case CTokenType::Int:
		case CTokenType::Float:
		case CTokenType::Double: {
			int type = next.type;
			Token id = scan->getNext();
			next = scan->getNext();
			
			if (next.type == CTokenType::Assign) {
				auto *vd = new AstVarDec(id.id);
				vd->set_type(token2type(type));
				buildVarAssign(vd);
			} else {
				syntax->addError("Invalid for-loop syntax.");
			}
		} break;
		
		//We have a syntax error
		default: syntax->addError("Invalid for-loop syntax.");
	}
	
	//Build the second part
	buildCond(CondType::For);
	
	next = scan->getNext();
	if (next.type != CTokenType::SemiColon)
		syntax->addError("Invalid for-loop syntax.");
	
	//Build the final part	
	Token id = scan->getNext();
	next = scan->getNext();
	
	//Regular assignment
	if (next.type == CTokenType::Assign) {
		auto *va = new AstVarAssign(id.id);
		buildVarAssign(va, CTokenType::RightParen, true);
		
	//Increment
	} else if (next.type == CTokenType::Plus) {
		next = scan->getNext();
		if (next.type != CTokenType::Plus) {
			syntax->fatalError("Expected token- perhaps you meant to increment?");
		}
		
		buildVarIncrement(id.id, true);
		
	//Syntax error
	} else {
		syntax->fatalError("Expected variable expression.");
	}
}

