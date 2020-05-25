#pragma once

#include <string>

#include <base/cbase.hh>

#include <javabuilder.hh>
#include <javafile.hh>
#include <javapool.hh>

class JVM : public CompilerBase {
public:
	explicit JVM(std::string name);
	
	void code_init() {}
	void write() {
		builder->write();
	}
private:
	JavaBuilder *builder;
};
