#pragma once

enum class OpType {
	None = 0x00,
	Ref = 0x07,
	String = 0x08,
	FieldRef = 0x09,
	NameType = 0x0C,
	MethodRef = 0x0A
};

struct FuncRef {
	std::string name;
	std::string type;
	int pos;
};
