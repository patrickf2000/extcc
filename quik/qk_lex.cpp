#include "qk_lex.hh"

//Tell the scanner about our separators
void QkScanner::initSeparators() {
	separators.push_back('(');
	separators.push_back(')');
	
	whitespace.push_back('\n');
	whitespace.push_back('\t');
	whitespace.push_back(' ');
}

//Tell the scanner about our keywords
void QkScanner::initKeywords() {
	keywords.push_back("func");
	keywords.push_back("end");
	keywords.push_back("int");
}

//Match everything with a token
void QkScanner::initTokens() {
	//Start with keywords
	tokens["func"] = QkTokenType::Func;
	tokens["end"] = QkTokenType::End;
	tokens["int"] = QkTokenType::Int;
	
	//Symbols
	tokens["("] = QkTokenType::LeftParen;
	tokens[")"] = QkTokenType::RightParen;
	
	//Constant/literal tokens
	idToken = QkTokenType::Id;
	intToken = QkTokenType::No;
	fltToken = QkTokenType::Float;
	charToken = QkTokenType::Char;
	strToken = QkTokenType::String;
}
