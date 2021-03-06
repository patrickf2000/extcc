#pragma once

#include <base/asm_base.hh>
#include <ltac/ltac.hh>

class Asm_i386 : public AsmGen {
public:
	Asm_i386(LtacFile *f) : AsmGen(f) {}
	
	void build_data(LtacDataSec *data);
	void build_code(LtacCodeSec *code);
	
	void build_func(LtacNode *node);
};
