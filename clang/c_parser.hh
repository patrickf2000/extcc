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
		syntax = new Syntax;
		
		topNodes.push(top);
	}
	void parse();
private:
	std::stack<AstNode *> topNodes;
	bool add_ret = false;
	
	DataType token2type(int token);
	void buildExtern();
	void buildFuncDec(AstFuncDec *fd);
	void buildFuncCall(AstFuncCall *fc);
	void buildReturn();
	void buildVarDec(AstVarDec *vd);
	void buildIf(bool elif);
	void addChildren(AstNode *parent, int stop = CTokenType::SemiColon);
	AstNode *buildNode(Token t);
};
