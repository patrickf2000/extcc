#include <iostream>

#include "javapool.hh"

//Adds the class name and the super class to the pool
JavaPool::JavaPool(std::string name) {
	class_name = name;
	addStringRef(class_name, OpType::Ref, true);
	pool[class_name] = pos;
	//pool[class_name] = pos + 1;
	cpos = pos;
	pos += 2;
	
	addStringRef("java/lang/Object", OpType::Ref, true);
	pool["java/lang/Object"] = pos;
	pos += 2;
}

//Imports a library to be used
void JavaPool::useLibrary(std::string lib) {
	pool[lib] = pos;
	addStringRef(lib, OpType::Ref, true);
	pos += 2;
}

//Adds a string constant to the pool
void JavaPool::addString(std::string str) {
	pool[str] = pos;
	addStringRef(str, OpType::String, true);
	pos += 2;
}

//Adds a static reference
void JavaPool::addStaticRef(Ref ref) {
	code.push_back((unsigned char)OpType::FieldRef);
	code.push_back(0x00);
	addRef(ref);
}

//Add a method reference
void JavaPool::useMethod(Ref ref, bool internal) {
	FuncRef func;
	func.pos = pos;
	func.type = ref.type;
	func.name = ref.name;
	funcRefs.push_back(func);

	code.push_back((unsigned char)OpType::MethodRef);
	code.push_back(0x00);
	addRef(ref, internal);
}

//Adds a reference to the constructor
void JavaPool::addConstructor() {
	pos += 1;
	code.push_back((unsigned char)OpType::MethodRef);
	code.push_back(0x00);
	
	code.push_back((unsigned char)cpos);
	code.push_back(0x00);
	code.push_back((unsigned char)cnt_pos);
	
	FuncRef func;
	func.pos = pos - 1;
	func.type = "()V";
	func.name = class_name;
	funcRefs.push_back(func);
}

//Add an attribute
void JavaPool::addAttribute(std::string attr) {
	pool[attr] = pos;
	addStringRef(attr, OpType::None, false);
	++pos;
}

void JavaPool::addRef(Ref ref, bool internal) {
	int index = pool[ref.base_lib];
	if (internal) index = pool[ref.base_lib] - 1;
	
	code.push_back((unsigned char)index);
	code.push_back(0x00);
	code.push_back((unsigned char)pos + 1);

	pool[ref.name] = pos;
	pool[ref.type] = pos + 1;
	
	pos += 2;
	
	//Add name type
	//TODO: Can probably create separate func
	if (ref.name == "<init>" && cnt_pos == -1)
		cnt_pos = pos - 1;
		
	code.push_back((unsigned char)OpType::NameType);
	code.push_back(0x00);
	
	code.push_back((unsigned char)pos);
	code.push_back(0x00);
	code.push_back((unsigned char)pos + 1);
	
	pos += 2;
	
	addStringRef(ref.name, OpType::None, false);
	addStringRef(ref.type, OpType::None, false);
}

//Adds a string based reference to the pool
// TYPE 0x00 POS PADDING FIELD
void JavaPool::addStringRef(std::string str, OpType type, bool use_pos) {
	//Write the type
	if (type != OpType::None)
		code.push_back((unsigned char)type);

	//Write the position
	if (use_pos) {
		code.push_back(0x00);
		code.push_back((unsigned char)pos + 1);
	}
	
	code.push_back(0x01);
	code.push_back(0x00);
	code.push_back((unsigned char)str.length());

	for (auto c : str) {
		code.push_back(c);
	}
}
