#include <iostream>

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
	if (ret_next) {
		ret_next = false;
		return tNext;
	}

	Token next;
	next.type = 0;
	next.id = "";
	
	//Check to make sure we aren't at the end of the file
	if (reader.eof()) {
		next.type = -1;
		return next;
	}
	
	//Get a character
	char c;
	char sym;
	bool ret_sym = false;
	
	//Get characters until we have a token
	while (!reader.eof()) {
		reader.get(c);
		
		if (isWhitespace(c)) {
			if (current == "")
				continue;
			break;
		} else if (isSeparator(c)) {
			sym = c;
			
			if (current != "") {
				ret_next = true;
				break;
			}
			
			current += c;
			break;
		}
		
		current += c;
	}
	
	if (current == "" && reader.eof()) {
		next.type = -1;
		return next;
	}
	
	if (ret_next) {
		std::string s = "";
		s += sym;
		
		tNext.type = tokens[s];
		tNext.id = s;
	}
	
	if (tokens.find(current) == tokens.end()) {
		next.type = idToken;
	} else {
		next.type = tokens[current];
	}
	
	next.id = current;
	current = "";
	
	return next;
}

//Check to see if we have a separator
bool Scanner::isSeparator(char c) {
	for (char s : separators) {
		if (s == c) return true;
	}
	return false;
}

//Check to see if we have whitespace
bool Scanner::isWhitespace(char c) {
	for (char w : whitespace) {
		if (c == w) return true;
	}
	return false;
}

//Close the scanner
void Scanner::close() {
	reader.close();
}

