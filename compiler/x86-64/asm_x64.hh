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
	void build_istret(PasmNode *ln);
	void build_ret();
	
	void build_istorec(PasmNode *ln);
	
	void build_isysarg(PasmNode *ln);
	void build_str_sysarg(PasmNode *ln);
	void build_syscall(PasmNode *ln);
protected:
	void fatalError(std::string msg);
	void warning(std::string msg);
private:
	std::ofstream writer;
};
