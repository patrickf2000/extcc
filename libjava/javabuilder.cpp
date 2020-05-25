#include "javabuilder.hh"

//Sets everything up
JavaBuilder::JavaBuilder(std::string name) {
	init(name);
}

JavaBuilder::JavaBuilder(JavaClass *clazz) {
	init(clazz->name);
}

//An assembler function used when we are given an AST
void JavaBuilder::assemble(JavaClass *clazz) {
	for (auto str : clazz->strings) {
		addString(str);
	}

	for (auto func : clazz->methods) {
		if (func->name == "main") {
			auto f = createMain();
			buildMethod(func, f);
			f->addSingle(JavaCode::RetVoid);
		} else {
			//TODO: Add interface for building functions
		}
	}
}

//Adds the common System.out.println libraries to the pool
// and enables their use
void JavaBuilder::useOutput() {
	jpool->useLibrary("java/lang/System");
	jpool->useLibrary("java/io/PrintStream");
	
	//Add a reference to the System.out static variable
	Ref ref;
	ref.base_lib = "java/lang/System";
	ref.name = "out";
	ref.type = "Ljava/io/PrintStream;";
	jpool->addStaticRef(ref);
	
	//Use println to print strings
	ref.base_lib = "java/io/PrintStream";
	ref.name = "println";
	ref.type = "(Ljava/lang/String;)V";
	jpool->useMethod(ref);
	
	//Use println to print ints
	ref.type = "(I)V";
	jpool->useMethod(ref);
}

//Adds a library to the constant pool and enables its use
void JavaBuilder::useLibrary(std::string str) {
	jpool->useLibrary(str);
}

//Adds a string to the constant pool and enables its use
void JavaBuilder::addString(std::string str) {
	jpool->addString(str);
}

//Writes everything to a class file
void JavaBuilder::write() {
	jfile->write(jpool);
}

//Creates the class constructor
JavaFunc *JavaBuilder::createConstructor(bool init) {
	//Init to the constructor if needed
	if (init) {
		jpool->addConstructor();
	}

	auto func = new JavaFunc("<init>", "()V");
	func->setPool(jpool);
	func->setAttributes(JFuncAttr::Public);
	
	func->addSingle(JavaCode::ALoad0);
	func->callFunc("<init>", "()V", FuncType::Special);
	
	jfile->addFunc(func);
	
	return func;
}

//Creates the main function and returns a reference to it
JavaFunc *JavaBuilder::createMain() {
	auto func = new JavaFunc("main", "([Ljava/lang/String;)V");
	func->setPool(jpool);
	func->setAttributes(JFuncAttr::Public);
	func->setAttributes(JFuncAttr::Static);
	
	jfile->addFunc(func);
	return func;
}

//The init function used by the constructors
void JavaBuilder::init(std::string name) {
	class_name = name;
	jfile = new JavaFile(name);
	jpool = new JavaPool(name);
	
	//Reference to the constructor
	Ref ref;
	ref.base_lib = "java/lang/Object";
	ref.base_name = class_name;
	ref.name = "<init>";
	ref.type = "()V";
	jpool->useMethod(ref);
	jpool->addAttribute("<init>");
	jpool->addAttribute("()V");
	
	//Required attributes
	jpool->addAttribute("Code");
	jpool->addAttribute("LineNumberTable");
	
	//Reference to the main function
	jpool->addAttribute("main");
	jpool->addAttribute("([Ljava/lang/String;)V");
}

//Builds a Java method
void JavaBuilder::buildMethod(JavaMethod *method, JavaFunc *target) {
	for (auto code : method->code) {
		switch (code->type) {
			//Virtual function call
			case JCodeType::VirtualFuncCall: {
				buildVirtualFC(code, method, target);
			} break;
			
			//Variable declaration
			case JCodeType::VarDec: {
				buildVarDec(code, target);
			} break;
		}
	}
}


