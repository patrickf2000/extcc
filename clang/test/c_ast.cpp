#include <iostream>
#include <cstdlib>

#include <c_parser.hh>
#include <ast.hh>

int main(int argc, char *argv[]) {
	if (argc == 1) {
		std::cout << "Error: No input specified!" << std::endl;
		return 1;
	}
	
	std::string path = argv[1];
	CParser parser(path);
	parser.parse();
	
	AstNode *top = parser.getTree();
	print_tree(top);
	
	delete top;
	return 0;
}

