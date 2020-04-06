#include "ltac_build.hh"
#include <iostream>

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
		v.name = field.name;
		v.type = field.type;
		v.is_ptr = field.is_ptr;
		
		if (field.is_ptr)
			stack_pos += 8;
		else
			inc_stack(v.type);
			
		vars[v.name] = v;
	}
}
