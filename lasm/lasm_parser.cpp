#include "lasm_parser.hh"

using namespace Asm;

//The main assembly parse file
void AsmParser::parse() {
	int type = 0;
	
	while (type != -1) {
		Token next = scan->getNext();
		type = next.type;
		
		switch (type) {
			case AsmTokenType::Section: buildSection(); break;
		}
	}
}

//Builds a section
void AsmParser::buildSection() {
	Token next = scan->getNext();
	
	if (next.type == AsmTokenType::Data)
		file->data = new LtacDataSec;
	else if (next.type == AsmTokenType::Code)
		file->code = new LtacCodeSec;
	else
		syntax->fatalError("Invalid section name.");
		
	next = scan->getNext();
	
	if (next.type != AsmTokenType::NewLn)
		syntax->fatalError("Expected end-of-line.");
}
