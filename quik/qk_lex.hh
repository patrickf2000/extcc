#pragma once

#include <lex.hh>

enum QkTokenType {
	Func,
	FuncCall,
	End,
	Int,
	Id,
	
	//Literals
	String,
	Char,
	No,
	Float,
	
	//Symbols
	LeftParen,
	RightParen
};

class QkScanner : public Scanner {
public:
	explicit QkScanner(std::string in) : Scanner(in) {}
protected:
	void initSeparators();
	void initKeywords();
	void initTokens();
};
