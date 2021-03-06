#pragma once

#include <string>
#include <fstream>

#include <pasm/pasm.hh>
#include <base/cbase.hh>

using namespace PASM;

class AArch64 : public CompilerBase {
public:
	explicit AArch64(std::string name);
	void build_data(PasmFile *file);
	
	void build_func(PasmNode *ln);
	void build_bldarg(PasmNode *ln);
	void build_ildarg(PasmNode *ln);
	void build_f32_ldarg(PasmNode *ln);
	void build_f64_ldarg(PasmNode *ln);
	void build_ptr_ldarg(PasmNode *ln);
	void build_ildret(PasmNode *ln);
	void build_istret(PasmNode *ln);
	void build_ptr_stret(PasmNode *ln);
	void build_istrptr(PasmNode *ln);
	void build_ret();
	void build_str_pusharg(PasmNode *ln);
	void build_bpusharg(PasmNode *ln);
	void build_ipusharg(PasmNode *ln);
	void build_f32_pusharg(PasmNode *ln);
	void build_f64_pusharg(PasmNode *ln);
	void build_ptr_pusharg(PasmNode *ln);
	void build_call(PasmNode *ln);
	
	void build_bstorec(PasmNode *ln);
	void build_istorec(PasmNode *ln);
	void build_f32_storec(PasmNode *ln);
	void build_f64_storec(PasmNode *ln);
	void build_str_storec(PasmNode *ln);
	void build_move_vv(PasmNode *ln);
	void build_ildr(PasmNode *ln);
	void build_str(PasmNode *ln);
	void build_ldptr(PasmNode *ln);
	void build_ptr_str(PasmNode *ln);
	
	void build_imath_ri(PasmNode *ln);
	void build_imath_rv(PasmNode *ln);
	void build_imath_vi(PasmNode *ln);
	void build_imath_rr(PasmNode *ln);
	
	void build_icmp(PasmNode *ln);
	void build_br(PasmNode *ln);
	
	void build_isysarg(PasmNode *ln);
	void build_str_sysarg(PasmNode *ln);
	void build_syscall(PasmNode *ln);
};

