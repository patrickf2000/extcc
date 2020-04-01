#include "c_lex.hh"

//Tell the scanner about our separators
void CScanner::initSeparators() {
	separators.push_back('(');
	separators.push_back(')');
	separators.push_back('{');
	separators.push_back('}');
	separators.push_back('*');
	separators.push_back(';');
	
	whitespace.push_back('\n');
	whitespace.push_back('\t');
	whitespace.push_back(' ');
}

//Tell the scanner about our keywords
void CScanner::initKeywords() {
	keywords.push_back("void");
	keywords.push_back("char");
	keywords.push_back("short");
	keywords.push_back("int");
	keywords.push_back("float");
	keywords.push_back("double");
	keywords.push_back("unsigned");
	keywords.push_back("signed");
	keywords.push_back("extern");
}

//Match everything with a token
void CScanner::initTokens() {
	//Start with keywords
	tokens["void"] = CTokenType::Void;
	tokens["char"] = CTokenType::Char;
	tokens["short"] = CTokenType::Short;
	tokens["int"] = CTokenType::Int;
	tokens["float"] = CTokenType::Float;
	tokens["double"] = CTokenType::Double;
	tokens["unsigned"] = CTokenType::Unsigned;
	tokens["signed"] = CTokenType::Signed;
	tokens["extern"] = CTokenType::Extern;
	
	//Symbols
	tokens["("] = CTokenType::LeftParen;
	tokens[")"] = CTokenType::RightParen;
	tokens["{"] = CTokenType::LeftCBrace;
	tokens["}"] = CTokenType::RightCBrace;
	tokens["*"] = CTokenType::Mul;
	tokens[";"] = CTokenType::SemiColon;
	
	//Constant/literal tokens
	idToken = CTokenType::Id;
	intToken = CTokenType::No;
	fltToken = CTokenType::FloatL;
	charToken = CTokenType::CharL;
	strToken = CTokenType::String;
}