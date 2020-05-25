#pragma once

#include <string>
#include <fstream>

#include <pasm/pasm.hh>

using namespace PASM;

class CompilerBase {
public:
	CompilerBase(std::string name) {
		writer = std::ofstream(name);
	}
	
	virtual void build_data(PasmFile *file) {}
	void build_code(PasmFile *file);
	void write();
	
	virtual void build_func(PasmNode *ln) {}
	virtual void build_ildarg(PasmNode *ln) {}
	virtual void build_f32_ldarg(PasmNode *ln) {}
	virtual void build_f64_ldarg(PasmNode *ln) {}
	virtual void build_ildret(PasmNode *ln) {}
	virtual void build_istret(PasmNode *ln) {}
	virtual void build_ret() {}
	virtual void build_str_pusharg(PasmNode *ln) {}
	virtual void build_ipusharg(PasmNode *ln) {}
	virtual void build_f32_pusharg(PasmNode *ln) {}
	virtual void build_f64_pusharg(PasmNode *ln) {}
	virtual void build_call(PasmNode *ln) {}
	
	virtual void build_istorec(PasmNode *ln) {}
	virtual void build_f32_storec(PasmNode *ln) {}
	virtual void build_f64_storec(PasmNode *ln) {}
	virtual void build_move_vv(PasmNode *ln) {}
	virtual void build_ildr(PasmNode *ln) {}
	virtual void build_str(PasmNode *ln) {}
	
	virtual void build_imath_ri(PasmNode *ln) {}
	virtual void build_imath_rv(PasmNode *ln) {}
	virtual void build_imath_vi(PasmNode *ln) {}
	
	virtual void build_icmp(PasmNode *ln) {}
	virtual void build_br(PasmNode *ln) {}
	
	virtual void build_isysarg(PasmNode *ln) {}
	virtual void build_str_sysarg(PasmNode *ln) {}
	virtual void build_syscall(PasmNode *ln) {}
protected:
	std::ofstream writer;
	
	void fatalError(std::string msg);
	void warning(std::string msg);
};
