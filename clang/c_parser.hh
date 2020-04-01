#pragma once

#include <parser.hh>

class CParser : public Parser {
public:
	explicit CParser(std::string in) : Parser(in) {
		top = new AstScope;
	}
	void parse();
};
