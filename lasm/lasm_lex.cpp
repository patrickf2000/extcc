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
	
	//Symbols
	tokens["\n"] = AsmTokenType::NewLn;
	tokens[")"] = AsmTokenType::Comma;
	
	//Constant/literal tokens
	idToken = AsmTokenType::Name;
	intToken = AsmTokenType::IntL;
	fltToken = AsmTokenType::FloatL;
	strToken = AsmTokenType::StringL;
}
