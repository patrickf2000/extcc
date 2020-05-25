#include "javafunc.hh"

//The constructor and setup functions
JavaFunc::JavaFunc(std::string name, std::string type) {
	this->name = name;
	this->type = type;
}
	
void JavaFunc::setPool(JavaPool *pool) {
	this->pool = pool->pool;
	refs = pool->funcRefs;
}

//Attributes
void JavaFunc::setAttributes(JFuncAttr attr) {
	attributes += (unsigned char)attr;
}

unsigned char JavaFunc::getAttributes() {
	return attributes;
}

//Load a static reference from the pool
void JavaFunc::getStatic(std::string name) {
	short loco = (short)pool[name];
	
	unsigned char buf[2];
	buf[0] = (loco & 0xff00) >> 8;
	buf[1] = (loco & 0x00ff);
	
	code.push_back(0xB2);
	code.push_back(buf[0]);
	code.push_back(buf[1]);
}

//Load a string constant to the stack
void JavaFunc::loadStrConst(std::string name) {
	int loco = pool[name];
	
	code.push_back(0x12);
	code.push_back((unsigned char)loco);
}

//Invoke a function
void JavaFunc::callFunc(std::string name, std::string type, FuncType ftype) {
	short loco = 0;

	for (auto func : refs) {
		if (func.name == name && func.type == type) {
			loco = func.pos;
		}
	}
	
	unsigned char buf[2];
	buf[0] = (loco & 0xff00) >> 8;
	buf[1] = (loco & 0x00ff);
	
	code.push_back((unsigned char)ftype);
	code.push_back(buf[0]);
	code.push_back(buf[1]);
}

//Adds a single bytecode (one without arguments)
void JavaFunc::addSingle(JavaCode c) {
	code.push_back((unsigned char)c);
}

//Class functions
//=================================================

//Create a new class and call the constructor
void JavaFunc::initClass(std::string name, std::string v_name) {
	short loco = pool[name];
	
	unsigned char buf[2];
	buf[0] = (loco & 0xff00) >> 8;
	buf[1] = (loco & 0x00ff);
	
	code.push_back(0xBB);
	code.push_back(buf[0]);
	code.push_back(buf[1]);
	
	code.push_back(0x59);
	
	obj_vars[v_name] = var_index;
	++var_index;
	++locals;
}

//Store a class variable
void JavaFunc::storeClassVar(std::string name) {
	int pos = obj_vars[name];
	
	switch (pos) {
		case 1: code.push_back(0x4C); break;
		case 2: code.push_back(0x4D); break;
		case 3: code.push_back(0x4E); break;
		default: {
			code.push_back(0x3A);
			code.push_back((unsigned char)pos);
		}
	}
}

//Load a class variable to the stack
void JavaFunc::loadClassVar(std::string name) {
	int pos = obj_vars[name];
	
	switch (pos) {
		case 1: code.push_back(0x2B); break;
		case 2: code.push_back(0x2C); break;
		case 3: code.push_back(0x2D); break;
		default: {
			code.push_back(0x19);
			code.push_back((unsigned char)pos);
		}
	}
}

//Integer functions
//=================================================

//Load an integer
void JavaFunc::loadInt(int val) {
	code.push_back(0x10);
	code.push_back((unsigned char)val);
}

//Loads an integer variable to the stack
void JavaFunc::loadIntVar(std::string var) {
	int pos = int_vars[var];
	
	switch (pos) {
		case 1: code.push_back(0x1B); break;
		case 2: code.push_back(0x1C); break;
		case 3: code.push_back(0x1D); break;
		default: {
			code.push_back(0x15);
			code.push_back((unsigned char)pos);
		}
	}
}

//Stores an integer variable from the stack
void JavaFunc::storeIntVar(std::string var) {
	int pos = int_vars[var];

	switch (pos) {
		case 1: code.push_back(0x3C); break;
		case 2: code.push_back(0x3D); break;
		case 3: code.push_back(0x3E); break;
		default: {
			code.push_back(0x36);
			code.push_back((unsigned char)pos);
		}
	}
}

//Stores a value to an integer variable
void JavaFunc::storeIntVar(std::string var, int val) {
	loadInt(val);
	storeIntVar(var);
}

//Create an integer variable
void JavaFunc::createIntVar(std::string var, int val) {
	int_vars[var] = var_index;
	++var_index;
	++locals;
	
	storeIntVar(var, val);
}



