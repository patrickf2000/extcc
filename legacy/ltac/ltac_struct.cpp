#include "ltac_build.hh"

//Adds a structure definition
void LTAC_Builder::build_struct(AstNode *node) {
	auto *sd = static_cast<AstStructDec *>(node);
	structs[sd->get_name()] = sd;
}

//Builds a structure declaration
void LTAC_Builder::build_struct_dec(AstNode *node) {
	auto *sd = static_cast<AstStruct *>(node);
	auto *str = structs[sd->str_name];
	
	Var v;
	v.struct_name = sd->var_name;
	v.is_struct = true;
	v.is_ptr = false;
	v.is_array = false;
	v.is_param = false;
	
	for (auto field : str->fields) {
		if (field.is_ptr)
			stack_pos += 8;
		else
			inc_stack(field.type);
			
		v.name = field.name;
		v.type = field.type;
		v.is_ptr = field.is_ptr;
		v.stack_pos = stack_pos;
		
		struct_vars.push_back(v);
	}
	
	//Store the struct as a variable
	v.name = sd->var_name;
	v.stack_pos = stack_pos;
	v.is_struct = true;
	v.is_ptr = true;
	v.is_array = false;
	v.is_param = false;
	vars[v.name] = v;
}

//Builds a structure field modification
void LTAC_Builder::build_struct_mod(AstNode *node) {
	auto *mod = static_cast<AstStructMod *>(node);
	auto str_name = mod->str_name;
	auto var_name = mod->var_name;
	Var sv;
	
	for (auto v : struct_vars) {
		if (v.struct_name == str_name && v.name == var_name) {
			sv = v;
			break;
		}
	}
	
	auto *var = new LtacVar;
	var->pos = sv.stack_pos;
	var->d_type = sv.type;
	var->is_ptr = sv.is_ptr;
	
	//Build the assigned value
	auto val = mod->children[0];
	auto lval = convert_ast_var(val);
	var->children.push_back(lval);
	
	file->code->children.push_back(var);
}

//Builds structure field access
LtacVar *LTAC_Builder::build_struct_acc(AstNode *node) {
	auto *acc = static_cast<AstStructAcc *>(node);
	auto str_name = acc->str_name;
	auto var_name = acc->var_name;
	Var sv;
	
	for (auto v : struct_vars) {
		if (v.struct_name == str_name && v.name == var_name) {
			sv = v;
			break;
		}
	}
	
	auto *var = new LtacVar;
	var->pos = sv.stack_pos;
	var->d_type = sv.type;
	var->is_ptr = sv.is_ptr;
	return var;
}

