#pragma once

#include <string>
#include <map>
#include <stack>

#include <ast.hh>
#include <pasm/pasm.hh>

using namespace PASM;

struct VarInfo {
	int pos;			//If -1, return register
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
	std::string buildFloat(AstNode *node);
	std::string buildDouble(AstNode *node);
	
	Func *buildFunc(AstNode *node);
	void buildFuncCall(AstNode *node);
	void buildRet(AstNode *node);
	
	void buildVarDec(AstNode *node);
	void buildVarAssign(AstNode *node);
	void buildVarAssign(AstVarAssign *node);
	void buildArrayAcc(AstNode *node);
	void buildArraySet(AstNode *node);
	
	void buildIMath(AstMath *math, VarInfo dest);
	
	Operand getOpType(AstNode *node);
	int getOp(AstNode *node);
	void buildCmp(AstNode *node, bool isLoop = false);
	void buildICmp(AstCond *cond);
	void buildCond(AstCond *cond);
	void buildLoop(AstNode *node);
	void buildLoopCmp(AstCond *cond);
private:
	PasmFile *file;
	
	//Function stuff
	DataType retType = DataType::Void;
	
	//Variable information
	int stackPos = 0;
	std::map<std::string, int> varPos;		//Meant for fast position lookups
	std::map<std::string, VarInfo> vars;	//Meant for general variable information
	
	//String information
	int strPos = 0;
	
	//Float information
	int fltPos = 0;
	
	//Label stuff
	int lbl_count = 0;
	std::stack<std::string> labels;
	std::stack<std::string> end_lbls;
};

