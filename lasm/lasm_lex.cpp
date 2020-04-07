#include "lasm_lex.hh"

using namespace Asm;

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
	keywords.push_back("text");
	keywords.push_back("func");
	keywords.push_back("ret");
}

//Match everything with a token
void AsmScanner::initTokens() {
	//Start with keywords
	tokens["section"] = AsmTokenType::Section;
	tokens["data"] = AsmTokenType::Data;
	tokens["text"] = AsmTokenType::Text;
	tokens["func"] = AsmTokenType::Func;
	tokens["ret"] = AsmTokenType::Ret;
	
	//Symbols
	tokens["\n"] = AsmTokenType::NewLn;
	tokens[")"] = AsmTokenType::Comma;
	
	//Constant/literal tokens
	idToken = AsmTokenType::Name;
	intToken = AsmTokenType::Int;
	fltToken = AsmTokenType::Float;
	strToken = AsmTokenType::String;
}
