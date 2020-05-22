#include "asm_x64.hh"

//Opens the file
X64::X64(std::string name) {
	writer = std::ofstream(name);
	
	writer << ".intel_syntax noprefix" << std::endl;
}

//Close out the file
void X64::write() {
	writer.close();
}
