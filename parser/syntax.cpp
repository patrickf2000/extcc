#include <iostream>
#include <cstdlib>

#include "syntax.hh"

//Adds an error message (via string)
void Syntax::addError(std::string msg) {
	CompileMsg cm;
	cm.msg = msg;
	cm.original = scan->getCurrentLn();
	cm.ln_no = scan->getLnNo();
	
	errors.push_back(cm);
}

//Adds an error message
void Syntax::addError(CompileMsg msg) {
	errors.push_back(msg);
}

//Adds a warning message (via string)
void Syntax::addWarning(std::string msg) {
	CompileMsg cm;
	cm.msg = msg;
	cm.original = scan->getCurrentLn();
	cm.ln_no = scan->getLnNo();
	
	warnings.push_back(cm);
}

//Adds a warning message
void Syntax::addWarning(CompileMsg msg) {
	warnings.push_back(msg);
}

//Display warning messages
void Syntax::displayWarnings() {
	for (auto msg : warnings) {
		std::cout << "[Warning] " << msg.msg << std::endl;
		std::cout << "[" << msg.ln_no << "] " << msg.original << std::endl;
	}
}

//Display error messages
void Syntax::displayErrors() {
	if (errors.size() == 0)
		return;

	for (auto msg : errors) {
		std::cout << "[Error] " << msg.msg << std::endl;
		std::cout << "[" << msg.ln_no << "] " << msg.original << std::endl;
	}
	
	std::exit(1);
}
