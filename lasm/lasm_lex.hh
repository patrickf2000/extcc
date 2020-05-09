#pragma once

#include <lex.hh>

namespace LtacAsm {
	enum AsmTokenType {
		None,
		
		//Section tokens
		Section,
		Data,
		Code,
		Lbl,
		
		//Data tokens
		Int,
		String,
		Float,
		Void,
		Any,
		Var,
		Reg,
		Ptr,
		
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
		StrRet,
		
		//Array operations
		ArraySet,
		ArrayAcc,
		
		//Vector operations
		Vldr,
		Vstr,
		
		//Integer math operations
		IAdd,
		ISub,
		IMul,
		IDiv,
		IMod,
		
		//Vector integer math
		VIAdd,
		VISub,
		VIMul,
		VIDiv,
		
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
		
		//Flow control
		ICmp,
		Jl,
		
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
