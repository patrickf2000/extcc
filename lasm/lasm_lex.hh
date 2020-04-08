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
		Int,
		String,
		Float,
		Var,
		Reg,
		
		//Function tokens
		Func,
		Ret,
		PushArg,
		Call,
		
		//Register operations
		Ldr,
		
		//Math operations
		IAdd,
		ISub,
		IMul,
		IDiv,
		
		//Identifiers
		Name,
		IntL,
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
