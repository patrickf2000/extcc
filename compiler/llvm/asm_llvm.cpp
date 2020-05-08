#include "asm_llvm.hh"

//Build the data section
void Asm_LLVM::build_data(LtacDataSec *data) {
	for (auto ln : data->children) {
		switch (ln->type) {
			//Floats
			case ltac::Float: {
				auto lf = static_cast<LtacFloat *>(ln);

			} break;
			
			//Doubles
			case ltac::Double: {
				auto ld = static_cast<LtacDouble *>(ln);

			} break;
		
			//Strings
			case ltac::String: {
				auto lstr = static_cast<LtacString *>(ln);
				std::string old_val = lstr->val;
				std::string val = "";
				int len = 0;
				
				//Check for special escape sequences
				for (int i = 0; i<old_val.length(); i++) {
					if (old_val[i] == '\\') {
						if (old_val[i+1] == 'n') {
							++i;
							++len;
							val += "\\0A";
							continue;
						}
					}
					
					val += old_val[i];
					++len;
				}
				
				//Add the null characters
				val += "\\00";
				++len;
				
				//Write the code
				writer << "@" << lstr->name << " = internal constant [";
				writer << len << " x i8] c\"" << val;
				writer << "\"" << std::endl;
				
				strings[lstr->name] = len;
			} break;
		}
	}
}

//Build the code section
void Asm_LLVM::build_code(LtacCodeSec *code) {
	for (auto ln : code->children) {
		switch (ln->type) {
			case ltac::Extern: build_extern(ln); break;
			case ltac::Func: build_func(ln); break;
			case ltac::Ret: build_ret(ln); break;
			
			case ltac::PushArg: build_pusharg(ln); break;
			case ltac::FuncCall: build_func_call(ln); break;
			
			case ltac::Var: build_var(ln); break;
		}
	}
}

//Converts a datatype to an LLVM type
std::string Asm_LLVM::type2str(DataType t) {
	switch (t) {
		case DataType::Int: return "i32";
		case DataType::Str: return "i8*";
		case DataType::Void: return "void";
		case DataType::Any: return "...";
	}
	
	return "";
}


