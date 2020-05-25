#pragma once

#include <string>
#include <fstream>

#include <pasm/pasm.hh>

using namespace PASM;

class X64 {
public:
	explicit X64(std::string name);
	void build_data(PasmFile *file);
	void build_code(PasmFile *file);
	void write();
	
	void build_func(PasmNode *ln);
	void build_ildarg(PasmNode *ln);
	void build_ildret(PasmNode *ln);
	void build_istret(PasmNode *ln);
	void build_ret();
	void build_str_pusharg(PasmNode *ln);
	void build_ipusharg(PasmNode *ln);
	void build_f32_pusharg(PasmNode *ln);
	void build_call(PasmNode *ln);
	
	void build_istorec(PasmNode *ln);
	void build_f32_storec(PasmNode *ln);
	void build_move_vv(PasmNode *ln);
	void build_ildr(PasmNode *ln);
	void build_str(PasmNode *ln);
	
	void build_imath_ri(PasmNode *ln);
	void build_imath_rv(PasmNode *ln);
	void build_imath_vi(PasmNode *ln);
	
	void build_icmp(PasmNode *ln);
	void build_br(PasmNode *ln);
	
	void build_isysarg(PasmNode *ln);
	void build_str_sysarg(PasmNode *ln);
	void build_syscall(PasmNode *ln);
protected:
	void fatalError(std::string msg);
	void warning(std::string msg);
private:
	std::ofstream writer;
};
