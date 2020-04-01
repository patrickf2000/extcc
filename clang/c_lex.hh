#pragma once

#include <lex.hh>

enum CTokenType {
	None,
	
	//Types
	Void,
	Char,
	Short,
	Int,
	Float,
	Double,
	Unsigned,
	Signed,
	
	//Function-related stuff
	Extern,
	
	//Literals
	Id,
	String,
	CharL,
	No,
	FloatL,
	
	//Symbols
	LeftParen,
	RightParen,
	LeftCBrace,
	RightCBrace,
	Mul,
	SemiColon,
	Assign
};

class CScanner : public Scanner {
public:
	explicit CScanner(std::string in) : Scanner(in) {
		initSeparators();
		initKeywords();
		initTokens();
	}
protected:
	void initSeparators() override;
	void initKeywords() override;
	void initTokens() override;
};
