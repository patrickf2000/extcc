#pragma once

#include <map>
#include <vector>
#include <stack>
#include <string>

#include "ltac.hh"
#include "ast.hh"

class LTAC_Builder {
public:
	LTAC_Builder() {}
	void set_ptr_size(int size) { ptr_size = size; }
	LtacFile *build_file(AstNode *top);
protected:
	void assemble(AstNode *top);
	
	//Function stuff
	bool has_func_calls(AstNode *top);
	LtacFunc *build_func(AstNode *node, bool is_extern=false);
	LtacFuncCall *build_func_call(AstNode *node);
	void build_ret(AstNode *node);
	
	//Variable stuff
	void build_var_dec(AstNode *node);
	void build_var_assign(AstNode *node);
	void build_multi_var_assign(AstNode *node);
	LtacNode *convert_ast_var(AstNode *val);
	void build_vector_dec(AstVarAssign *va);
	void build_vector_math(AstVarAssign *va);
	
	//Array stuff
	void build_array_dec(AstNode *node);
	LtacArrayAcc *build_array_acc(AstNode *node);
	void build_array_set(AstNode *node);
	
	//Structure stuff
	void build_struct(AstNode *node);
	void build_struct_dec(AstNode *node);
	void build_struct_mod(AstNode *node);
	LtacVar *build_struct_acc(AstNode *node);
	
	//Math stuff
	void build_int_math(LtacVar *var, AstNode *node, int r = 1);
	
	//Utility stuff
	LtacNode *build_string(AstNode *node);
	LtacNode *build_float(AstNode *node);
	LtacNode *build_double(AstNode *node);
	void inc_stack(DataType type);
	
	//Comparisons
	DataType determine_type(LtacNode *node);
	void build_while(AstNode *node);
	void build_loop(AstNode *node);
	void build_cmp(AstNode *node, bool is_loop = false);
	void build_cond_cmp(AstCond *cmp);
	void build_loop_cmp(AstCond *cmp);
	
	LtacFile *file;
private:
	//Variable related stuff
	int stack_pos = 0;
	int ptr_size = 8;
	std::map<std::string, Var> vars;
	std::map<std::string, AstStructDec *> structs;
	std::vector<Var> struct_vars;
	
	//Register variable
	bool enable_rvar = false;
	int rvar_index = 0;
	
	//String stuff
	int str_count = 0;
	std::map<std::string, std::string> dec_strings;
	
	//Float stuff
	int flt_count = 0;
	std::map<float, std::string> dec_flt;
	
	//Double stuff
	int dbl_count = 0;
	std::map<double, std::string> dec_dbl;
	
	//Label stuff
	int lbl_count = 0;
	std::stack<std::string> labels;
	std::stack<std::string> end_lbls;
};
