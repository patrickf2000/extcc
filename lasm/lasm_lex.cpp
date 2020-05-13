#include "lasm_lex.hh"

using namespace LtacAsm;

//Tell the scanner about our separators
void AsmScanner::initSeparators() {
	separators.push_back('\n');
	separators.push_back(',');
	
	whitespace.push_back('\t');
	whitespace.push_back(' ');
}

//Tell the scanner about our keywords
void AsmScanner::initKeywords() {
	keywords.push_back("section");
	keywords.push_back("data");
	keywords.push_back("code");
	keywords.push_back("func");
	keywords.push_back("ret");
	keywords.push_back("string");
	keywords.push_back("float");
	keywords.push_back("pusharg");
	keywords.push_back("call");
	keywords.push_back("int");
	keywords.push_back("var");
	keywords.push_back("ldr");
	keywords.push_back("iadd");
	keywords.push_back("isub");
	keywords.push_back("imul");
	keywords.push_back("idiv");
	keywords.push_back("imod");
	keywords.push_back("reg");
	keywords.push_back("str");
}

//Match everything with a token
void AsmScanner::initTokens() {
	//Start with keywords
	tokens["section"] = AsmTokenType::Section;
	tokens["data"] = AsmTokenType::Data;
	tokens["code"] = AsmTokenType::Code;
	tokens["func"] = AsmTokenType::Func;
	tokens["ret"] = AsmTokenType::Ret;
	tokens["string"] = AsmTokenType::String;
	tokens["float"] = AsmTokenType::Float;
	tokens["pusharg"] = AsmTokenType::PushArg;
	tokens["call"] = AsmTokenType::Call;
	tokens["int"] = AsmTokenType::Int;
	tokens["var"] = AsmTokenType::Var;
	tokens["ldr"] = AsmTokenType::Ldr;
	tokens["iadd"] = AsmTokenType::IAdd;
	tokens["isub"] = AsmTokenType::ISub;
	tokens["imul"] = AsmTokenType::IMul;
	tokens["idiv"] = AsmTokenType::IDiv;
	tokens["imod"] = AsmTokenType::IMod;
	tokens["reg"] = AsmTokenType::Reg;
	tokens["str"] = AsmTokenType::Str;
	tokens["fldr"] = AsmTokenType::FLdr;
	tokens["fstr"] = AsmTokenType::FStr;
	tokens["f32.add"] = AsmTokenType::F32_Add;
	tokens["f32.sub"] = AsmTokenType::F32_Sub;
	tokens["f32.mul"] = AsmTokenType::F32_Mul;
	tokens["f32.div"] = AsmTokenType::F32_Div;
	tokens["extern"] = AsmTokenType::Extern;
	tokens["void"] = AsmTokenType::Void;
	tokens["any"] = AsmTokenType::Any;
	tokens["ptr"] = AsmTokenType::Ptr;
	tokens["str.ret"] = AsmTokenType::StrRet;
	tokens["arrayset"] = AsmTokenType::ArraySet;
	tokens["arrayacc"] = AsmTokenType::ArrayAcc;
	tokens["vldr"] = AsmTokenType::Vldr;
	tokens["vstr"] = AsmTokenType::Vstr;
	tokens["viadd"] = AsmTokenType::VIAdd;
	tokens["visub"] = AsmTokenType::VISub;
	tokens["vimul"] = AsmTokenType::VIMul;
	tokens["vidiv"] = AsmTokenType::VIDiv;
	tokens["icmp"] = AsmTokenType::ICmp;
	tokens["jl"] = AsmTokenType::Jl;
	tokens["lbl"] = AsmTokenType::Lbl;
	tokens["f32.vadd"] = AsmTokenType::F32_VAdd;
	tokens["f32.vsub"] = AsmTokenType::F32_VSub;
	tokens["f32.vmul"] = AsmTokenType::F32_VMul;
	tokens["f32.vdiv"] = AsmTokenType::F32_VDiv;
	tokens["vldri"] = AsmTokenType::Vldri;
	tokens["double"] = AsmTokenType::Double;
	tokens["f64.ldr"] = AsmTokenType::F64_Ldr;
	tokens["f64.str"] = AsmTokenType::F64_Str;
	tokens["f64.add"] = AsmTokenType::F64_Add;
	tokens["f64.sub"] = AsmTokenType::F64_Sub;
	tokens["f64.mul"] = AsmTokenType::F64_Mul;
	tokens["f64.div"] = AsmTokenType::F64_Div;
	tokens["ldarg"] = AsmTokenType::LdArg;
	tokens["i.store_i"] = AsmTokenType::IStoreI;

	//Symbols
	tokens["\n"] = AsmTokenType::NewLn;
	tokens[")"] = AsmTokenType::Comma;
	
	//Constant/literal tokens
	idToken = AsmTokenType::Name;
	intToken = AsmTokenType::IntL;
	fltToken = AsmTokenType::FloatL;
	strToken = AsmTokenType::StringL;
}
