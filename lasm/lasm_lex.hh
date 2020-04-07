#pragma once

#include <lex.hh>

enum AsmTokenType {
	None,
	
	//Section tokens
	Section,
	Data,
	Text,
	
	//Function tokens
	Func,
	Ret,
	
	//Identifiers
	Id,
	No,
	Float,
	Char,
	String,
	
	//Symbols
	NewLn,
	Comma
};

class AsmScanner : public Scanner {
public:
	explicit AsmScanner(std::string in) : Scanner(in) {
		initSeparators();
		initKeywords();
		initTokens();
	}
protected:
	void initSeparators() override;
	void initKeywords() override;
	void initTokens() override;
};
