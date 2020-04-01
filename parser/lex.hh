#pragma once

#include <vector>
#include <map>
#include <string>
#include <fstream>

struct Token {
	int type;
	std::string id;
};

class Scanner {
public:
	explicit Scanner(std::string in);
	Token getNext();
	void close();
protected:
	virtual void initSeparators() {}
	virtual void initKeywords() {}
	virtual void initTokens() {}

	std::vector<char> separators;
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
};
