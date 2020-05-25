#include <iostream>
#include <stdlib.h>

#include <c_parser.hh>
#include <ast.hh>
#include <pasm/pasm.hh>
#include <pasm/pasm_builder.hh>

using namespace PASM;

int main(int argc, char *argv[]) {
	if (argc == 1) {
		std::cout << "Error: No input file." << std::endl;
		return 1;
	}
	
	CParser parser(argv[1]);
	parser.parse();
	AstNode *top = parser.getTree();
	
	auto builder = new PasmBuilder("");
	auto file = builder->buildFile(top);
	
	std::string code = unwrite(file);
	std::cout << code << std::endl;
	
	return 0;
}
