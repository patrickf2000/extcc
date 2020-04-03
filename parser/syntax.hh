#pragma once

#include <string>
#include <vector>

#include <lex.hh>

struct CompileMsg {
	std::string msg;
	std::string original;
	int ln_no;
};

class Syntax {
public:
	Syntax(Scanner *s) {
		scan = s;
	}
	
	//Adds an error message
	void addError(std::string msg);
	void addError(CompileMsg msg);
	
	//Adds a warning message
	void addWarning(std::string msg);
	void addWarning(CompileMsg msg);
	
	//Display warnings and errors
	void displayWarnings();
	void displayErrors();
private:
	Scanner *scan;
	std::vector<CompileMsg> warnings;
	std::vector<CompileMsg> errors;
};
