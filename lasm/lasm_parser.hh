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
	int flt_count = 0;
	
	//Check functions
	void checkCode();
	void checkData();
	
	//Parser functions
	void buildSection();
	void buildLabel();
	
	void buildFunc();
	void calcStackSize();
	void buildExtern();
	void buildLdArg();
	void buildPushArg();
	void buildFuncCall();
	void buildRet();
	
	void buildString();
	void buildFloat();
	LtacFloat *buildFloat(std::string name, std::string val);
	void buildDouble();
	LtacDouble *buildDouble(std::string name, std::string val);
	
	void buildVar(bool is_ptr = false);
	void buildLdr(RegType rtype = RegType::Gp);
	void buildStr(RegType rtype = RegType::Gp);
	void buildVLdr(int type);
	void buildMath(int type, int op);
	
	void buildIStoreI();
	
	void buildArraySet();
	void buildArrayAcc();
	
	void buildCmp();
	void buildJmp(int type);
	
	LtacNode *addChildren(LtacNode *parent, bool inc_stack=false);
	DataType getDataType(Token t);
};

