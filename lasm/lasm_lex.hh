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
		Void,
		Var,
		Reg,
		
		//Function tokens
		Func,
		Extern,
		Ret,
		PushArg,
		Call,
		
		//Register operations
		Ldr,
		Str,
		FLdr,
		FStr,
		
		//Integer math operations
		IAdd,
		ISub,
		IMul,
		IDiv,
		IMod,
		
		//Float-32 math operations
		F32_Add,
		F32_Sub,
		F32_Mul,
		F32_Div,
		
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
