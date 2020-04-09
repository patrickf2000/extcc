#pragma once

#include <vector>
#include <string>

#include <base/asm_base.hh>
#include <ltac/ltac.hh>

class Asm_Arm7 : public AsmGen {
public:
	Asm_Arm7(LtacFile *f) : AsmGen(f) {}
	
	void build_data(LtacDataSec *data);
	void build_code(LtacCodeSec *code);
	
	void build_func(LtacNode *node);
	void build_ret(LtacNode *node);
	void build_pusharg(LtacNode *node, bool is_arg=false);
	void build_func_call(LtacNode *node);
	
	void build_reg(LtacNode *node);
	void build_var(LtacNode *node);
private:
	std::vector<std::string> data_labels;
};
