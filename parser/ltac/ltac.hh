#pragma once

#include <vector>
#include <string>

#include <ast.hh>

// This relates to the overall node types
enum class LtacType {
	None,
	File,
	Data,
	Code
};

// This contains the actual instructions
enum class ltac {
	None,
	Func,
	FuncCall,
	Label,
	Ret,
	Var,
	Reg,
	Array,
	ArrayAcc,
	ArraySet,
	
	PushArg,
	
	Int,
	Byte,
	Float,
	Double,
	String,
	
	MathOp,
	Math,
	IMath,
	
	Push,
	Pop,
	
	Cmp,
	ICmp,
	
	Jmp
};

// This relates to ltac data
enum class LtacData {
	None,
	String,
	Float,
	Int
};

//Various operators (for math functions)
enum class Operator {
	None,
	Add,
	Sub,
	Mul,
	Div,
	Mod,
	
	PAdd,
	PSub,
	
	Equal,
	NotEqual,
	Greater,
	Less,
	GreaterEq,
	LessEq
};

//The nodes for ltac
//This is the base class for all nodes
class LtacNode {
public:
	explicit LtacNode() {}
	explicit LtacNode(ltac t) { type = t; }

	LtacType node_type;
	ltac type = ltac::None;		//Because most nodes are code/instruction nodes, we want this
	std::vector<LtacNode *> children;
};

//The base class for the data section
class LtacDataSec : public LtacNode {
public:
	explicit LtacDataSec() { node_type = LtacType::Data; }
};

//The base class for the for code section
class LtacCodeSec : public LtacNode {
public:
	explicit LtacCodeSec() { node_type = LtacType::Code; }
};

//This is the base class for the file
class LtacFile : public LtacNode {
public:
	explicit LtacFile() {
		node_type = LtacType::File;
	}
	
	explicit LtacFile(std::string n) {
		node_type = LtacType::File;
		name = n;
	}
	
	std::string name = "";
	
	LtacDataSec *data;
	LtacCodeSec *code;
};

//Labels
class LtacLabel : public LtacNode {
public:
	explicit LtacLabel() { type = ltac::Label; }
	explicit LtacLabel(std::string n) {
		type = ltac::Label;
		name = n;
	}
	
	std::string name = "";
};

//Functions
class LtacFunc : public LtacLabel {
public:
	explicit LtacFunc() { type = ltac::Func; }
	explicit LtacFunc(std::string n) {
		type = ltac::Func;
		name = n;
	}

	int stack_size = 0;
	bool is_global = false;
	bool is_extern = false;
};

//A node with a datatype
class LtacTypeNode : public LtacNode {
public:
	DataType d_type = DataType::Void;
};

//Variable assignment/declaration/operation
class LtacVar : public LtacTypeNode {
public:
	explicit LtacVar() { type = ltac::Var; }
	int pos = 0;
	int size = 0;
	
	int rvar = -1;
	bool is_ref = false;
	bool is_ptr = false;
};

//Register assignment/operation
class LtacReg : public LtacNode {
public:
	explicit LtacReg() { type = ltac::Reg; }
	int pos = 0;
};

//Push argument operation
class LtacPushArg : public LtacNode {
public:
	explicit LtacPushArg() { type = ltac::PushArg; }
};

//Function calls
class LtacFuncCall : public LtacNode {
public:
	explicit LtacFuncCall() { type = ltac::FuncCall; }
	explicit LtacFuncCall(std::string n) {
		type = ltac::FuncCall;
		name = n;
	}
	
	std::string name = "";
	std::vector<Var> args;
	std::vector<LtacVar *> ret_dest;
};

//Function returns
class LtacRet : public LtacTypeNode {
public:
	explicit LtacRet() { type = ltac::Ret; }
};

//Integers
class LtacInt : public LtacNode {
public:
	explicit LtacInt() { type = ltac::Int; }
	explicit LtacInt(int i) {
		type = ltac::Int;
		val = i;
	}
	
	int val = 0;
};

//Characters
// The characters are encoded as ints, but they are
//accessed and stored a little differently, hence separate class
class LtacByte : public LtacInt {
public:
	explicit LtacByte() { type = ltac::Byte; }
	explicit LtacByte(int i) {
		type = ltac::Byte;
		val = i;
	}
};

//Single-precision floats
class LtacFloat : public LtacNode {
public:
	explicit LtacFloat() { type = ltac::Float; }
	
	float val = 0;
	int i_val = 0;
	std::string name = "";
};

//Double-precision floats
class LtacDouble : public LtacNode {
public:
	explicit LtacDouble() { type = ltac::Double; }
	
	double val;
	std::string i_val = "";
	std::string name = "";
};

//Strings
class LtacString : public LtacNode {
public:
	explicit LtacString() { type = ltac::String; }
	explicit LtacString(std::string n, std::string v) {
		type = ltac::String;
		name = n;
		val = v;
	}
	
	std::string name = "";
	std::string val = "";
};

//Arrays
class LtacArray : public LtacVar {
public:
	explicit LtacArray() { type = ltac::Array; }

	int type_size = 1;
};

//Array access
class LtacArrayAcc : public LtacArray {
public:
	explicit LtacArrayAcc() { type = ltac::ArrayAcc; }
};

//Array set
class LtacArraySet : public LtacArray {
public:
	explicit LtacArraySet() { type = ltac::ArraySet; }
	
	LtacNode *index;
};

class LtacOp : public LtacNode {
public:
	Operator op;
};

//Math operations
class LtacMathOp : public LtacOp {
public:
	explicit LtacMathOp() { type = ltac::MathOp; }
	
	LtacNode *operand;
};

//Math
class LtacMath : public LtacNode {
public:
	explicit LtacMath() { type = ltac::Math; }
	
	LtacNode *init_val;
};

//Integer math
class LtacIMath : public LtacOp {
public:
	explicit LtacIMath() { type = ltac::IMath; }
};

//Comparisons
class LtacCmp : public LtacNode {
public:
	explicit LtacCmp() { type = ltac::Cmp; }
	
	LtacNode *lval;
	LtacNode *rval;
};

//Integer comparisons
class LtacICmp : public LtacCmp {
public:
	explicit LtacICmp() { type = ltac::ICmp; }
};

//Jumps
//Set the operator to none for unconditional jump
class LtacJmp : public LtacNode {
public:
	explicit LtacJmp() { type = ltac::Jmp; }
	
	std::string dest;
	Operator op = Operator::None;
};

//Useful functions
void print_ltac(LtacFile *file);

