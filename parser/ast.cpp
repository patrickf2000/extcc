#include <iostream>

#include "ast.hh"

//Returns an AST type as a string (debugging purposes)
std::string ast2str(AstType type) {
	switch (type) {
		case AstType::Scope: return "Scope";
		case AstType::ExternFunc:
		case AstType::FuncDec: return "FuncDec";
		case AstType::FuncCall: return "FuncCall";
		case AstType::Return: return "Return";
		case AstType::End: return "End";
		case AstType::VarDec: return "VarDec";
		case AstType::VarAssign: return "VarAssign";
		case AstType::Math: return "Math";
		case AstType::Int: return "Int";
		case AstType::Hex: return "Hex";
		case AstType::Char: return "Char";
		case AstType::Bool: return "Bool";
		case AstType::Float: return "Float";
		case AstType::Id: return "ID";
		case AstType::Str: return "Str";
		
		case AstType::ArrayDec: return "ArrayDec";
		case AstType::ArrayAccess: return "ArrayAccess";
		case AstType::ArrayAssign: return "ArrayAssign";
		
		case AstType::Struct: return "Struct";
		case AstType::StructDec: return "StructDec";
		case AstType::StructAcc: return "StructAcc";
		case AstType::StructMod: return "StructMod";
		
		case AstType::Double: return "Double";
				
		case AstType::If: return "If";
		case AstType::Elif: return "Elif";
		case AstType::Else: return "Else";
		case AstType::EndIf: return "EndIf";
		case AstType::While: return "While";
		case AstType::Loop: return "Loop";
		case AstType::ForEach: return "ForEach";
		case AstType::For: return "For";
		
		case AstType::Add: return "OP: +";
		case AstType::Sub: return "OP: -";
		case AstType::Mul: return "OP: *";
		case AstType::Div: return "OP: /";
		case AstType::Mod: return "OP: %";
		case AstType::Inc: return "OP: ++";
		case AstType::DMul: return "OP: **";
		
		case AstType::And: return "And";
		case AstType::Or: return "Or";
		case AstType::Xor: return "Xor";
	}
	
	return "NONE";
}

//Returns a data type as a string (debugging purposes)
std::string type2str(DataType type) {
	switch (type) {
		case DataType::Void: return "Void";
	
		case DataType::Byte: return "Byte";
		case DataType::Char: return "Char";
		case DataType::Short: return "Short";
		case DataType::Int: return "Int";
		case DataType::Float: return "Float";
		case DataType::Bool: return "Bool";
		case DataType::Str: return "Str";
		
		case DataType::Double: return "Double";
	}
	
	return "NONE";
}

//Returns an operator to string (debugging)
std::string op2str(CondOp op) {
	switch (op) {
		case CondOp::Equals: return "==";
		case CondOp::NotEquals: return "!=";
		case CondOp::Greater: return ">";
		case CondOp::GreaterEq: return ">=";
		case CondOp::Less: return "<";
		case CondOp::LessEq: return "<=";
	}
	
	return "NONE";
}

//Prints an AST tree
void print_tree(AstNode *node, int indent, bool nl) {
	for (int i = 0; i<indent; i++) {
		std::cout << "  ";
	}
	std::cout << ast2str(node->type);
	
	if (node->type == AstType::Scope) {
		AstScope *scope = dynamic_cast<AstScope *>(node);
		
		std::cout << " [" << scope->get_name() << "]";
		
		if (scope->vars.size() > 0) {
			std::cout << " <";
			for (auto v : scope->vars) {
				std::cout << "{" << v.second.name << "} ";
			}
			std::cout << ">";
		}
	} else if (node->type == AstType::FuncDec || node->type == AstType::ExternFunc) {
		AstFuncDec *fd = dynamic_cast<AstFuncDec *>(node);
		if (node->type == AstType::ExternFunc) {
			std::cout << " EXT";
		}
		
		if (fd->is_global)
			std::cout << " GLOBAL";
		
		std::cout << " ["  << fd->get_name() << "] ";
			
		if (fd->args.size() > 0) {
			std::cout << " <";
			for (auto v : fd->args) {
				std::cout << "{" << v.name;
				if (v.is_array)
					std::cout << "[]";
				else if (v.is_ptr)
					std::cout << "*";
				std::cout << ":" << type2str(v.type);
				std::cout << "} ";
			}
			std::cout << ">";
		}
	} else if (node->type == AstType::FuncCall) {
		AstFuncCall *fc = dynamic_cast<AstFuncCall *>(node);
		std::cout << " [" << fc->get_name() << "]";
		
	} else if (node->type == AstType::Return) {
		auto ret = static_cast<AstReturn *>(node);
		std::cout << " (" << type2str(ret->d_type) << ") ";
		
	} else if (node->type == AstType::VarDec || node->type == AstType::VarAssign) {
		AstVarDec *vd = dynamic_cast<AstVarDec *>(node);
		std::cout << " ";
		if (vd->is_ptr) std::cout << " *";
		std::cout << "[" << vd->get_name() << "] ("
			<< type2str(vd->get_type()) << ")";
			
	} else if (node->type == AstType::If || node->type == AstType::Elif 
		|| node->type == AstType::While || node->type == AstType::For) {
		AstCond *cond = dynamic_cast<AstCond *>(node);
		std::cout << " <" << op2str(cond->get_op()) << "> [{" << std::endl;
		print_tree(cond->lval, indent+4, true);
		for (int i = 0; i<indent+8; i++)
			std::cout << " ";
		std::cout << "} {" << std::endl;
		print_tree(cond->rval, indent+4, true);
		for (int i = 0; i<indent+8; i++)
			std::cout << " ";
		std::cout << "}]";
		
	} else if (node->type == AstType::ArrayDec) {
		AstArrayDec *arr = dynamic_cast<AstArrayDec *>(node);
		std::cout << " [" << arr->get_name() << "]";
		std::cout << " {" << arr->get_size() << ":";
		std::cout << type2str(arr->get_type()) << "}";
		
	} else if (node->type == AstType::ArrayAccess || node->type == AstType::ArrayAssign) {
		AstVarDec *acc = dynamic_cast<AstVarDec *>(node);
		std::cout << " [" << acc->get_name() << ":" << type2str(acc->get_type()) << "]";
		
		if (node->type == AstType::ArrayAssign) {
			auto assign = dynamic_cast<AstArrayAssign *>(node);
			std::cout << " {";
		
			auto index = assign->index;
			switch (index->type) {
				case AstType::Id: {
					std::cout << dynamic_cast<AstID *>(index)->get_name();
				} break;
				
				case AstType::Int: {
					std::cout << dynamic_cast<AstInt *>(index)->get_val();
				}
			}
			
			std::cout << "}";
		}
		
	} else if (node->type == AstType::Loop) {
		AstLoop *lp = static_cast<AstLoop *>(node);
		
		if (lp->param == nullptr) {
			std::cout << " {INF}";
		} else {
			switch (lp->param->type) {
				case AstType::Int: {
					auto i = static_cast<AstInt* >(lp->param)->get_val();
					std::cout << " {" << ast2str(lp->param->type) << ":" << i << "}";
				} break;
				
				case AstType::Id: {
					auto name = static_cast<AstID *>(lp->param)->get_name();
					std::cout << " {" << ast2str(lp->param->type) << ":" << name << "}";
				} break;
			}
		}
		
	} else if (node->type == AstType::ForEach) {
		AstForEach *fe = static_cast<AstForEach *>(node);
		
		std::cout << " {" << fe->i_var << " : " << fe->r_var << "}";
		
	} else if (node->type == AstType::StructDec) {
		AstStructDec *strd = static_cast<AstStructDec *>(node);
		std::cout << " (" << strd->get_name() << ") {";
		
		for (auto v : strd->fields) {
			std::cout << type2str(v.type) << " ";
			if (v.is_ptr) std::cout << "*";
			std::cout << v.name << ", ";
		}
		std::cout << "}";
		
	} else if (node->type == AstType::Struct || node->type == AstType::StructAcc) {
		AstStruct *s = static_cast<AstStruct *>(node);
		std::cout << " {" << s->str_name << ":" << s->var_name << "}";
		
	} else if (node->type == AstType::StructMod) {
		auto *s = static_cast<AstStructMod *>(node);
		std::cout << " {" << s->str_name << ":" << s->var_name << "}";
		
	//values
	} else if (node->type == AstType::Int) {
		std::cout << " " << dynamic_cast<AstInt *>(node)->get_val();
	} else if (node->type == AstType::Hex) {
		std::cout << " 0x" << std::hex << static_cast<AstHex *>(node)->get_val();
	} else if (node->type == AstType::Char) {
		std::cout << " " << dynamic_cast<AstChar *>(node)->get_val();
	} else if (node->type == AstType::Bool) {
		std::cout << " " << static_cast<AstBool *>(node)->get_val();
	} else if (node->type == AstType::Float) {
		std::cout << " " << dynamic_cast<AstFloat *>(node)->get_val();
	} else if (node->type == AstType::Double) {
		std::cout << " " << dynamic_cast<AstDouble *>(node)->get_val();
	} else if (node->type == AstType::Id) {
		auto id = dynamic_cast<AstID *>(node);
		std::cout << " ";
		if (id->is_ref) std::cout << "&";
		else if (id->is_ptr) std::cout << "*";
		
		std::cout << id->get_name();
	} else if (node->type == AstType::Str) {
		std::cout << " " << dynamic_cast<AstString *>(node)->get_val();
	}
	
	if (nl)
		std::cout << std::endl;

	if (node->children.size() > 0) {
		for (auto c : node->children) {
			print_tree(c, indent+1);
		}
	}
}
