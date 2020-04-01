#pragma once

#include <string>

#include <ast.hh>
#include <lex.hh>

class Parser {
public:
	explicit Parser(std::string in);
	AstNode *getTree() { return top; }
	virtual void parse() {}
protected:
	Scanner *scan;
	AstNode *top;
};
