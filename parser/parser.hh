#pragma once

#include <string>

#include <ast.hh>
#include <lex.hh>

class Parser {
public:
	AstNode *getTree() { return top; }
	virtual void parse() {}
protected:
	Scanner *scan;
	AstNode *top;
};
