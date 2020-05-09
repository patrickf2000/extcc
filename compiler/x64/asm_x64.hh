#pragma once

#include <string>
#include <fstream>

#include <base/asm_base.hh>
#include <ltac/ltac.hh>

extern std::string var_regs[];

class Asm_x64 : public AsmGen {
public:
	Asm_x64(LtacFile *f) : AsmGen(f) {}
protected:
	void build_data(LtacDataSec *data);
	void build_code(LtacCodeSec *code);
	
	void build_func(LtacNode *node);
	void build_push_arg(LtacNode *node, bool is_arg=false);
	void build_func_call(LtacNode *node);
	void build_ret(LtacNode *node);
	
	void build_reg(LtacNode *node);
	void build_var(LtacNode *node);
	void build_int_math(LtacTypeNode *var, LtacNode *src, bool store=true);
	void build_float_math(LtacVar *var, LtacNode *src, bool store=true);
	void build_vector_math(LtacVar *var, LtacNode *src);
	
	void build_array(LtacNode *node);
	void build_array_acc(LtacNode *node);
	void build_array_set(LtacNode *node);
	
	void build_icmp(LtacNode *node);
	void build_jmp(LtacNode *node);
	
	void build_imath(LtacNode *node);
	void build_idiv(LtacIMath *math);
	void build_f32math(LtacNode *node);
	
	void build_vload(LtacNode *node);
	void build_vstore(LtacNode *node);
	void build_vmath(LtacNode *node);
};
