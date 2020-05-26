#include <iostream>
#include <cctype>

#include "lex.hh"

//The constructor
Scanner::Scanner(std::string in) {
	path = in;
	
	initSeparators();
	initKeywords();
	initTokens();
	
	reader = std::ifstream(path.c_str());
	if (!reader.is_open()) {
		std::cout << "Error: Unable to open input file!" << std::endl;
		std::cout << "Path: " << path << std::endl;
		std::cout << std::endl;
		std::cout << "Please make sure the file exists and you have permission to read." << std::endl;
		std::exit(2);
	}
}

//Get's the next token
Token Scanner::getNext() {
	if (stored.size() > 0) {
		auto t = stored.top();
		stored.pop();
		return t;
	}

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
		current_ln += c;
		
		if (c == '\"' && !in_quote) {
			in_quote = true;
			continue;
		} else if (c == '\'' && !in_squote) {
			in_squote = true;
			continue;
		} else if (c == '\"' && in_quote) {
			in_quote = false;
			
			next.type = strToken;
			next.id = current;
			current = "";
			return next;
		} else if (c == '\'' && in_squote) {
			in_squote = false;
			
			next.type = charToken;
			next.id = current;
			current = "";
			return next;
		} else if (!in_quote) {
			if (c == '.') {
				if (isInt()) {
					current += c;
					continue;
				}
			} 
			
			if (isWhitespace(c)) {
				if (c == '\n') {
					current_ln = "";
					++ln_no;
				}
				
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
	
	if (isFloat()) {
		next.type = fltToken;
	} else if (isInt()) {
		next.type = intToken;
	} else if (tokens.find(current) == tokens.end()) {
		next.type = idToken;
	} else {
		next.type = tokens[current];
	}
	
	next.id = current;
	current = "";
	
	return next;
}

//Returns a token to be returned by nextToken
void Scanner::unget(Token t) {
	stored.push(t);
}

//Returns the rest of the line
//ONLY call this in a fatal error event
void Scanner::finishLine() {
	char c = 0;
	
	while (!reader.eof() && c != '\n') {
		reader.get(c);
		current_ln += c;
	}
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

//Checks to see if the current buffer is an integer
bool Scanner::isInt() {
	for (char c : current) {
		if (!isdigit(c))
			return false;
	}
	return true;
}

//Checks to see if the current buffer is an float
bool Scanner::isFloat() {
	int point_c = 0;
	
	for (char c : current) {
		if (c == '.') {
			++point_c;
			continue;
		}
		
		if (!isdigit(c))
			return false;
	}
	
	if (point_c != 1)
		return false;
		
	return true;
}

//Close the scanner
void Scanner::close() {
	reader.close();
}

