#pragma once

#include <lex.hh>

namespace LtacAsm {
	enum AsmTokenType {
		None,
		
		//Section tokens
		Section,
		Data,
		Code,
		
		//Data tokens
		String,
		Float,
		
		//Function tokens
		Func,
		Ret,
		
		//Identifiers
		Name,
		Int,
		FloatL,
		StringL,
		
		//Symbols
		NewLn,
		Comma
	};
}

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
