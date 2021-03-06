#pragma once

#include <vector>
#include <string>
#include <map>
#include <sstream>

//Data type information
enum class DataType {
	None,
	Void,
	Bool,
	Byte,
	Short,
	Int,
	Long,
	Float,
	Double,
	Char,
	Str,
	Pointer,
	Any
};

//Conditional operator information
enum class CondOp {
	None,
	Equals,
	NotEquals,
	Greater,
	GreaterEq,
	Less,
	LessEq
};

//Denotes the types of our nodes so we can upcast
enum class AstType {
	Scope,
	
	//Function stuff
	FuncDec,
	ExternFunc,
	FuncCall,
	End,
	Return,
	
	//Structure stuff
	Struct,
	StructDec,
	StructAcc,
	StructMod,
	
	//Conditional stuff
	If,
	Elif,
	Else,
	EndIf,
	
	//Loop stuff
	While,
	Loop,
	ForEach,
	For,
	
	//Variable stuff
	VarDec,
	VarAssign,
	Id,
	Int,
	Hex,
	Char,
	Bool,
	Float,
	Double,
	Str,
	ArrayDec,
	ArrayAccess,
	ArrayAssign,
	Math,
	
	//Operator
	Add,
	Sub,
	Mul,
	Div,
	Mod,
	Inc,
	DMul,
	
	//Logical operators
	And,
	Or,
	Xor
};

//Holds variable information
struct Var {
	std::string name;
	DataType type;
	bool is_array;
	bool is_param;
	bool is_ptr;
	
	//Needed for the assembler
	int stack_pos;
	int size;
	
	//Structure stuff
	bool is_struct;
	std::string struct_name;
	
	//Used by the parser
	int scope_level;
};

//The base of all our nodes
class AstNode {
public:
	explicit AstNode() {}
	explicit AstNode(AstType t) { type = t; }
	virtual ~AstNode() {}
	AstType type;
	std::vector<AstNode *> children;
};

//The base class for nodes that have a string attribute
class AstAttrNode : public AstNode {
public:
	std::string get_name() { return name; }
	void set_name(std::string n) { name = n; }
protected:
	std::string name = "";
};

//Represents a scope
class AstScope : public AstAttrNode {
public:
	AstScope() { type = AstType::Scope; }
	std::map<std::string, Var> vars;
};

//The function declaration type
class AstFuncDec : public AstAttrNode {
public:
	explicit AstFuncDec() { type = AstType::FuncDec; }
	explicit AstFuncDec(std::string n) {
		type = AstType::FuncDec;
		name = n;
	}

	std::vector<Var> args;
	bool is_global = false;
	DataType rtype = DataType::Void;
};

//Extern function declarations
class AstExternFunc : public AstFuncDec {
public:
	explicit AstExternFunc() { type = AstType::ExternFunc; }
	explicit AstExternFunc(std::string n) {
		type = AstType::ExternFunc;
		name = n;
	}
};

//The function call type
class AstFuncCall : public AstAttrNode {
public:
	explicit AstFuncCall() { type = AstType::FuncCall; }
	explicit AstFuncCall(std::string n) {
		type = AstType::FuncCall;
		name = n;
	}
};

//The return keyword
class AstReturn : public AstNode {
public:
	AstReturn() { type = AstType::Return; }
	DataType d_type = DataType::Void;
};

//The base class for conditionals
class AstCond : public AstNode {
public:
	CondOp get_op() { return op; }
	void set_op(CondOp o) { op = o; }
	
	AstNode *lval;
	AstNode *rval;
private:
	CondOp op;
};

//The If keyword
class AstIf : public AstCond {
public:
	AstIf() { type = AstType::If; }
};

class AstElif : public AstCond {
public:
	AstElif() { type = AstType::Elif; }
};

//The else keyword
class AstElse : public AstNode {
public:
	AstElse() { type = AstType::Else; }
};

//The while keyword
class AstWhile : public AstCond {
public:
	AstWhile() { type = AstType::While; }
};

//The for keyword
class AstFor : public AstCond {
public:
	AstFor() { type = AstType::For; }
};

//The loop keyword
class AstLoop : public AstNode {
public:
	AstLoop() { type = AstType::Loop; }
	AstNode *param;
	std::string i_var = "";
};

//The foreach keyword
class AstForEach : public AstAttrNode {
public:
	AstForEach() { type = AstType::ForEach; }
	std::string i_var = "";		//Index
	std::string r_var = "";		//Range
	std::string i_var_in = "";	//Internal index counter
};

//Variable declaration
//initial value go as children
class AstVarDec : public AstAttrNode {
public:
	explicit AstVarDec() { type = AstType::VarDec; }
	explicit AstVarDec(std::string n) {
		type = AstType::VarDec;
		name = n;
	}
	
	DataType get_type() { return dtype; }
	void set_type(DataType t) { dtype = t; }
	
	bool is_ptr = false;
protected:
	DataType dtype;
};

//Variable assignment/operation
class AstVarAssign : public AstVarDec {
public:
	explicit AstVarAssign() { type = AstType::VarAssign; }
	explicit AstVarAssign(std::string n) {
		type = AstType::VarAssign;
		name = n;
	}
};

//Structure declarations
class AstStructDec : public AstAttrNode {
public:
	explicit AstStructDec() { type = AstType::StructDec; }
	explicit AstStructDec(std::string n) {
		type = AstType::StructDec;
		name = n;
	}
	
	std::vector<Var> fields;
};

//A struct variable
class AstStruct : public AstNode {
public:
	explicit AstStruct() { type = AstType::Struct; }
	
	std::string str_name = "";		//Refers to the structure being used
	std::string var_name = "";		//The name of our structure variable
};

//A struct access
class AstStructAcc : public AstStruct {
public:
	explicit AstStructAcc() { type = AstType::StructAcc; }
};

//A struct modification
class AstStructMod : public AstVarAssign {
public:
	explicit AstStructMod() { type = AstType::StructMod; }
	
	std::string str_name = "";		//Refers to the structure being used
	std::string var_name = "";		//The name of our structure variable
};

//Represents a math operation
class AstMath : public AstNode {
public:
	explicit AstMath() { type = AstType::Math; }
};

//The ID type
class AstID : public AstAttrNode {
public:
	explicit AstID() { type = AstType::Id; }
	explicit AstID(std::string n) {
		type = AstType::Id;
		name = n;
	}
	
	bool is_ref = false;	//Memory reference?
	bool is_ptr = false;	//Pointer?
};

//The Integer type
class AstInt : public AstNode {
public:
	explicit AstInt() { type = AstType::Int; }
	explicit AstInt(int i) {
		type = AstType::Int;
		no = i;
	}
	
	int get_val() { return no; }
	void set_val(int i) { no = i; }
private:
	int no = 0;
};

//The hex type
class AstHex : public AstNode {
public:
	explicit AstHex() { type = AstType::Hex; }
	explicit AstHex(unsigned short b) {
		type = AstType::Hex;
		byte = b;
	}
	
	unsigned short get_val() { return byte; }
	void set_val(unsigned short b) { byte = b; }
	
	void set_val(std::string str) {
		std::stringstream ss(str);
		ss.flags(std::ios_base::hex);
		ss >> byte;
	}
private:
	unsigned short byte;
};

//The char type
class AstChar : public AstNode {
public:
	explicit AstChar() { type = AstType::Char; }
	explicit AstChar(char c) {
		type = AstType::Char;
		ch = c;
	}
	
	char get_val() { return ch; }
	void set_val(char c) { ch = c; }
private:
	char ch = 0;
};

//The bool types
class AstBool : public AstNode {
public:
	explicit AstBool() { type = AstType::Bool; }
	explicit AstBool(bool b) {
		type = AstType::Bool;
		val = b;
	}
	
	bool get_val() { return val; }
	void set_val(bool b) { val = b; }
private:
	bool val = false;
};

//The float type
class AstFloat : public AstNode {
public:
	explicit AstFloat() { type = AstType::Float; }
	explicit AstFloat(float n) {
		type = AstType::Float;
		no = n;
	}
	
	float get_val() { return no; }
	void set_val(float n) { no = n; }
protected:
	float no = 0;
};

//The double type
class AstDouble : public AstNode {
public:
	explicit AstDouble() { type = AstType::Double; }
	explicit AstDouble(double n) {
		type = AstType::Double;
		no = n;
	}
	
	double get_val() { return no; }
	void set_val(double n) { no = n; }
protected:
	double no = 0;
};

//The string type
class AstString : public AstNode {
public:
	explicit AstString() { type = AstType::Str; }
	explicit AstString(std::string s) {
		type = AstType::Str;
		val = s;
	}
	
	std::string get_val() { return val; }
	void set_val(std::string s) { val = s; }
private:
	std::string val = "";
};

//The array type
class AstArrayDec : public AstVarDec {
public:
	explicit AstArrayDec() { type = AstType::ArrayDec; }
	
	int get_size() { return size; }
	void set_size(int s) { size = s; }
private:
	int size;
};

//Array access
class AstArrayAcc : public AstVarDec {
public:
	explicit AstArrayAcc() { type = AstType::ArrayAccess; }
	explicit AstArrayAcc(std::string n) {
		type = AstType::ArrayAccess;
		name = n;
	}
};

//Array assignment
class AstArrayAssign : public AstVarDec {
public:
	explicit AstArrayAssign() { type = AstType::ArrayAssign; }
	explicit AstArrayAssign(std::string n) {
		type = AstType::ArrayAssign;
		name = n;
	}
	
	AstNode *index;
};

//Debugging stuff
std::string ast2str(AstType type);
std::string type2str(DataType type);
void print_tree(AstNode *node, int indent = 0, bool nl=true);
