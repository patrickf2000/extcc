#include "jvm.hh"

JVM::JVM(std::string name) {
	builder = new JavaBuilder(name);
	builder->useOutput();
}
