#pragma once

#include <vector>
#include <string>
#include <map>

#include "types.hh"

struct Ref {
	std::string base_lib;
	std::string base_name;
	std::string name;
	std::string type;
};

class JavaPool {
public:
	JavaPool(std::string class_name);
	
	void useLibrary(std::string lib);
	void addString(std::string str);
	void addStaticRef(Ref ref);
	void useMethod(Ref ref, bool internal=false);
	void addConstructor();
	void addAttribute(std::string attr);
	
	//TODO: Move me
	std::vector<unsigned char> getCode() {
		return code;
	}
	
	int getPoolSize() {
		return pos;
	}
	
	std::map<std::string, int> pool;
	std::vector<FuncRef> funcRefs;
protected:
	void addRef(Ref ref, bool internal=false);
	void addStringRef(std::string str, OpType type, bool use_pos);
private:
	std::string class_name;
	std::vector<unsigned char> code;
	int pos = 1;
	
	int cpos = 0;
	int cnt_pos = -1;
};
