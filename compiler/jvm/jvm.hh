#pragma once

#include <string>

#include <base/cbase.hh>
#include <pasm/pasm.hh>

#include <javabuilder.hh>
#include <javafile.hh>
#include <javapool.hh>

using namespace PASM;

class JVM : public CompilerBase {
public:
	explicit JVM(std::string name);
	void build_data(PasmFile *file);
	
	void build_func(PasmNode *ln);
	void build_ret();
	void build_call(PasmNode *ln);
	
	void code_init() {}
	void write() {
		builder->write();
	}
private:
	JavaBuilder *builder;
	JavaFunc *currentFunc;
};
