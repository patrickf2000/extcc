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
	
	//Data elements
	String,
	
	//Function stuff
	Func,
	Lbl,
	Ret,
	IPushArg,
	StrPushArg,
	FuncCall,
	
	//Load/store stuff
	IStoreC,
	ILdRet,
	MoveVV,
	
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

//Represents a string data element
class PasmString : public PasmNode {
public:
	explicit PasmString(std::string name, std::string val) {
		type = pasm::String;
		this->name = name;
		this->val = val;
	}
	
	std::string name = "";
	std::string val = "";
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

//Return
class Ret : public PasmNode {
public:
	explicit Ret() { type = pasm::Ret; }
};

//Push an integer argument
class IPushArg : public PasmNode {
public:
	explicit IPushArg(int val) {
		type = pasm::IPushArg;
		this->val = val;
	}
	
	int val = 0;
};

//Push a string argument
class StrPushArg : public PasmNode {
public:
	explicit StrPushArg(std::string name) {
		type = pasm::StrPushArg;
		this->name = name;
	}
	
	std::string name = "";
};

//Function call
class FuncCall : public PasmNode {
public:
	explicit FuncCall(std::string name) {
		type = pasm::FuncCall;
		this->name = name;
	}
	
	std::string name = "";
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

//Move on var to another
class MoveVV : public PasmNode {
public:
	explicit MoveVV(int pos1, int pos2) {
		type = pasm::MoveVV;
		this->pos1 = pos1;
		this->pos2 = pos2;
	}
	
	int pos1 = 0;
	int pos2 = 0;
	DType dType = DType::None;
};

//Store integer to return register
class ILdRet : public PasmNode {
public:
	explicit ILdRet(int val) {
		type = pasm::ILdRet;
		this->val = val;
	}
	
	int val;
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

