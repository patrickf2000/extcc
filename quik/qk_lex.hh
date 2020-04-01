#pragma once

#include <lex.hh>

enum QkTokenType {
	None,
	
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
	explicit QkScanner(std::string in) : Scanner(in) {
		initSeparators();
		initKeywords();
		initTokens();
	}
protected:
	void initSeparators() override;
	void initKeywords() override;
	void initTokens() override;
};
