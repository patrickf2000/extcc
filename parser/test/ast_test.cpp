#include <iostream>
#include <cstdlib>

#include <ast.hh>

#include <c_parser.hh>

void help() {
	std::cout << "Fatal: Missing or invalid command line options" << std::endl;
	std::cout << "CMD: asm <file> <language>" << std::endl;
	std::cout << "Language:" << std::endl;
	std::cout << "\t-c\tC language" << std::endl;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		help();
		return 1;
	}
	
	std::string language = argv[2];
	AstNode *top;
	
	if (language == "-c") {
		CParser parser(argv[1]);
		parser.parse();
		top = parser.getTree();
	} else {
		help();
		return 1;
	}
	
	print_tree(top);
	
	delete top;
	return 0;
}

