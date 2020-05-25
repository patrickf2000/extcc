#include <java_ast.hh>
#include <javabuilder.hh>

//Builds a variable declaration
void JavaBuilder::buildVarDec(JCode *code, JavaFunc *target) {
	auto vd = static_cast<JVar *>(code);
	
	switch (vd->d_type) {
		//Integers
		case JDataType::Int: {
			target->createIntVar(vd->name, 0);
			buildIntAssign(vd, target);
		} break;
		
		//TODO: Add the rest
	}
}

//Builds an integer variable assignment
void JavaBuilder::buildIntAssign(JVar *var, JavaFunc *target) {
	auto val = var->val;
	
	switch (val->type) {
		//Integers
		case JCodeType::Int: {
			auto i = static_cast<JInt *>(val);
			target->loadInt(i->val);
			target->storeIntVar(var->name);
		} break;
	}
}
