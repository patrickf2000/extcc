#include <iostream>
#include <fstream>
#include <cstdlib>

#include <ast.hh>
#include <ltac/ltac.hh>
#include <ltac/ltac_build.hh>

#ifdef BUILD_PASM
#include <pasm/pasm.hh>
#include <pasm/pasm_builder.hh>
#endif

#include "compiler.hh"
#include "utils.hh"

#ifdef BUILD_PASM
#include "x86-64/asm_x64.hh"
#else
#include "x64/asm_x64.hh"
#include "i386/asm_i386.hh"
#include "arm7/asm_arm7.hh"
#include "llvm/asm_llvm.hh"
#endif

#ifdef BUILD_CLANG
#include <c_parser.hh>
#elif BUILD_ASM
#include <lasm_parser.hh>
#endif

Compiler::Compiler(Config c) {
	config = c;
}

//Setup all the inputs
void Compiler::set_inputs(std::vector<std::string> inputs) {
	src_files = inputs;

	for (auto in : inputs) {
		std::string path = get_path(in);
		std::string base = get_basename(in);
		
		std::string asm_path = "/tmp/" + base + ".asm";
		std::string obj_path = "/tmp/" + base + ".o";
		
		if (config.arch == CpuArch::LLVM)
			asm_path = "/tmp/" + base;
		
		asm_files.push_back(asm_path);
		obj_files.push_back(obj_path);
	}
}

//Generates assembly
void Compiler::assemble() {
	for (int i = 0; i<src_files.size(); i++) {
#ifdef BUILD_CLANG
		CParser parser(src_files[i]);
		parser.parse();
		parser.runSyntax();
		AstNode *top = parser.getTree();
#elif BUILD_ASM
		AsmParser parser(src_files[i]);
		parser.parse();
		LtacFile *file = parser.getFile();
#else
#ifndef BUILD_PASM
#error Unknown Compiler
#endif
#endif

		int ptr_size = 8;
		if (config.arch == CpuArch::Arm7)
			ptr_size = 4;
		
#ifndef BUILD_ASM
#ifdef BUILD_PASM
		PasmBuilder *builder = new PasmBuilder("out.asm");
		PasmFile *file = builder->buildFile(top);
#else
		LTAC_Builder *builder = new LTAC_Builder;
		builder->set_ptr_size(ptr_size);
		LtacFile *file = builder->build_file(top);
#endif
#endif
		file->name = asm_files[i];
		
		switch (config.arch) {
			case CpuArch::Intel64: {
#ifdef BUILD_PASM
				X64 asm_builder(file->name);
				asm_builder.build_code(file);
				asm_builder.write();
#else
				Asm_x64 asm_builder(file);
				
				if (config.out_type == BuildType::DynLib)
					asm_builder.build_PIC();
				
				asm_builder.write();
#endif
			} break;
			
#ifndef BUILD_PASM
			case CpuArch::Intel32: {
				Asm_i386 asm_builder(file);
				
				if (config.out_type == BuildType::DynLib)
					asm_builder.build_PIC();
				
				asm_builder.write();
			} break;
			
			case CpuArch::Arm7: {
				Asm_Arm7 asm_builder(file);
				
				if (config.out_type == BuildType::DynLib)
					asm_builder.build_PIC();
					
				asm_builder.write(false);
			} break;
			
			case CpuArch::LLVM: {
				file->name = asm_files[i] + ".ll";
				Asm_LLVM llvm_builder(file);
				llvm_builder.write(false, true);
			} break;
#endif
		}
		
#ifndef BUILD_ASM
		delete top;
		delete builder;
#endif
		delete file;
	}
}

//Assemble the assembly
//TODO: Update not to use system calls
void Compiler::compile() {
	for (int i = 0; i<asm_files.size(); i++) {
		std::string cmd = "as " + asm_files[i] + " -o ";
		cmd += obj_files[i];
		
		if (config.arch == CpuArch::LLVM) {
			cmd = "llc " + asm_files[i] + ".ll -o ";
			cmd += asm_files[i] + ".asm";
			system(cmd.c_str());
			
			cmd = "as " + asm_files[i] + ".asm -o ";
			cmd += obj_files[i];
		}
		
		if (config.arch == CpuArch::Intel32)
			cmd += " --32";
		
		system(cmd.c_str());
	}
}

//Link the object files
//TODO: Do not use the system() function
void Compiler::link() {
	std::string ld_line = "ld ";

	//Link an executable
	if (config.out_type == BuildType::Exe) {
		if (config.arch == CpuArch::Arm7) {
			ld_line += "/usr/lib/arm-linux-gnueabihf/crti.o ";
			ld_line += "/usr/lib/arm-linux-gnueabihf/crtn.o ";
			ld_line += "/usr/lib/arm-linux-gnueabihf/crt1.o ";
		} else if (config.arch == CpuArch::Intel32) {
			ld_line += "-melf_i386 ";
			ld_line += "/usr/lib32/crti.o ";
			ld_line += "/usr/lib32/crtn.o ";
			ld_line += "/usr/lib32/crt1.o ";
		} else {
#ifndef BUILD_PASM
			ld_line += "/usr/lib/x86_64-linux-gnu/crti.o ";
			ld_line += "/usr/lib/x86_64-linux-gnu/crtn.o ";
			ld_line += "/usr/lib/x86_64-linux-gnu/crt1.o ";
#endif
		}
		
#ifndef BUILD_PASM
		ld_line += "-lc ";
#endif

		for (auto obj : obj_files) {
			ld_line += obj + " ";
		}
		
#ifndef BUILD_PASM
		if (config.arch == CpuArch::Arm7)
			ld_line += "-dynamic-linker /lib/ld-linux-armhf.so.3 ";
		else if (config.arch == CpuArch::Intel32)
			ld_line += "-dynamic-linker /lib32/ld-linux.so.2 ";
		else
			ld_line += "-dynamic-linker /lib64/ld-linux-x86-64.so.2 ";
#endif
		
		ld_line += "-o " + config.output;
		
	//Link a dynamic library
	} else if (config.out_type == BuildType::DynLib) {
		for (auto obj : obj_files) {
			ld_line += obj + " ";
		}
		
		ld_line += "-o lib" + config.output + ".so ";
		ld_line += "-shared -lc";
	}
	
	system(ld_line.c_str());
}


