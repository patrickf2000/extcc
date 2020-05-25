#pragma once

#include <string>
#include <map>
#include <stack>

#include <ast.hh>
#include <pasm/pasm.hh>

using namespace PASM;

struct VarInfo {
	int pos;
	DType type;
	int size;
};

class PasmBuilder {
public:
	explicit PasmBuilder(std::string file);
	PasmFile *buildFile(AstNode *top);
protected:
	void assemble(AstNode *top);
	std::string buildString(AstNode *node);
	
	Func *buildFunc(AstNode *node);
	void buildFuncCall(AstNode *node);
	void buildRet(AstNode *node);
	
	void buildVarDec(AstNode *node);
	void buildVarAssign(AstNode *node);
	void buildVarAssign(AstVarAssign *node);
	
	void buildIMath(AstMath *math, VarInfo dest);
	
	Operand getOpType(AstNode *node);
	int getOp(AstNode *node);
	void buildCmp(AstNode *node);
	void buildICmp(AstCond *cond);
	void buildCond(AstCond *cond);
private:
	PasmFile *file;
	
	//Variable information
	int stackPos = 0;
	std::map<std::string, int> varPos;		//Meant for fast position lookups
	std::map<std::string, VarInfo> vars;	//Meant for general variable information
	
	//String information
	int strPos = 0;
	
	//Label stuff
	int lbl_count = 0;
	std::stack<std::string> labels;
	std::stack<std::string> end_lbls;
};
