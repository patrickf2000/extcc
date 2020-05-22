#include <iostream>

#include <pasm/pasm.hh>

#include <x86-64/asm_x64.hh>

using namespace PASM;

int main(int argc, char *argv[]) {
	auto *file = new PasmFile;
	file->path = "output.asm";
	
	auto *main_fc = new Func("_start");
	file->code.push_back(main_fc);
	
	auto *arg1 = new ISysArg(60);
	auto *arg2 = new ISysArg(23);
	auto *syscall = new SysCall;
	
	file->code.push_back(arg1);
	file->code.push_back(arg2);
	file->code.push_back(syscall);
	
	auto writer = new X64("/tmp/out.asm");
	writer->write();
	
	return 0;
}
