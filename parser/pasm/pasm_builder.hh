#pragma once

#include <string>

#include <ast.hh>
#include <pasm/pasm.hh>

using namespace PASM;

class PasmBuilder {
public:
	explicit PasmBuilder(std::string file);
	PasmFile *buildFile(AstNode *top);
protected:
	void assemble(AstNode *top);
private:
	PasmFile *file;
};
