#include <iostream>
#include <string>

#include <ast.hh>
#include <ltac/ltac.hh>
#include <ltac/ltac_build.hh>

#ifdef BUILD_CLANG
#include <c_parser.hh>
#endif

int main(int argc, char *argv[]) {
	if (argc == 1) {
		std::cout << "Fatal: No input file." << std::endl;
		return 1;
	}
	
#ifdef BUILD_CLANG
		CParser parser(argv[1]);
		parser.parse();
		AstNode *top = parser.getTree();
#else
#error Unknown Compiler
#endif
	
	LTAC_Builder *builder = new LTAC_Builder;
	LtacFile *file = builder->build_file(top);
	
	print_ltac(file);
	
	delete builder;
	delete file;
	delete top;
	
	return 0;
}
