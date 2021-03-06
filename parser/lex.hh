#pragma once

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <stack>

enum TokenType {

};

struct Token {
	int type;
	std::string id;
};

class Scanner {
public:
	explicit Scanner(std::string in);
	Token getNext();
	void unget(Token t);
	void finishLine();
	bool isSeparator(char c);
	bool isWhitespace(char c);
	bool isInt();
	bool isFloat();
	void close();
	
	int getLnNo() { 
		return ln_no;
	
	}
	std::string getCurrentLn() {
		return current_ln;
	}
protected:
	virtual void initSeparators() {}
	virtual void initKeywords() {}
	virtual void initTokens() {}

	std::vector<char> separators;
	std::vector<char> whitespace;
	std::vector<std::string> keywords;
	std::map<std::string, int> tokens;
	
	int idToken = 0;
	int intToken = 0;
	int fltToken = 0;
	int charToken = 0;
	int strToken = 0;
private:
	std::string path = "";
	std::ifstream reader;
	
	//Control stuff
	std::string current = "";
	bool ret_next = false;
	bool in_quote = false;
	bool in_squote = false;
	Token tNext;
	std::stack<Token> stored;
	
	//Position information
	int ln_no = 1;
	std::string current_ln = "";
};
