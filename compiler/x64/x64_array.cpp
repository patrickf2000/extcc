#include "asm_x64.hh"

//Builds an array
void Asm_x64::build_array(LtacNode *node) {
	auto array = static_cast<LtacArray *>(node);
	
	int pos = array->pos;
	int size = array->type_size;
	
	for (auto child : array->children) {
		std::string addr = "[rbp-" + std::to_string(pos) + "]";
		pos -= size;
	
		switch (child->type) {
			//Integers
			case ltac::Int: {
				auto i = static_cast<LtacInt *>(child);
			
				writer << "\tmov DWORD PTR " << addr << ", ";
				writer << i->val << std::endl;
			} break;
			
			//Floating point numbers
			case ltac::Float: {
				auto f = static_cast<LtacFloat *>(child);
				
				writer << "\tmovss xmm0, DWORD PTR ";
				writer << f->name << "[rip]" << std::endl;
				writer << "\tmovss DWORD PTR " << addr << ", ";
				writer << "xmm0" << std::endl;
			} break;
			
			//TODO: Add the rest
		}
	}
	
	writer << std::endl;
}

//Builds an array access
void Asm_x64::build_array_acc(LtacNode *node) {
	auto acc = static_cast<LtacArrayAcc *>(node);
	auto child = acc->children[0];
	
	int pos = acc->pos;
	int size = acc->type_size;
	
	switch (child->type) {
		//Integer index
		case ltac::Int: {
			auto li = static_cast<LtacInt *>(child);
			int offset = (size * li->val);
			
			switch (acc->d_type) {
				//Integer arrays
				case DataType::Int: {
					writer << "\tmov rbx, [rbp-" << pos;
					writer << "]" << std::endl;
					writer << "\tmov ebx, [rbx+" << offset;
					writer << "]" << std::endl;
				} break;
				
				//Float arrays
				case DataType::Float: {
					writer << "\tmovss xmm1, [rbp-" << offset;
					writer << "]" << std::endl;
				} break;
			}
		} break;
		
		//Variable index
		case ltac::Var: {
			auto lv = static_cast<LtacVar *>(child);
			
			if (acc->is_ptr) {
				writer << "\tmov eax, DWORD PTR [rbp-" << lv->pos;
				writer << "]" << std::endl;
				writer << "\tcdqe" << std::endl;
				
				writer << "\tlea rdx, [0+rax*4]" << std::endl;
				writer << "\tmov rax, QWORD PTR [rbp-" << pos;
				writer << "]" << std::endl;
				writer << "\tadd rax, rdx" << std::endl;
			} else {
				writer << "\tmov eax, DWORD PTR [rbp-" << lv->pos;
				writer << "]" << std::endl;
				writer << "\tcdqe" << std::endl;
			}
			
			switch (acc->d_type) {
				//Integer arrays
				case DataType::Int: 
				case DataType::Int128:
				case DataType::Int256: {
					if (acc->is_ptr) {
						writer << "\tmov ebx, [rax]" << std::endl;
					} else {
						writer << "\tmov ebx, DWORD PTR [rbp-" << pos << "+rax*";
						writer << size << "]" << std::endl;
					}
				} break;
				
				//Float arrays
				case DataType::Float: 
				case DataType::Float128:
				case DataType::Float256: {
					if (acc->is_ptr) {
						writer << "\tmovss xmm1, DWORD PTR [rax]" << std::endl;
					} else {
						writer << "\tmovss xmm1, DWORD PTR [rbp-" << pos << "+rax*";
						writer << size << "]" << std::endl;
					}
				} break;
			}
		} break;
	}
}

//Build an array-set statement
void Asm_x64::build_array_set(LtacNode *node) {
	auto *arr = static_cast<LtacArraySet *>(node);
	
	int pos = arr->pos;
	int size = arr->type_size;
	
	switch (arr->index->type) {
		//Set by integer
		case ltac::Int: {
			auto *li = static_cast<LtacInt *>(arr->index);
			int offset = li->val * size;
			
			writer << "\tmov r9, QWORD PTR [rbp-";
			writer << std::to_string(pos) << "]" << std::endl;
			writer << "\tmov r9, [r9+" << offset << "]" << std::endl;
		} break;
		
		//Set by variable
		case ltac::Var: {
			auto *var = static_cast<LtacVar *>(arr->index);
			
			writer << "\tmov eax, DWORD PTR [rbp-";
			writer << std::to_string(var->pos) << "]" << std::endl;
			
			writer << "\tcdqe" << std::endl;
			writer << "\tlea rdx, [0+rax*4]" << std::endl;
			writer << "\tmov r9, QWORD PTR [rbp-" << std::to_string(pos);
			writer << "]" << std::endl;
			writer << "\tadd r9, rdx" << std::endl;
		} break;
	}
	
	//Build the operands
	auto src = arr->children[0];
	
	switch (src->type) {
		//Integers
		case ltac::Int: {
			auto *li = static_cast<LtacInt *>(src);
			
			writer << "\tmov DWORD PTR [r9], ";
			writer << std::to_string(li->val) << std::endl;
		} break;
	
		//Other variables
		case ltac::Var: {
			auto *var = static_cast<LtacVar *>(src);
			
			if (var->d_type == DataType::Int) {
				writer << "\tmov eax, [rbp-" << std::to_string(var->pos);
				writer << "]" << std::endl;
				writer << "\tmov DWORD PTR [r9], eax" << std::endl;
			} else if (var->d_type == DataType::Float) {
				writer << "\tmovss xmm0, [rbp-" << std::to_string(var->pos);
				writer << "]" << std::endl;
				writer << "\tmovss DWORD PTR [r9], xmm0" << std::endl;
			} else {
				//TODO
			}
		} break;
		
		//Math expressions
		case ltac::Math: {
			switch (arr->d_type) {
				//Integers
				case DataType::Int: {
					build_int_math(arr, src, false);
					
					writer << "\tmov DWORD PTR [r9], eax" << std::endl;
					writer << std::endl;
				} break;
				
				//Single-precision floats
				case DataType::Float: {
					build_float_math(arr, src, false);
					
					writer << "\tmovss [r9], xmm0" << std::endl;
					writer << std::endl;
				} break;
			}
		} break;
		
		//TODO: Add the rest
	}
	
	writer << std::endl;
}

