#pragma once

#include <stack>

#include <parser.hh>
#include <ast.hh>

#include <c_lex.hh>

class CParser : public Parser {
public:
	explicit CParser(std::string in) {
		top = new AstScope;
		scan = new CScanner(in);
		
		topNodes.push(top);
	}
	void parse();
private:
	std::stack<AstNode *> topNodes;
	
	void buildExtern();
	void buildFuncDec(AstFuncDec *fd);
};
