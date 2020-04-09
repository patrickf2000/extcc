#include "asm_arm7.hh"

//Build the data section
void Asm_Arm7::build_data(LtacDataSec *data) {

}

//Build the code section
void Asm_Arm7::build_code(LtacCodeSec *code) {
	for (auto ln : code->children) {
		switch (ln->type) {
			case ltac::Func: build_func(ln); break;
			case ltac::Ret: build_ret(ln); break;
			
			case ltac::Var: build_var(ln); break;
		}
	}
}

