#pragma once

#include <stack>
#include <map>
#include <string>

#include <parser.hh>
#include <ast.hh>

#include <c_lex.hh>

enum class CondType {
	If,
	Elif,
	While,
	DoWhile,
	For
};

class CParser : public Parser {
public:
	explicit CParser(std::string in) {
		top = new AstScope;
		scan = new CScanner(in);
		syntax = new Syntax(scan);
		
		topNodes.push(top);
	}
	void parse();
private:
	std::stack<AstNode *> topNodes;
	std::stack<AstNode *> blockEnds;
	bool add_ret = false;
	
	//Holds declared variables
	std::map<std::string, Var> vars;
	
	//Functions
	DataType token2type(int token);
	
	//Function stuff
	void buildExtern();
	void buildFuncDec(AstFuncDec *fd);
	void buildFuncCall(AstFuncCall *fc, bool add_top = true);
	void buildReturn();
	
	//Variable stuff
	void buildVarIncrement(std::string name, bool add_end = false);
	void buildVarAssign(AstVarDec *vd, int stop = CTokenType::SemiColon, bool add_end = false);
	void buildArrayDec(AstArrayDec *arr);
	
	//Flow control
	void buildCond(CondType type);
	void buildElse();
	void buildFor();
	
	//Utilities
	void addChildren(AstNode *parent, int stop = CTokenType::SemiColon);
	AstNode *buildNode(Token t, bool float2dbl = false);
};
