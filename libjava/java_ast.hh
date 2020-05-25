#pragma once

#include <string>
#include <vector>

//Modifiers for class, functions, variables, etc
enum class JavaModifier {
	None,
	Public,
	Protected,
	Private,
	Static,
	Native
};

//Represents Java primitive data types
// Note: Because we use String so much, I'm including it here
enum class JDataType {
	None,
	Void,
	
	Byte,		//8-bit int
	Short,		//16-bit int
	Int,		//32-bit int
	Long,		//64-bit int
	Float,		//32-bit float
	Double,		//64-bit float
	Boolean,	//1-bit, true or false
	Char,		//16-bit UTF-16
	
	Object,
	String
};

//Represents the type of Java code (AST-specific)
enum class JCodeType {
	None,
	
	VirtualFuncCall,
	
	Var,
	VarDec,
	
	Int,
	String
};

//Represents a piece of Java Code
class JCode {
public:
	explicit JCode() {}
	explicit JCode(JCodeType type) {
		this->type = type;
	}
	
	JCodeType type = JCodeType::None;
	std::vector<JCode *> children;
};

//Represents a Java method
class JavaMethod {
public:
	explicit JavaMethod() {}
	explicit JavaMethod(std::string name) {
		this->name = name;
	}
	
	std::string name;
	std::string signature = "()V";
	
	std::vector<JCode *> code;
};

//Represents the Java main method (convience class)
class JavaMain : public JavaMethod {
public:
	JavaMain() {
		name = "main";
		signature = "([Ljava/lang/String;)V";
	}
};

//Represents a Java class file
class JavaClass {
public:
	explicit JavaClass(std::string name) {
		this->name = name;
	}

	std::string name;
	std::vector<JavaMethod *> methods;
	std::vector<std::string> strings;
};

//Represents a function class
class JFuncCall : public JCode {
public:
	std::string name = "";
	std::string signature = "()V";
	std::vector<JCode *> args;
};

//Represents a static function call
class JVirtualFuncCall : public JFuncCall {
public:
	explicit JVirtualFuncCall() {
		type = JCodeType::VirtualFuncCall;
	}
	
	explicit JVirtualFuncCall(std::string base, std::string name, std::string sig) {
		this->base = base;
		this->name = name;
		this->signature = sig;
		type = JCodeType::VirtualFuncCall;
	}
	
	std::string base;
};

//Represents a variable
class JVar : public JCode {
public:
	explicit JVar() {
		type = JCodeType::Var;
	}
	
	explicit JVar(std::string name, JDataType t) {
		type = JCodeType::Var;
		this->d_type = t;
		this->name = name;
	}
	
	std::string name = "";
	JDataType d_type = JDataType::None;
	JCode *val = new JCode;
};

//Represents a variable declaration
class JVarDec : public JVar {
public:
	explicit JVarDec() { type = JCodeType::VarDec; }
	explicit JVarDec(std::string name, JDataType t) {
		type = JCodeType::VarDec;
		this->d_type = t;
		this->name = name;
	}
};

//Represents an integer
class JInt : public JCode {
public:
	explicit JInt(int i) {
		type = JCodeType::Int;
		val = i;
	}
	
	int val = 0;
};

//Represents a Java String
class JString : public JCode {
public:
	explicit JString() {
		type = JCodeType::String;
	}
	
	explicit JString(std::string val) {
		this->val = val;
		type = JCodeType::String;
	}
	
	std::string val;
};

