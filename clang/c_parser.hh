#pragma once

#include <parser.hh>
#include <ast.hh>

#include <c_lex.hh>

class CParser : public Parser {
public:
	explicit CParser(std::string in) {
		top = new AstScope;
		scan = new CScanner(in);
	}
	void parse();
private:
	AstNode *currentTop;
	
	void buildExtern();
};
