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
	
	//Keywords
	Return,
	If,
	Else,
	While,
	For,
	Struct,
	
	//Literals
	Id,
	String,
	CharL,
	No,
	FloatL,
	
	//Conditional Operators
	Greater,
	Less,
	
	//Math operators
	Plus,
	Minus,
	Mul,		//Also used for pointers
	Div,
	Mod,
	
	//Symbols
	LeftParen,
	RightParen,
	LeftCBrace,
	RightCBrace,
	LBracket,
	RBracket,
	SemiColon,
	Assign,
	Comma,
	Amp,
	Dot
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
