#include "ltac_build.hh"

//Build a variable declaration
void LTAC_Builder::build_var_dec(AstNode *node) {
	auto vd = static_cast<AstVarDec *>(node);

	Var v;
	v.name = vd->get_name();
	v.type = vd->get_type();
	v.is_array = false;
	v.is_param = false;
	v.is_ptr = false;
	
	if (v.type == DataType::Int128 || v.type == DataType::Int256
		|| v.type == DataType::Float128 || v.type == DataType::Float256) {
		v.is_array = true;	
	}
	
	if (vd->is_ptr) {
		stack_pos += 8;
		v.is_ptr = true;
	} else {
		inc_stack(v.type);
	}
	
	v.stack_pos = stack_pos;
	vars[v.name] = v;
}

//Build a variable assignment
void LTAC_Builder::build_var_assign(AstNode *node) {
	auto va = static_cast<AstVarAssign *>(node);
	auto v = vars[va->get_name()];
	
	if (v.type == DataType::Int128 || v.type == DataType::Int256
		|| v.type == DataType::Float128 || v.type == DataType::Float256) {
		if (va->children[0]->type != AstType::Math && va->children.size() > 1) {
			build_vector_dec(va);	
			return;
		}
	}
	
	auto var = new LtacVar;
	var->pos = v.stack_pos;
	var->d_type = v.type;
	var->is_ptr = v.is_ptr;
	
	//Build the assigned value
	auto val = va->children[0];
	
	switch (val->type) {
		//Increment
		case AstType::Inc: {
			auto math = new LtacIMath;
			math->op = Operator::Add;
			math->children.push_back(var);
			math->children.push_back(new LtacInt(1));
			file->code->children.push_back(math);
		} break;
		
		//Math
		case AstType::Math: {
			if (v.type == DataType::Int) {
				build_int_math(var, val);
			} else {
				//TODO: Remove me
				auto lval = convert_ast_var(val);
				var->children.push_back(lval);
				file->code->children.push_back(var);
			}
		} break;
	
		default: {
			auto lval = convert_ast_var(val);
			var->children.push_back(lval);
			file->code->children.push_back(var);
		}
	}
}

//Build a multi-var variable assignment
void LTAC_Builder::build_multi_var_assign(AstNode *node) {
	auto mva = static_cast<AstMultiVarAssign *>(node);
	auto fc = build_func_call(mva->children[0]);
	
	for (auto var : mva->vars) {
		auto id = static_cast<AstID *>(var);
		auto v = vars[id->get_name()];
		
		auto lvar = new LtacVar;
		lvar->pos = v.stack_pos;
		lvar->d_type = v.type;
		
		fc->ret_dest.push_back(lvar);
	}
	
	file->code->children.push_back(fc);
}

//Converts an AST node to an LTAC node
LtacNode *LTAC_Builder::convert_ast_var(AstNode *val) {
	LtacNode *lval = new LtacNode;

	switch (val->type) {
		//Bytes
		case AstType::Hex: {
			auto ch = static_cast<AstHex *>(val);
			int c = (int)ch->get_val();
			
			lval = new LtacByte(c);
		} break;
	
		//Chars
		case AstType::Char: {
			auto ch = static_cast<AstChar *>(val);
			int c = (int)ch->get_val();
			
			lval = new LtacByte(c);
		} break;
	
		//Booleans
		case AstType::Bool: {
			auto bl = static_cast<AstBool *>(val);
			lval = new LtacInt((int)bl->get_val());
		} break;
	
		//Integers
		case AstType::Int: {
			auto i = static_cast<AstInt *>(val);
			lval = new LtacInt(i->get_val());
		} break;
		
		//Floats
		case AstType::Float: lval = build_float(val); break;
		
		//Double
		case AstType::Double: lval = build_double(val); break;
		
		//Strings
		case AstType::Str: lval = build_string(val); break;
		
		//Other variables
		case AstType::Id: {
			auto id = static_cast<AstID *>(val);
			auto v2 = vars[id->get_name()];
			
			auto l_id = new LtacVar;
			l_id->pos = v2.stack_pos;
			l_id->d_type = v2.type;
			l_id->is_ref = id->is_ref;
			l_id->is_ptr = v2.is_ptr;
			lval = l_id;
		} break;
		
		//Function class
		case AstType::FuncCall: lval = build_func_call(val); break;
		
		//Array access
		case AstType::ArrayAccess: lval = build_array_acc(val); break;
		
		//Structure access
		case AstType::StructAcc: lval = build_struct_acc(val); break;
		
		//Math
		case AstType::Math: {
			auto math = new LtacMath;
			math->init_val = convert_ast_var(val->children[0]);
			
			for (int i = 1; i<val->children.size(); i+=2) {
				auto math_op = new LtacMathOp;
			
				auto op = val->children[i];
				auto current = val->children[i+1];
				
				//Build the operator
				switch (op->type) {
					case AstType::Add: math_op->op = Operator::Add; break;
					case AstType::Sub: math_op->op = Operator::Sub; break;
					case AstType::Mul: math_op->op = Operator::Mul; break;
					case AstType::Div: math_op->op = Operator::Div; break;
					case AstType::Mod: math_op->op = Operator::Mod; break;
					
					case AstType::Inc: math_op->op = Operator::PAdd; break;
				}
				
				//Build the operand
				if (current->type == AstType::Math) {
					math_op->operand = new LtacNode;
					math->children.push_back(math_op);
					
					math->children.push_back(new LtacNode(ltac::Push));
					
					auto sub_math = convert_ast_var(current);
					math->children.push_back(sub_math);
					
					math->children.push_back(new LtacNode(ltac::Pop));
				} else {
					math_op->operand = convert_ast_var(current);
					math->children.push_back(math_op);
				}
			}
			
			lval = math;
		} break;
		
		//TODO: Add the rest
	}
	
	return lval;
}

//Builds a vector variable declaration
void LTAC_Builder::build_vector_dec(AstVarAssign *va) {
	auto *vector = new LtacArray;
	vector->pos = stack_pos;
	vector->size = va->children.size();
	vector->type_size = 4;
	vector->d_type = va->get_type();
	
	for (auto child : va->children) {
		auto node = convert_ast_var(child);
		vector->children.push_back(node);
	}
	
	file->code->children.push_back(vector);
}


