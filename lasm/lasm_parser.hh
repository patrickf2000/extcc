#pragma once

#include <string>
#include <stack>

#include <parser.hh>
#include <syntax.hh>
#include <ltac/ltac.hh>

#include <lasm_lex.hh>

class AsmParser {
public:
	explicit AsmParser(std::string in) {
		scan = new AsmScanner(in);
		file = new LtacFile;
		syntax = new Syntax(scan);
	}
	
	LtacFile *getFile() {
		return file;
	}
	
	void parse();
private:
	AsmScanner *scan;
	LtacFile *file;
	Syntax *syntax;
	
	//Control stuff
	std::stack<LtacNode *> topNode;
	int stack_pos = 0;
	
	//Check functions
	void checkCode();
	void checkData();
	
	//Parser functions
	void buildSection();
	void buildFunc();
	void calcStackSize();
	void buildPushArg();
	void buildFuncCall();
	void buildRet();
	void buildString();
	void buildVar();
	
	void addChildren(LtacNode *parent, bool inc_stack=false);
};
