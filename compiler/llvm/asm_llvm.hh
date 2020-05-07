#pragma once

#include <map>

#include <base/asm_base.hh>
#include <ltac/ltac.hh>

class Asm_LLVM : public AsmGen {
public:
	Asm_LLVM(LtacFile *f) : AsmGen(f) {}
	
	void build_data(LtacDataSec *data);
	void build_code(LtacCodeSec *code);
	
	void build_func(LtacNode *node);
	void build_ret(LtacNode *node);
	void build_pusharg(LtacNode *node);
	void build_func_call(LtacNode *node);
	
	void build_var(LtacNode *node);
private:
	std::map<std::string, int> strings;
	std::map<int, int> vars;
	int llvm_reg_pos = 1;
	
	std::string args = "";
};
