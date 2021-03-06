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
	Space,
	
	//Data elements
	String,
	Float,
	Double,
	
	//Function stuff
	Func,
	Lbl,
	Ret,
	BPushArg,
	IPushArg,
	F32_PushArg,
	F64_PushArg,
	StrPushArg,
	Ptr_PushArg,
	FuncCall,
	BLdArg,
	ILdArg,
	F32_LdArg,
	F64_LdArg,
	Ptr_LdArg,
	
	//Load/store stuff
	BStoreC,
	IStoreC,
	F32_StoreC,
	F64_StoreC,
	Str_StoreC,
	ILdRet,
	IStrRet,
	Ptr_StrRet,
	IStrPtr,
	MoveVV,
	ILdr,
	PtrLd,
	Str,
	PtrStr,
	
	//Math stuff
	IMathRI,
	IMathRV,
	IMathRR,
	IMathVI,
	
	//System class stuff
	ISysarg,
	StrSysarg,
	Syscall,
	
	//Branching
	Br,
	
	//Comparisons
	ICmp
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

//Represents a math type
enum class MathType {
	None,
	Add,
	Sub,
	Mul,
	Div
};

//Represents a branch type
enum class JmpType {
	None,
	Eq,
	Neq,
	Z,
	Nz,
	G,
	Ge,
	L,
	Le
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

//Represents a float data element
class PasmFloat : public PasmString {
public:
	explicit PasmFloat(std::string name, std::string val)
		: PasmString(name, val) {
		type = pasm::Float;	
	}
};

//Represents a double data element
class PasmDouble : public PasmString {
public:
	explicit PasmDouble(std::string name, std::string val)
		: PasmString(name, val) {
		type = pasm::Double;
	}
};

//Represents an empty line- for formatting purposes
class PasmSpace : public PasmNode {
public:
	explicit PasmSpace() { type = pasm::Space; }
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

//Push a byte argument
class BPushArg : public PasmNode {
public:
	explicit BPushArg(int val) {
		type = pasm::BPushArg;
		this->val = val;
	}
	
	int val = 0;
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

//Push a float-32 argument
class F32_PushArg : public PasmNode {
public:
	explicit F32_PushArg(std::string val) {
		type = pasm::F32_PushArg;
		this->val = val;
	}
	
	explicit F32_PushArg(int pos) {
		type = pasm::F32_PushArg;
		this->pos = pos;
	}
	
	std::string val = "";
	int pos = 0;
	bool promote = false;
};

//Push a float-64 argument
class F64_PushArg : public PasmNode {
public:
	explicit F64_PushArg(std::string val) {
		type = pasm::F64_PushArg;
		this->val = val;
	}
	
	explicit F64_PushArg(int pos) {
		type = pasm::F64_PushArg;
		this->pos = pos;
	}
	
	std::string val = "";
	int pos = 0;
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

//Push a pointer argument
class Ptr_PushArg : public PasmNode {
public:
	explicit Ptr_PushArg(int pos) {
		type = pasm::Ptr_PushArg;
		this->pos = pos;
	}
	
	int pos = 0;
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

class LdArg : public PasmNode {
public:
	explicit LdArg(int pos, pasm type) {
		this->type = type;
		this->pos = pos;
	}
	
	int pos = 0;
};

//Load a byte argument
class BLdArg : public LdArg {
public:
	explicit BLdArg(int pos) : 
		LdArg(pos,pasm::BLdArg) {}
};

//Load an integer argument
class ILdArg : public LdArg {
public:
	explicit ILdArg(int pos) : 
		LdArg(pos,pasm::ILdArg) {}
};

//Load a float-32 argument
class F32_LdArg : public LdArg {
public:
	explicit F32_LdArg(int pos) : 
		LdArg(pos,pasm::F32_LdArg) {}
};

//Load a float-64 argument
class F64_LdArg : public LdArg {
public:
	explicit F64_LdArg(int pos) : 
		LdArg(pos,pasm::F64_LdArg) {}
};

//Load a pointer argument
class Ptr_LdArg : public LdArg {
public:
	explicit Ptr_LdArg(int pos) : 
		LdArg(pos,pasm::Ptr_LdArg) {}
};

//Stores a byte/char constant to a variable
class BStoreConst : public PasmNode {
public:
	explicit BStoreConst(int pos, unsigned char val) {
		type = pasm::BStoreC;
		this->pos = pos;
		this->val = val;
	}
	
	int pos = 0;
	unsigned char val = 0;
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

//Store a float constant to a variable
class F32_StoreConst : public PasmNode {
public:
	explicit F32_StoreConst(int pos, std::string name) {
		type = pasm::F32_StoreC;
		this->pos = pos;
		this->name = name;
	}
	
	int pos = 0;
	std::string name = "";
};

//Store a double constant to a variable
class F64_StoreConst : public PasmNode {
public:
	explicit F64_StoreConst(int pos, std::string name) {
		type = pasm::F64_StoreC;
		this->pos = pos;
		this->name = name;
	}
	
	int pos = 0;
	std::string name = "";
};

//Store a string constant
class Str_StoreConst : public PasmNode {
public:
	explicit Str_StoreConst(int pos, std::string name) {
		type = pasm::Str_StoreC;
		this->pos = pos;
		this->name = name;
	}
	
	int pos = 0;
	std::string name = "";
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

//Load an integer register
class ILdr : public PasmNode {
public:
	explicit ILdr(int reg, int pos) {
		type = pasm::ILdr;
		this->reg = reg;
		this->pos = pos;
	}
	
	int reg = 0;
	int pos = 0;
	DType dType = DType::None;
};

//Load a pointer element to a register
class PtrLd : public PasmNode {
public:
	explicit PtrLd(int ptrPos, int pos, int size) {
		type = pasm::PtrLd;
		this->ptrPos = ptrPos;
		this->pos = pos;
		this->size = size;
	}
	
	int ptrPos = 0;
	int pos = 0;
	int size = 4;
	
	Operand posType = Operand::None;
};

//Store register value to memory
class Str : public ILdr {
public:
	explicit Str(int reg, int pos) : ILdr(reg, pos) {
		type = pasm::Str;
	}
};

//Store register value to a pointer
// opType will contain info about what we are storing
class PtrStr : public PtrLd {
public:
	explicit PtrStr(int ptrPos, int pos, int size)
		: PtrLd(ptrPos, pos, size) {
		type = pasm::PtrStr;
	}
	
	int src = 0;
};

//Load integer to return register
class ILdRet : public PasmNode {
public:
	explicit ILdRet(int val) {
		type = pasm::ILdRet;
		this->val = val;
	}
	
	int val;
};

//Store integer from return register
class IStrRet : public PasmNode {
public:
	explicit IStrRet(int val) {
		type = pasm::IStrRet;
		this->val = val;
	}
	
	int val;
};

//Store pointer from return register
class Ptr_StrRet : public PasmNode {
public:
	explicit Ptr_StrRet(int val) {
		type = pasm::Ptr_StrRet;
		this->val = val;
	}
	
	int val;
};

//Stores value in register loaded from pointer
// ie, if you access array value, it will be in register and you have to store
class IStrPtr : public PasmNode {
public:
	explicit IStrPtr(int val) {
		type = pasm::IStrPtr;
		this->val = val;
	}
	
	int val;
};

//Math operations
//Integer register-immediate math
class IMathRI : public PasmNode {
public:
	explicit IMathRI(MathType mType, int reg, int val) {
		type = pasm::IMathRI;
		this->mType = mType;
		this->reg = reg;
		this->val = val;
	}
	
	MathType mType = MathType::None;
	int reg = 0;
	int val = 0;
};

//Integer register-variable math
class IMathRV : public PasmNode {
public:
	explicit IMathRV(MathType mType, int reg, int pos) {
		type = pasm::IMathRV;
		this->mType = mType;
		this->reg = reg;
		this->pos = pos;
	}
	
	MathType mType = MathType::None;
	int reg = 0;
	int pos = 0;
};

//Integer register-register math
// If register == -1, return register
// If register == -2, pointer access register
class IMathRR : public PasmNode {
public:
	explicit IMathRR(MathType mType, int reg1, int reg2) {
		type = pasm::IMathRR;
		this->mType = mType;
		this->reg1 = reg1;
		this->reg2 = reg2;
	}
	
	MathType mType = MathType::None;
	int reg1 = 0;
	int reg2 = 0;
};

//Integer variable - immediate math
class IMathVI : public PasmNode {
public:
	explicit IMathVI(MathType mType, int pos, int val) {
		type = pasm::IMathVI;
		this->mType = mType;
		this->pos = pos;
		this->val = val;
	}
	
	MathType mType = MathType::None;
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

//Represents a branch
class Branch : public PasmNode {
public:
	explicit Branch(std::string lbl) {
		type = pasm::Br;
		label = lbl;
	}
	
	explicit Branch(JmpType jmp, std::string lbl) {
		type = pasm::Br;
		this->jmp = jmp;
		label = lbl;
	}
	
	JmpType jmp = JmpType::None;	//Unconditional by default
	std::string label = "";
};

//Integer comparisons
//Variable to variable
class ICmp : public PasmNode {
public:
	explicit ICmp() { type = pasm::ICmp; }

	explicit ICmp(int pos1, int pos2) {
		type = pasm::ICmp;
		this->pos1 = pos1;
		this->pos2 = pos2;
	}
	
	int pos1 = 0;
	int pos2 = 0;
	
	Operand op1 = Operand::Var;
	Operand op2 = Operand::Var;
};

//Useful functions
std::string unwrite(PasmFile *file);

}

