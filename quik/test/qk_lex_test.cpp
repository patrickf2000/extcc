#include <iostream>
#include <string>
#include <cstdlib>

#include <qk_lex.hh>

int main(int argc, char *argv[]) {
	if (argc == 1) {
		std::cout << "Error: No input provided." << std::endl;
		std::exit(1);
	}
	
	std::string input = argv[1];
	QkScanner scan(input);
	int count = 1;
	
	while (true) {
		Token t = scan.getNext();
		if (t.type == -1) break;
		
		std::cout << "[" << count << "] ";
		std::cout << (int)t.type << " -> " << t.id << std::endl;
		
		++count;
	}
	
	scan.close();
	std::cout << "Done" << std::endl;
	
	return 0;
}
