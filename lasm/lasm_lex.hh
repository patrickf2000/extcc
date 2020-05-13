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
		Double,
		Void,
		Any,
		Var,
		Reg,
		Ptr,
		
		//Function tokens
		Func,
		Extern,
		Ret,
		LdArg,
		PushArg,
		Call,
		
		//Register operations
		Ldr,
		Str,
		FLdr,
		FStr,
		StrRet,
		F64_Ldr,
		F64_Str,
		
		//Store operations
		IStoreI,
		
		//Array operations
		ArraySet,
		ArrayAcc,
		
		//Vector operations
		Vldr,
		Vldri,
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
		
		//Vector float-32 math
		F32_VAdd,
		F32_VSub,
		F32_VMul,
		F32_VDiv,

		//Float-64 math operations
		F64_Add,
		F64_Sub,
		F64_Mul,
		F64_Div,
		
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
