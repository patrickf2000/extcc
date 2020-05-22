#pragma once

#include <string>
#include <fstream>

#include <pasm/pasm.hh>

using namespace PASM;

class X64 {
public:
	explicit X64(std::string name);
	void build_code(PasmFile *file);
	void write();
	
	void build_func(PasmNode *ln);
private:
	std::ofstream writer;
};
