#include "parser.hh"

Parser::Parser(std::string in) {
	scan = new Scanner(in);
}
