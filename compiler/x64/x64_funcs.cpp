#include "asm_x64.hh"

//Registers for function calls
std::string call_regs[] = {
	"rdi",
	"rsi",
	"rdx",
	"rcx",
	"r8",
	"r9"
};

std::string call_regs32[] = {
	"edi",
	"esi",
	"edx",
	"ecx",
	"r8",
	"r9"
};

std::string call_flt_regs[] = {
	"xmm0",
	"xmm1",
	"xmm2",
	"xmm3",
	"xmm4",
	"xmm5",
	"xmm6",
	"xmm7"
};

std::string ret_regs32[] {
	"eax",
	"r10d",
	"r11d",
	"r12d"
};

std::string ret_regs[] {
	"rax",
	"r10",
	"r11",
	"r12"
};

int call_index = 0;
int call_index_flt = 0;

//Build a function declaration
void Asm_x64::build_func(LtacNode *node) {
	auto fd = static_cast<LtacFunc *>(node);
	
	if (fd->is_extern) return;
	
	if (fd->is_global) {
		writer << ".global " << fd->name << std::endl;
		writer << ".type " << fd->name << ", @function";
		writer << std::endl << std::endl;
	} else if (fd->name == "main") {
		writer << ".global main" << std::endl;
		writer << std::endl;
	}
	
	writer << fd->name << ":" << std::endl;
	
	//Setup the stack
	writer << "\tpush rbp" << std::endl;
	writer << "\tmov rbp, rsp" << std::endl;
	
	int size = fd->stack_size;
	if (size != 0) {
		writer << "\tsub rsp, " << std::to_string(size) << std::endl;
	}
	
	writer << std::endl;
	
	//Retrieve the arguments
	int call_index = 0;
	int call_index_flt = 0;
	
	for (auto a : fd->children) {
		auto arg = static_cast<LtacVar *>(a);
		
		if (arg->is_ptr) {
			writer << "\tmov QWORD PTR [rbp-" << arg->pos;
			writer << "], " << call_regs[call_index] << std::endl;
		} else {
			switch (arg->d_type) {
				//Integers
				case DataType::Int: {
					writer << "\tmov DWORD PTR ";
					if (pic) {
						writer << "-" << arg->pos << "[rbp], ";
					} else {
						writer <<"[rbp-" << arg->pos << "], ";
					}
					writer << call_regs32[call_index] << std::endl;
				} break;
				
				//Strings
				case DataType::Str: {
					writer << "\tmov QWORD PTR ";
					if (pic) {
						writer << "-" << arg->pos << "[rbp], ";
					} else {
						writer <<"[rbp-" << arg->pos << "], ";
					}
					writer << call_regs[call_index] << std::endl;
				} break;
			}
		}
		
		++call_index;
	}
	
	if (fd->children.size() > 0)
		writer << std::endl;
}

//Builds a push argument call
// This pushes an argument to one of the call registers
void Asm_x64::build_push_arg(LtacNode *node, bool is_arg) {
	LtacNode *arg = node;
	
	if (!is_arg)
		arg = node->children[0];
	
	//Build the actual argument
	switch (arg->type) {
		//Raw string arguments
		case ltac::String: {
			auto lstr = static_cast<LtacString *>(arg);
			auto reg = call_regs[call_index];
			++call_index;
			
			if (pic) {
				writer << "\tlea " << reg << ", ";
				writer << lstr->name << "[rip]" << std::endl;
			} else {
				writer << "\tmov " << reg << ", ";
				writer << "OFFSET FLAT:" << lstr->name << std::endl;
			}
		} break;
		
		//TODO: Build the rest
	}
}

//Build a function call
void Asm_x64::build_func_call(LtacNode *node) {
	auto fc = static_cast<LtacFuncCall *>(node);
	
	//Add the arguments
	//TODO: Eventually make the entire thing use the build_push_arg function
	for (auto arg : fc->children) {
		switch (arg->type) {
			//Raw integer arguments
			case ltac::Int: {
				auto li = static_cast<LtacInt *>(arg);
				
				writer << "\tmov " << call_regs32[call_index];
				writer << ", " << li->val << std::endl;
				
				++call_index;
			} break;
		
			//Raw string arguments
			case ltac::String: build_push_arg(arg, true); break;
			
			//Other variables
			case ltac::Var: {
				auto var = static_cast<LtacVar *>(arg);
				
				//Reference variables
				if (var->is_ref) {
					writer << "\tlea " << call_regs[call_index] << ", ";
					writer << "[rbp-" << std::to_string(var->pos) << "]";
					
					writer << std::endl;
					++call_index;
					
				//Pointer variables
				} else if (var->is_ptr) {
					writer << "\tmov " << call_regs[call_index] << ", ";
					writer << "QWORD PTR [rbp-" << std::to_string(var->pos);
					writer << "]" << std::endl;
					
					++call_index;
					
				//Single-precision float variables
				} else if (var->d_type == DataType::Float) {
					std::string reg = call_flt_regs[call_index_flt];
					++call_index_flt;
					
					writer << "\tcvtss2sd xmm0, DWORD PTR [rbp-";
					writer << std::to_string(var->pos) + "]" << std::endl;
					
				//Double-precision float variables
				} else if (var->d_type == DataType::Double) {
					std::string reg = call_flt_regs[call_index_flt];
					++call_index_flt;
					
					writer << "\tmovsd xmm0, QWORD PTR [rbp-";
					writer << std::to_string(var->pos) + "]" << std::endl;
					
				//Other variables- usually integers
				} else {
					std::string instr = "mov";
					std::string prefix = "DWORD PTR";
					std::string reg = call_regs32[call_index];
					
					if (var->d_type == DataType::Str) {
						prefix = "QWORD PTR";
						reg = call_regs[call_index];
					}
					
					if (pic) {
						writer << "\tmov " << reg << ", ";
						writer << prefix << " -" << var->pos;
						writer << "[rbp]";
					} else {
						writer << "\tmov " << reg << ", ";
						writer << prefix << " ";
						writer << "[rbp-" << var->pos << "]";
					}
					
					writer << std::endl;
					++call_index;
				}
			} break;
			
			//Array access
			case ltac::ArrayAcc: {
				build_array_acc(arg);
				auto type = static_cast<LtacVar *>(arg)->d_type;
			
				switch (type) {
					//Ints
					case DataType::Int: {
						writer << "\tmov " << call_regs32[call_index];
						writer << ", ebx" << std::endl;
						++call_index;
					} break;
					
					//Floats and Doubles
					case DataType::Float: 
					case DataType::Double: {
						writer << "\tcvtss2sd " << call_flt_regs[call_index_flt];
						writer << ", xmm1" << std::endl;
						++call_index_flt;
					} break;
				}
			} break;
			
			//TODO: Add the rest
		}
	}
	
	//Tell the function about floating-point arguments
	if (call_index_flt > 0) {
		writer << "\tmov eax, " << call_index_flt << std::endl;
	}
	
	//Call the function
	writer << "\tcall " << fc->name;
	if (pic)
		writer << "@PLT";
	writer << std::endl << std::endl;
	
	//If we have any destination variables to store, do that
	if (fc->ret_dest.size() > 0) {
		int ret_index = 0;
		
		for (auto var : fc->ret_dest) {
			auto reg = ret_regs32[ret_index];
			++ret_index;
		
			writer << "\tmov DWORD PTR [rbp-";
			writer << std::to_string(var->pos) + "], ";
			writer << reg << std::endl;
		}
		
		writer << std::endl;
	}
	
	call_index = 0;
	call_index_flt = 0;
}

//Builds a function return
void Asm_x64::build_ret(LtacNode *node) {
	writer << std::endl;
	int ret_index = 0;
	
	auto *ret = static_cast<LtacRet *>(node);

	for (auto val : ret->children) {
		auto reg = ret_regs32[ret_index];
	
		switch (val->type) {
			//Variables
			case ltac::Var: {
				auto var = static_cast<LtacVar *>(val);
				
				if (var->is_ptr) {
					reg = ret_regs[ret_index];
					
					writer << "\tmov " << reg << ", QWORD PTR ";
					writer << "[rbp-" << var->pos << "]" << std::endl;
				} else {
					writer << "\tmov " << reg << ", [rbp-";
					writer << std::to_string(var->pos) << "]";
					writer << std::endl;
				}
			} break;
			
			//Integers
			case ltac::Int: {
				auto i = static_cast<LtacInt *>(val);
				
				writer << "\tmov " << reg << ", ";
				writer << i->val << std::endl;
			} break;
			
			//Math expressions
			case ltac::Math: {
				switch (ret->d_type) {
					case DataType::Int: build_int_math(ret, val, false); break;
					
					//TODO: Add the rest
				}
			} break;
			
			//TODO: Add the rest
		}
		
		++ret_index;
	}

	writer << std::endl;
	writer << "\tleave" << std::endl;
	writer << "\tret" << std::endl;
	writer << std::endl;
}


