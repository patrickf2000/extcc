#pragma once

#include <string>
#include <vector>
#include <map>

#include "javapool.hh"
#include "writer.hh"
#include "javafunc.hh"
#include "types.hh"

/*struct Constant {
	std::string field;
	std::vector<unsigned char> padding;
	int pos;
	OpType type;
};*/

class JavaFile {
public:
	JavaFile(std::string cname);
	
	void addFunc(JavaFunc *fc) {
		funcs.push_back(fc);
	}
	
	void writeFuncs(JavaPool *pool);
	
	void write(JavaPool *pool);
private:
	BinWriter *writer;
	std::string class_name = "";
	
	std::vector<JavaFunc *> funcs;
	std::map<std::string, int> labels;
};
