#pragma once

#include <string>
#include <stack>
#include <map>

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
	std::map<std::string, int> vars;
	std::map<std::string, DataType> types;
	std::map<std::string, bool> pointers;
	int stack_pos = 0;
	
	//Check functions
	void checkCode();
	void checkData();
	
	//Parser functions
	void buildSection();
	void buildFunc();
	void calcStackSize();
	void buildExtern();
	void buildPushArg();
	void buildFuncCall();
	void buildRet();
	void buildString();
	void buildFloat();
	void buildVar(bool is_ptr = false);
	void buildLdr(RegType rtype = RegType::Gp);
	void buildStr(RegType rtype = RegType::Gp);
	void buildMath(int type, int op);
	void buildArraySet();
	void buildArrayAcc();
	
	void addChildren(LtacNode *parent, bool inc_stack=false);
	DataType getDataType(Token t);
};

