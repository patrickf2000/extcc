#pragma once

#include <string>

#include <ast.hh>
#include <lex.hh>
#include <syntax.hh>

class Parser {
public:
	AstNode *getTree() { return top; }
	virtual void parse() {}
	
	void runSyntax() {
		syntax->displayWarnings();
		syntax->displayErrors();
	}
protected:
	Scanner *scan;
	Syntax *syntax;
	AstNode *top;
};
