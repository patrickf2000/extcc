#pragma once

#include <string>
#include <map>

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
	
	void buildFunc(AstNode *node);
	void buildFuncCall(AstNode *node);
	
	void buildVarDec(AstNode *node);
	void buildVarAssign(AstNode *node);
	void buildVarAssign(AstVarAssign *node);
private:
	PasmFile *file;
	
	//Variable information
	int stackPos = 0;
	std::map<std::string, int> varPos;		//Meant for fast position lookups
	std::map<std::string, VarInfo> vars;	//Meant for general variable information
};
