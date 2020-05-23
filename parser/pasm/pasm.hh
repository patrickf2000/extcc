//Portable Assembly (PASM)
/*
 * Portable assembly is a new low-level IR designed to present a close correlation
 * between the software and architecture. It is a typed language and is designed
 * to be more easily translated into Assembly than LTAC.
 *
 * This header contains the types for PASM. I haven't decided yet what I'm going to
 * do with LTAC, it will depend on how this works out.
 *
 * For now, I will be creating a text version of PASM to test the translation into
 * Assembly before porting it to the other parts of the compiler
*/
#pragma once

#include <string>
#include <vector>

namespace PASM {

//Represents PASM types
enum class pasm {
	None,
	
	//Function stuff
	Func,
	Lbl,
	
	//Load/store stuff
	IStoreC,
	
	//System class stuff
	ISysarg,
	StrSysarg,
	Syscall
};

//Represents operand type
enum class Operand {
	None,
	Reg,
	Var,
	Const
};

//Represents PASM data types
enum class DType {
	None,
	Byte,
	Short,
	Int,
	Float32,
	Float64,
	String,
	Ptr
};

//The base of all PASM nodes
class PasmNode {
public:
	explicit PasmNode() {}
	explicit PasmNode(pasm type) {
		this->type = type;
	}
	
	pasm type = pasm::None;
	Operand opType = Operand::None;
};

//Represents a PASM file
class PasmFile {
public:
	PasmFile() { }
	
	std::string name = "";
	std::vector<PasmNode *> data;
	std::vector<PasmNode *> code;
};

//Assembly label
class Label : public PasmNode {
public:
	explicit Label() { type = pasm::Lbl; }
	explicit Label(std::string name) {
		type = pasm::Lbl;
		this->name = name;
	}
	
	std::string name = "";
};

//Assembly function
// On some platforms or in some cases, we may want to make a distinction
// between labels and functions
class Func : public Label {
public:
	explicit Func() { type = pasm::Func; }
	explicit Func(std::string name) {
		type = pasm::Func;
		this->name = name;
	}
	
	int stackSize = 0;
};

//Stores a constant to a variable
class IStoreConst : public PasmNode {
public:
	explicit IStoreConst(int pos, int val) {
		type = pasm::IStoreC;
		this->pos = pos;
		this->val = val;
	}
	
	int pos = 0;
	int val = 0;
};

//Integer system call arguments
class ISysArg : public PasmNode {
public:
	explicit ISysArg(int arg) {
		type = pasm::ISysarg;
		this->arg = arg;
	}
	
	int arg = 0;
};

//String system call arguments
class StrSysArg : public PasmNode {
public:
	explicit StrSysArg(std::string arg) {
		type = pasm::StrSysarg;
		this->arg = arg;
	}
	
	std::string arg = "";
};

//Represents a system call
class SysCall : public PasmNode {
public:
	explicit SysCall() { type = pasm::Syscall; }
};

//Useful functions
std::string unwrite(PasmFile *file);

}

