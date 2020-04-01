#include "lex.hh"

//The constructor
Scanner::Scanner(std::string in) {
	path = in;
	
	initSeparators();
	initKeywords();
	initTokens();
	
	reader = std::ifstream(path.c_str());
	if (!reader.is_open()) {
		//TODO: Die
	}
}

//Get's the next token
Token Scanner::getNext() {
	Token next;
	return next;
}

//Close the scanner
void Scanner::close() {
	reader.close();
}
