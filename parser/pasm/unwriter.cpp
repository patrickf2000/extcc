#include <pasm/pasm.hh>
#include <iostream>

namespace PASM {

std::string getType(DType type) {
	switch (type) {
		case DType::Byte: return "b";
		case DType::Short: return "s";
		case DType::Int: return "i";
		case DType::Float32: return "f32";
		case DType::Float64: return "f64";
		case DType::String: return "str";
		case DType::Ptr: return "ptr";
	}

	return "";
}

std::string getMath(MathType type) {
	switch (type) {
		case MathType::Add: return "add";
		case MathType::Sub: return "sub";
		case MathType::Mul: return "mul";
		case MathType::Div: return "div";
	}
	
	return "";
}

//Generates a string from PASM tree
std::string unwrite(PasmFile *file) {
	std::string ret = "";
	
	//Build the data section
	ret += "section data\n";
	
	for (auto ln : file->data) {
		auto str = static_cast<PasmString *>(ln);
		
		switch (ln->type) {
			//Strings
			case pasm::String: {
				ret += "\tstring " + str->name + " \"" + str->val + "\"\n";
			} break;
			
			//Floats
			case pasm::Float: {
				ret += "\tfloat " + str->name + " " + str->val + "\n";
			} break;
			
			//Doubles
			case pasm::Double: {
				ret += "\tdouble " + str->name + " " + str->val + "\n";
			} break;
		}
	}
	
	//Build the code section
	ret += "\nsection text\n";
	
	for (auto ln : file->code) {
		switch (ln->type) {
			//Space
			case pasm::Space: {
				ret += "\n";
			} break;
			
			//Labels
			case pasm::Lbl: {
				auto lbl = static_cast<Label *>(ln);
				ret += "lbl " + lbl->name + "\n";
			} break;
		
			//Functions
			case pasm::Func: {
				auto func = static_cast<Func *>(ln);
				ret += "func " + func->name;
				
				if (func->stackSize > 0)
					ret += "(" + std::to_string(func->stackSize) + ")";
					
				ret += "\n";
			} break;
			
			//Return
			case pasm::Ret: ret += "\tret\n\n"; break;
			
			//Pusharg
			case pasm::IPushArg: {
				auto pusharg = static_cast<IPushArg *>(ln);
				ret += "\ti.pusharg_";
				
				switch (pusharg->opType) {
					case Operand::Var: ret += "v VAR"; break;
					case Operand::Reg: ret += "r "; break;
					case Operand::Const: ret += "c "; break;
				}
				
				ret += std::to_string(pusharg->val) + "\n";
			} break;
			
			case pasm::F32_PushArg: {
				auto pusharg = static_cast<F32_PushArg *>(ln);
				ret += "\tf32.pusharg_";
				
				switch (pusharg->opType) {
					case Operand::Var: ret += "v VAR"; break;
					case Operand::Reg: ret += "r "; break;
					case Operand::Const: ret += "c "; break;
				}
				
				if (pusharg->opType == Operand::Const)
					ret += pusharg->val + "\n";
				else
					ret += std::to_string(pusharg->pos) + "\n";
			} break;
			
			case pasm::F64_PushArg: {
				auto pusharg = static_cast<F64_PushArg *>(ln);
				ret += "\tf64.pusharg_";
				
				switch (pusharg->opType) {
					case Operand::Var: ret += "v VAR"; break;
					case Operand::Reg: ret += "r "; break;
					case Operand::Const: ret += "c "; break;
				}
				
				if (pusharg->opType == Operand::Const)
					ret += pusharg->val + "\n";
				else
					ret += std::to_string(pusharg->pos) + "\n";
			} break;
			
			case pasm::StrPushArg: {
				auto pusharg = static_cast<StrPushArg *>(ln);
				ret += "\tstr.pusharg " + pusharg->name + "\n";
			} break;
			
			case pasm::Ptr_PushArg: {
				auto pusharg = static_cast<Ptr_PushArg *>(ln);
				ret += "\tptr.pusharg VAR" + std::to_string(pusharg->pos) + "\n";
			} break;
			
			//Function call
			case pasm::FuncCall: {
				auto call = static_cast<FuncCall *>(ln);
				ret += "\tcall " + call->name + "\n";
			} break;
			
			//Load integer function argument
			case pasm::ILdArg: {
				auto arg = static_cast<ILdArg *>(ln);
				ret += "\ti.ldarg VAR" + std::to_string(arg->pos) + "\n";
			} break;
			
			//Load float-32 function argument
			case pasm::F32_LdArg: {
				auto arg = static_cast<F32_LdArg *>(ln);
				ret += "\tf32.ldarg VAR" + std::to_string(arg->pos) + "\n";
			} break;
			
			//Load float-64 function argument
			case pasm::F64_LdArg: {
				auto arg = static_cast<F64_LdArg *>(ln);
				ret += "\tf64.ldarg VAR" + std::to_string(arg->pos) + "\n";
			} break;
			
			//Load pointer function argument
			case pasm::Ptr_LdArg: {
				auto arg = static_cast<Ptr_LdArg *>(ln);
				ret += "\tptr.ldarg VAR" + std::to_string(arg->pos) + "\n";
			} break;
			
			//Load/Store
			//Store constant (byte)
			case pasm::BStoreC: {
				auto store = static_cast<BStoreConst *>(ln);
				ret += "\tb.store_c VAR" + std::to_string(store->pos);
				ret += " " + std::to_string(store->val) + "\n";
			} break;
			
			//Store constant (int)
			case pasm::IStoreC: {
				auto store = static_cast<IStoreConst *>(ln);
				ret += "\ti.store_c VAR" + std::to_string(store->pos);
				ret += " " + std::to_string(store->val) + "\n";
			} break;
			
			//Store constant (float-32)
			case pasm::F32_StoreC: {
				auto store = static_cast<F32_StoreConst *>(ln);
				ret += "\tf32.store_c VAR" + std::to_string(store->pos);
				ret += " " + store->name + "\n";
			} break;
			
			//Store constant (float-64)
			case pasm::F64_StoreC: {
				auto store = static_cast<F64_StoreConst *>(ln);
				ret += "\tf64.store_c VAR" + std::to_string(store->pos);
				ret += " " + store->name + "\n";
			} break;
			
			//Store constant (string)
			case pasm::Str_StoreC: {
				auto store = static_cast<Str_StoreConst *>(ln);
				ret += "\tstr.store_c VAR" + std::to_string(store->pos);
				ret += " " + store->name + "\n";
			} break;
			
			//Move one var to another
			case pasm::MoveVV: {
				auto move = static_cast<MoveVV *>(ln);
				ret += "\t" + getType(move->dType) + ".move_v VAR";
				ret += std::to_string(move->pos1) + " VAR" + std::to_string(move->pos2);
				ret += "\n";
			} break;
			
			//Loads an integer register
			case pasm::ILdr: {
				auto load = static_cast<ILdr *>(ln);
				ret += "\ti.ldr_";
				
				switch (load->opType) {
					case Operand::Var: ret += "v r"; break;
					case Operand::Const: ret += "c r"; break;
				}
				
				ret += std::to_string(load->reg) + " ";
				if (load->opType == Operand::Var) ret += "VAR";
				
				ret += std::to_string(load->pos) + "\n";
			} break;
			
			//Loads a pointer element
			case pasm::PtrLd: {
				auto load = static_cast<PtrLd *>(ln);
				ret += "\tptr.ld VAR";
				ret += std::to_string(load->ptrPos) + "[";
				
				switch (load->posType) {
					case Operand::Var: ret += "VAR"; break;
					case Operand::Reg: ret += "r"; break;
				}
				
				ret += std::to_string(load->pos) + "]\n";
			} break;
			
			//Store a register to variable
			case pasm::Str: {
				auto str = static_cast<Str *>(ln);
				ret += "\t" + getType(str->dType) + ".str VAR";
				ret += std::to_string(str->pos) + " r" + std::to_string(str->reg);
				ret += "\n";
			} break;
			
			//Store value to integer pointer
			case pasm::PtrStr: {
				auto store = static_cast<PtrStr *>(ln);
				ret += "\tptr.str_";
				
				switch (store->opType) {
					case Operand::Var: ret += "v VAR"; break;
					case Operand::Reg: ret += "r r"; break;
					case Operand::Const: ret += "c "; break;
				}
				
				ret += std::to_string(store->src) + " VAR";
				ret += std::to_string(store->ptrPos) + "[";
				
				switch (store->posType) {
					case Operand::Var: ret += "VAR"; break;
					case Operand::Reg: ret += "r"; break;
				}
				
				ret += std::to_string(store->pos) + "]\n";
			} break;
			
			//Load integer to return register
			case pasm::ILdRet: {
				auto store = static_cast<ILdRet *>(ln);
				ret += "\ti.ldret_";
				
				switch (store->opType) {
					case Operand::Var: ret += "v VAR"; break;
					case Operand::Reg: ret += "r "; break;
					case Operand::Const: ret += "c "; break;
				}
				
				ret += std::to_string(store->val) + "\n";
			} break;
			
			//Store integer from return register
			case pasm::IStrRet: {
				auto store = static_cast<IStrRet *>(ln);
				ret += "\ti.stret_";
				
				switch (store->opType) {
					case Operand::Var: ret += "v VAR"; break;
					case Operand::Reg: ret += "r r"; break;
				}
				
				ret += std::to_string(store->val) + "\n";
			} break;
			
			//Store pointer from return register
			case pasm::Ptr_StrRet: {
				auto store = static_cast<Ptr_StrRet *>(ln);
				ret += "\tptr.stret_";
				
				switch (store->opType) {
					case Operand::Var: ret += "v VAR"; break;
					case Operand::Reg: ret += "r r"; break;
				}
				
				ret += std::to_string(store->val) + "\n";
			} break;
			
			//Store loaded value from pointer to integer
			case pasm::IStrPtr: {
				auto store = static_cast<IStrPtr *>(ln);
				ret += "\ti.strptr_";
				
				switch (store->opType) {
					case Operand::Var: ret += "v VAR"; break;
					case Operand::Reg: ret += "r r"; break;
				}
				
				ret += std::to_string(store->val) + "\n";
			} break;
			
			//Math- register <- immediate
			case pasm::IMathRI: {
				auto math = static_cast<IMathRI *>(ln);
				ret += "\ti." + getMath(math->mType)  + "_ri r" + std::to_string(math->reg);
				ret += " " + std::to_string(math->val) + "\n";
			} break;
			
			//Math- register <- variable
			case pasm::IMathRV: {
				auto math = static_cast<IMathRV *>(ln);
				ret += "\ti." + getMath(math->mType) + "_rv r" + std::to_string(math->reg);
				ret += " VAR" + std::to_string(math->pos) + "\n";
			} break;
			
			//Math- register <- register
			case pasm::IMathRR: {
				auto math = static_cast<IMathRR *>(ln);
				ret += "\ti." + getMath(math->mType) + "_rr r" + std::to_string(math->reg1);
				ret += " r" + std::to_string(math->reg2) + "\n";
			} break;
			
			//Math- variable <- immediate
			case pasm::IMathVI: {
				auto math = static_cast<IMathVI *>(ln);
				ret += "\ti." + getMath(math->mType) + "_vi VAR" + std::to_string(math->pos);
				ret += " " + std::to_string(math->val) + "\n";
			} break;
			
			//System call arguments
			case pasm::ISysarg: {
				auto arg = static_cast<ISysArg *>(ln);
				ret += "\ti.sysarg " + std::to_string(arg->arg) + "\n";
			} break;
			
			case pasm::StrSysarg: {
				auto arg = static_cast<StrSysArg *>(ln);
				ret += "\tstr.sysarg " + arg->arg + "\n";
			} break;
			
			//The syscall command
			case pasm::Syscall: ret += "\tsyscall\n\n"; break;
			
			//Branch
			case pasm::Br: {
				auto br = static_cast<Branch *>(ln);
				ret += "\t";
				
				switch (br->jmp) {
					case JmpType::Eq: ret += "beq "; break;
					case JmpType::Neq: ret += "bneq "; break;
					case JmpType::Z: ret += "bz "; break;
					case JmpType::Nz: ret += "bnz "; break;
					case JmpType::G: ret += "bg "; break;
					case JmpType::Ge: ret += "bge "; break;
					case JmpType::L: ret += "bl "; break;
					case JmpType::Le: ret += "ble "; break;
					default: ret += "br ";
				}
				
				ret += br->label + "\n";
			} break;
			
			//Integer comparisons
			case pasm::ICmp: {
				auto cmp = static_cast<ICmp *>(ln);
				std::string arg1 = "";
				std::string arg2 = "";
				
				switch (cmp->op1) {
					case Operand::Var: arg1 = "VAR"; break;
					case Operand::Reg: arg1 = "r"; break;
					case Operand::Const: break;
				}
				
				switch (cmp->op2) {
					case Operand::Var: arg2 = "VAR"; break;
					case Operand::Reg: arg2 = "r"; break;
					case Operand::Const: break;
				}
				
				arg1 += std::to_string(cmp->pos1);
				arg2 += std::to_string(cmp->pos2);
				
				ret += "\ti.cmp " + arg1 + " " + arg2 + "\n";
			} break;
		}
	}
	
	return ret;
}

}
