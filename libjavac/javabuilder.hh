#pragma once

#include <string>

#include "java_ast.hh"
#include "javafile.hh"
#include "javapool.hh"
#include "javafunc.hh"

class JavaBuilder {
public:
	explicit JavaBuilder(std::string name);
	explicit JavaBuilder(JavaClass *clazz);
	void assemble(JavaClass *clazz);
	
	void useOutput();
	void useLibrary(std::string str);
	void addString(std::string str);
	void write();
	
	JavaFunc *createConstructor(bool init=false);
	JavaFunc *createMain();
	
	JavaFile *jfile;
	JavaPool *jpool;
protected:
	void init(std::string name);
	void buildMethod(JavaMethod *method, JavaFunc *target);
	
	//In the jfunccall file
	void buildVirtualFC(JCode *code, JavaMethod *method, JavaFunc *target);
	
	//In the jvar file
	void buildVarDec(JCode *code, JavaFunc *target);
	void buildIntAssign(JVar *var, JavaFunc *target);
private:
	std::string class_name = "";
};
