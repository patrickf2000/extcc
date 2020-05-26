#include <iostream>
#include <cstdlib>

#include "asm_aarch64.hh"

//Opens the file
AArch64::AArch64(std::string name) : CompilerBase(name) {
}

//The main loop to generate code for the data section
void AArch64::build_data(PasmFile *file) {
	writer << ".data" << std::endl;
	
	for (auto ln : file->data) {
		auto str = static_cast<PasmString *>(ln);
		std::string type = "";
		std::string dl = "";
		
		switch (ln->type) {
			//Strings
			case pasm::String: type = ".string"; dl = "\""; break;
			
			//Floats
			case pasm::Float: type = ".long"; break;
			
			//Doubles
			case pasm::Double: type = ".quad"; break;
		}
		
		writer << "\t" << str->name << ": " << type << " " << dl << str->val;
		writer << dl << std::endl;
	}
	
	writer << std::endl;
}

