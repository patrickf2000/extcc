#include "jvm.hh"

JVM::JVM(std::string name) {
	builder = new JavaBuilder(name);
	builder->useOutput();
}

//Build the data section
void JVM::build_data(PasmFile *file) {
	for (auto item : file->data) {
		auto str = static_cast<PasmString *>(item);
	
		switch (item->type) {
			case pasm::String: builder->addString(str->val); break;
		}
	}
}

//Build function declaration
void JVM::build_func(PasmNode *ln) {

}

//Build function return
void JVM::build_ret() {

}

//Build function call
void JVM::build_call(PasmNode *ln) {

}
