#include "asm_x64.hh"
#include "registers.hh"

//Build a register load
void Asm_x64::build_reg(LtacNode *node) {
	auto reg = static_cast<LtacReg *>(node);
	auto src = reg->children[0];
	int pos = reg->pos - 1;
	
	switch (src->type) {
		//Variable
		case ltac::Var: {
			auto var = static_cast<LtacVar *>(src);
		
			switch (reg->rtype) {
				//General purpose registers
				case RegType::Gp: {
					writer << "\tmov " << registers32[pos] << ", ";
					writer << "[rbp-" << var->pos << "]" << std::endl;
				} break;
				
				//Floating-point registers
				case RegType::Flt: {
					writer << "\tmovss " << float_registers[pos] << ", ";
					writer << "[rbp-" << var->pos << "]" << std::endl;
				} break;

				//Double registers
				case RegType::Flt64: {
					writer << "\tmovsd " << float_registers[pos] << ", ";
					writer << "[rbp-" << var->pos << "]" << std::endl;
				} break;
			}
			
		} break;
		
		//Integer
		case ltac::Int: {
			auto i = static_cast<LtacInt *>(src);
			
			writer << "\tmov " << registers32[pos] << ", ";
			writer << i->val << std::endl;
		} break;
		
		//TODO: add rest
	}
}

//Build a variable declaration
void Asm_x64::build_var(LtacNode *node) {
	if (node->children.size() == 0)
		return;
		
	auto var = static_cast<LtacVar *>(node);
	auto src = node->children[0];
	
	switch (src->type) {
		//Character
		case ltac::Byte: {
			writer << "\tmov BYTE PTR [rbp-";
			writer << var->pos << "], ";
			
			auto li = static_cast<LtacInt *>(src);
			writer << li->val << std::endl;
		} break;
	
		//Integers
		case ltac::Int: {
			if (var->rvar == -1) {
				writer << "\tmov DWORD PTR [rbp-";
				writer << var->pos << "], ";
			} else {
				writer << "\tmov " << var_regs[var->rvar] << ", ";
			}
			
			auto li = static_cast<LtacInt *>(src);
			writer << li->val << std::endl;
		} break;
		
		//Floats
		case ltac::Float: {
			auto lf = static_cast<LtacFloat *>(src);
		
			writer << "\tmovss xmm0, DWORD PTR ";
			writer << lf->name << "[rip]" << std::endl;
			
			writer << "\tmovss DWORD PTR [rbp-";
			writer << var->pos << "], xmm0" << std::endl;
		} break;
		
		//Doubles
		case ltac::Double: {
			auto ld = static_cast<LtacDouble *>(src);
			
			writer << "\tmovsd xmm0, QWORD PTR ";
			writer << ld->name << "[rip]" << std::endl;
			
			writer << "\tmovsd QWORD PTR [rbp-";
			writer << var->pos << "], xmm0" << std::endl;
		} break;
		
		//Strings
		case ltac::String: {
			writer << "\tmov QWORD PTR [rbp-";
			writer << var->pos << "], ";
			writer << "OFFSET FLAT:";
			
			auto lstr = static_cast<LtacString *>(src);
			writer << lstr->name << std::endl;
		} break;
		
		//Registers
		case ltac::Reg: {
			auto reg = static_cast<LtacReg *>(src);
			int pos = reg->pos - 1;
		
			switch (reg->rtype) {
				//General purpose
				case RegType::Gp: {
					writer << "\tmov DWORD PTR [rbp-" << var->pos;
					writer << "], " << registers32[pos] << std::endl;
				} break;
				
				//Return register
				case RegType::Ret: {
					//Pointers use RAX
					if (var->is_ptr) {
						writer << "\tmov QWORD PTR [rbp-" << var->pos;
						writer << "], rax" << std::endl;
						
					//Floating-point uses XMM0
					} else if (var->d_type == DataType::Float 
							|| var->d_type == DataType::Double) {
						writer << "\tmovss [rbp-" << var->pos << "], xmm0";
						writer << std::endl;
					
					//All else uses EAX
					} else {
						writer << "\tmov DWORD PTR [rbp-" << var->pos;
						writer << "], eax" << std::endl;
					}
				} break;
				
				//Floating-point
				case RegType::Flt: {
					writer << "\tmovss DWORD PTR [rbp-" << var->pos;
					writer << "], " << float_registers[pos] << std::endl;
				} break;

				//Double
				case RegType::Flt64: {
					writer << "\tmovsd QWORD PTR [rbp-" << var->pos;
					writer << "], " << float_registers[pos] << std::endl;
				} break;
			}
			
		} break;
		
		//Variables
		case ltac::Var: {
			auto var2 = static_cast<LtacVar *>(src);
			writer << std::endl;
			
			switch (var->d_type) {
				//Integers
				case DataType::Int: {
					if (var2->rvar == -1) {
						writer << "\tmov eax, [rbp-" << var2->pos;
						writer << "]" << std::endl;
					} else {
						writer << "\tmov eax, " << var_regs[var2->rvar] << std::endl;
					}
					
					if (var->rvar == -1) {
						writer << "\tmov DWORD PTR [rbp-";
						writer << var->pos << "], eax" << std::endl;
					} else {
						writer << "\tmov " << var_regs[var->rvar] << ", eax" << std::endl;
					}
				} break;
				
				//Floats
				case DataType::Float: {
					writer << "\tmovss xmm0, [rbp-" << var2->pos;
					writer << "]" << std::endl;
					
					writer << "\tmovss [rbp-";
					writer << var->pos << "], xmm0" << std::endl;
				} break;
				
				//Doubles
				case DataType::Double: {
					writer << "\tmovsd xmm0, [rbp-" << var2->pos;
					writer << "]" << std::endl;
					
					writer << "\tmovsd [rbp-";
					writer << var->pos << "], xmm0" << std::endl;
				} break;
				
				//128-bit vector types
				case DataType::Int128:
				case DataType::Float128: {
					writer << "\tmovups xmm0, [rbp-" << var2->pos;
					writer << "]" << std::endl;
					
					writer << "\tmovups [rbp-";
					writer << var->pos << "], xmm0" << std::endl;
				} break;
				
				//256-bit vector types
				case DataType::Int256:
				case DataType::Float256: {
					writer << "\tvmovups ymm0, [rbp-" << var2->pos;
					writer << "]" << std::endl;
					
					writer << "\tvmovups [rbp-";
					writer << var->pos << "], ymm0" << std::endl;
				} break;
			}
		} break;
		
		//Math expressions
		case ltac::Math: {
			switch (var->d_type) {
				case DataType::Int: build_int_math(var, src); break;
				case DataType::Float: build_float_math(var, src); break;
				case DataType::Int128:
				case DataType::Int256:
				case DataType::Float128:
				case DataType::Float256: build_vector_math(var, src); break;
			}
		} break;
		
		//Function calls
		case ltac::FuncCall: {
			build_func_call(src);
			
			if (var->is_ptr) {
				writer << "\tmov QWORD PTR [rbp-";
				writer << var->pos << "], rax" << std::endl;
			} else {
				writer << "\tmov DWORD PTR [rbp-";
				writer << var->pos << "], eax" << std::endl;
			}
		} break;
		
		//Array access
		case ltac::ArrayAcc: {
			build_array_acc(src);
			
			switch (var->d_type) {
				//Ints
				case DataType::Int: {
					writer << "\tmov DWORD PTR [rbp-";
					writer << var->pos << "], ebx" << std::endl;
				} break;
				
				//Floats
				case DataType::Float: {
					writer << "\tmovss DWORD PTR [rbp-";
					writer << var->pos << "], xmm1" << std::endl;
				} break;
			}
			
			writer << std::endl;
		} break;
	}
}

//Builds integer math expressions
void Asm_x64::build_int_math(LtacTypeNode *var, LtacNode *src, bool store) {
	auto math = static_cast<LtacMath *>(src);
	
	//Build the first value
	auto first = math->init_val;
	
	switch (first->type) {
		//An integer
		case ltac::Int: {
			auto i = static_cast<LtacInt *>(first);
			writer << "\tmov eax, " << std::to_string(i->val) << std::endl;
		} break;
		
		//A variable
		case ltac::Var: {
			auto id = static_cast<LtacVar *>(first);
			
			if (id->rvar == -1) {
				writer << "\tmov eax, [rbp-" << std::to_string(id->pos);
				writer << "]" << std::endl;
			} else {
				writer << "\tmov eax, " << var_regs[id->rvar] << std::endl;
			}
		} break;
		
		//A function call
		case ltac::FuncCall: {
			build_func_call(first);
		} break;
	}
	
	//Build the other parts
	for (auto node : math->children) {
		auto math_op = static_cast<LtacMathOp *>(node);
		bool is_mod = false;
		
		//See if we have a function call
		if (math_op->operand->type == ltac::FuncCall) {
			writer << "\tpush rax" << std::endl;
			build_func_call(math_op->operand);
			writer << "\tpop rbx" << std::endl;
		}
		
		//Build the operator
		switch (math_op->op) {
			case Operator::Add: writer << "\tadd eax, "; break;
			case Operator::Sub: writer << "\tsub eax, "; break;
			case Operator::Mul: writer << "\timul eax, "; break;
			case Operator::Div: {
				writer << "\tcdq" << std::endl;
				writer << "\tidiv "; 
			} break;
			case Operator::Mod: {
				writer << "\tcdq" << std::endl;
				writer << "\tidiv ";
				is_mod = true;
			} break;
		}
		
		//Build the operand
		switch (math_op->operand->type) {
			case ltac::Int: {
				auto i = static_cast<LtacInt *>(math_op->operand);
				writer << std::to_string(i->val) << std::endl;
			} break;
			
			case ltac::Var: {
				auto id = static_cast<LtacVar *>(math_op->operand);
				writer << "DWORD PTR ";
				writer << "[rbp-" << std::to_string(id->pos) << "]";
				writer << std::endl;
			} break;
			
			case ltac::FuncCall: {
				writer << "ebx" << std::endl;
			} break;
		}
		
		if (is_mod)
			writer << "\tmov eax, edx" << std::endl;
	}
	
	//Check to see if we are storing the result
	if (!store)
		return;
		
	//Save the result back to the variable
	if (var->type != ltac::Var)
		return;
		
	auto var2 = static_cast<LtacVar *>(var);
	
	if (var2->rvar == -1) {
		writer << "\tmov DWORD PTR [rbp-" << std::to_string(var2->pos);
		writer << "], eax" << std::endl << std::endl;
	} else {
		writer << "\tmov " << var_regs[var2->rvar] << ", eax" << std::endl;
		writer << std::endl;
	}
}

//Builds floating-point math expressions
void Asm_x64::build_float_math(LtacVar *var, LtacNode *src, bool store) {
	auto math = static_cast<LtacMath *>(src);
	
	//Build the first value
	auto first = math->init_val;
	std::string ln = "";
	
	switch (first->type) {
		//A float
		case ltac::Float: {
			auto i = static_cast<LtacFloat *>(first);
			ln = "\tmovss xmm0, [" + i->name + "]";
		} break;
		
		//A variable
		case ltac::Var: {
			auto id = static_cast<LtacVar *>(first);
			
			if (id->d_type == DataType::Int) {
				writer << "\tcvtsi2ss xmm0, DWORD PTR [rbp-";
				writer << std::to_string(id->pos) << "]" << std::endl;
			} else {
				ln = "\tmovss xmm0, [rbp-" + std::to_string(id->pos) + "]";
			}
		} break;
	}
	
	writer << ln << std::endl;
	
	//Build the other parts
	for (auto node : math->children) {
		auto math_op = static_cast<LtacMathOp *>(node);
		ln = "";
		
		//Build the operator
		switch (math_op->op) {
			case Operator::Add: ln = "\taddss xmm0, "; break;
			case Operator::Sub: ln = "\tsubss xmm0, "; break;
			case Operator::Mul: ln = "\tmulss xmm0, "; break;
			case Operator::Div: ln = "\tdivss xmm0, "; break;
		}
		
		//Build the operand
		switch (math_op->operand->type) {
			case ltac::Float: {
				auto i = static_cast<LtacFloat *>(math_op->operand);
				ln += "[" + i->name + "]";
			} break;
			
			case ltac::Var: {
				auto id = static_cast<LtacVar *>(math_op->operand);
				
				if (id->d_type == DataType::Int) {
					writer << "\tcvtsi2ss xmm2, [rbp-";
					writer << std::to_string(id->pos) << "]" << std::endl;
					
					ln += "xmm2";
				} else {
					ln += "DWORD PTR [rbp-" + std::to_string(id->pos) + "]";
				}
			} break;
		}
		
		writer << ln << std::endl;
	}
	
	//Save the result back to the variable
	if (store) {
		writer << "\tmovss DWORD PTR [rbp-" << std::to_string(var->pos);
		writer << "], xmm0" << std::endl << std::endl;
	}
}

//Builds vector integer math
void Asm_x64::build_vector_math(LtacVar *var, LtacNode *src) {
	auto math = static_cast<LtacMath *>(src);
	
	//Determine if we are using 256-bit registers
	bool islong = false;
	if (var->d_type == DataType::Int256 || var->d_type == DataType::Float256)
		islong = true;
	
	//Print the first value
	auto first = math->init_val;
	
	switch (first->type) {
		//Variables
		case ltac::Var: {
			auto id = static_cast<LtacVar *>(first);
			
			if (islong)
				writer << "\tvmovups ymm0, ";
			else
				writer << "\tmovups xmm0, ";
			
			writer << "[rbp-" << std::to_string(id->pos);
			writer << "]" << std::endl;
		} break;
		
		//TODO: Add rest (we support constants)
	}
	
	//Build the rest of the equation
	for (auto node : math->children) {
		auto math_op = static_cast<LtacMathOp *>(node);
		std::string ln = "\t";
		
		//Build the operator
		//Intel is a pain :)
		switch (var->d_type) {
			//Int 128
			case DataType::Int128: {
				switch (math_op->op) {
					case Operator::Add: ln += "phaddd xmm0, "; break;
					case Operator::PAdd: ln += "addps xmm0, "; break;
					case Operator::Sub: ln += "psubb xmm0, "; break;
					case Operator::Mul: ln += "pmulld xmm0, "; break;
				}
			} break;
			
			//Int 256
			case DataType::Int256: {
				switch (math_op->op) {
					case Operator::Add: ln += "vphaddd ymm0, ymm0, "; break;
					case Operator::PAdd: ln += "vpaddd ymm0, ymm0, "; break;
					case Operator::Sub: ln += "vpsubb ymm0, ymm0, "; break;
					case Operator::Mul: ln += "vpmulld ymm0, ymm0, "; break;
				}
			} break;
			
			//Float 128
			case DataType::Float128: {
				switch (math_op->op) {
					case Operator::Add: ln += "haddps xmm0, "; break;
					case Operator::PAdd: ln += "addps xmm0, "; break;
					case Operator::Sub: ln += "subps xmm0, "; break;
					case Operator::Mul: ln += "mulps xmm0, "; break;
					case Operator::Div: ln += "divps xmm0, "; break;
				}
			} break;
			
			//Float 256
			case DataType::Float256: {
				switch (math_op->op) {
					case Operator::Add: ln += "vhaddps ymm0, ymm0, "; break;
					case Operator::PAdd: ln += "vaddps ymm0, ymm0, "; break;
					case Operator::Sub: ln += "vsubps ymm0, ymm0, "; break;
					case Operator::Mul: ln += "vmulps ymm0, ymm0, "; break;
					case Operator::Div: ln += "vdivps ymm0, ymm0, "; break;
				}
			} break;
		}
		
		//Build the operand
		switch (math_op->operand->type) {
			//Other variables
			case ltac::Var: {
				auto id = static_cast<LtacVar *>(math_op->operand);
				ln += "[rbp-" + std::to_string(id->pos) + "]";
			} break;
			
			//The index of an array
			case ltac::ArrayAcc: {
				auto acc = static_cast<LtacArrayAcc *>(math_op->operand);
				auto child = acc->children[0];
				
				int pos = acc->pos;
				int size = acc->type_size;
				
				switch (child->type) {
					//Integer access
					case ltac::Int: {
						auto li = static_cast<LtacInt *>(child);
						int offset = pos - (size * li->val);
						
						writer << "\tmovups xmm1, [rbp-" + std::to_string(offset);
						writer << "]" << std::endl;
					} break;
					
					//By-variable access
					case ltac::Var: {
						auto lv = static_cast<LtacVar *>(child);
						writer << "\tmov eax, DWORD PTR [rbp-" << std::to_string(lv->pos);
						writer << "]" << std::endl;
						writer << "\tcdqe" << std::endl;
						writer << "\tmovups xmm1, [rbp-" << std::to_string(pos);
						writer << "+rax*4]" << std::endl;
					} break;
				}
			
				if (islong)
					ln += "ymm1";
				else
					ln += "xmm1";
			} break;
			
			//TODO: Add rest
		}
		
		//Write out the line
		writer << ln << std::endl;
	}
	
	//Save results back to the variable
	std::string mem = "[rbp-" + std::to_string(var->pos) + "]";
	
	if (islong)
		writer << "\tvmovups " << mem << ", ymm0";
	else
		writer << "\tmovups " << mem << ", xmm0";
	
	writer << std::endl << std::endl;
}

