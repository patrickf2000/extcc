#include <iostream>
#include <string>

#include <ast.hh>
#include <ltac/ltac.hh>
#include <ltac/ltac_build.hh>

#include <c_parser.hh>
#include <lasm_parser.hh>

void help() {
	std::cout << "Fatal: Missing or invalid command line options" << std::endl;
	std::cout << "CMD: asm <file> <language>" << std::endl;
	std::cout << "Language:" << std::endl;
	std::cout << "\t-c\tC language" << std::endl;
	std::cout << "\t-l\tLTAC Assembly language." << std::endl;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		help();
		return 1;
	}
	
	std::string language = argv[2];
	AstNode *top;
	LtacFile *file;
	
	if (language == "-c") {
		CParser parser(argv[1]);
		parser.parse();
		top = parser.getTree();
		
		LTAC_Builder *builder = new LTAC_Builder;
		file = builder->build_file(top);
		
		delete builder;
	} else if (language == "-l") {
		AsmParser parser(argv[1]);
		parser.parse();
		file = parser.getFile();
	} else {
		help();
		return 1;
	}
	
	print_ltac(file);
	
	return 0;
}
