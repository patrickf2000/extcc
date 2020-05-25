#include <iostream>
#include <fstream>
#include <cstdlib>

#include <ast.hh>
#include <pasm/pasm.hh>
#include <pasm/pasm_builder.hh>

#include "compiler.hh"
#include "utils.hh"
#include "base/cbase.hh"
#include "x86-64/asm_x64.hh"

#include <c_parser.hh>

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
		CParser parser(src_files[i]);
		parser.parse();
		parser.runSyntax();
		AstNode *top = parser.getTree();

		int ptr_size = 8;
		if (config.arch == CpuArch::Arm7)
			ptr_size = 4;

		PasmBuilder *builder = new PasmBuilder("out.asm");
		PasmFile *file = builder->buildFile(top);
		
		file->name = asm_files[i];
		
		CompilerBase *cc;
		
		switch (config.arch) {
			case CpuArch::Intel64: {
				cc = new X64(file->name);
				/*X64 asm_builder(file->name);
				asm_builder.build_data(file);
				asm_builder.build_code(file);
				asm_builder.write();*/

				//if (config.out_type == BuildType::DynLib)
				//	asm_builder.build_PIC();
			} break;
			

			/*case CpuArch::Intel32: {
				Asm_i386 asm_builder(file);
				
				if (config.out_type == BuildType::DynLib)
					asm_builder.build_PIC();
				
				asm_builder.write();
			} break;*/
			
			/*case CpuArch::Arm7: {
				Asm_Arm7 asm_builder(file);
				
				if (config.out_type == BuildType::DynLib)
					asm_builder.build_PIC();
					
				asm_builder.write(false);
			} break;*/
			
			/*case CpuArch::LLVM: {
				file->name = asm_files[i] + ".ll";
				Asm_LLVM llvm_builder(file);
				llvm_builder.write(false, true);
			} break;*/
		}
		
		cc->build_data(file);
		cc->build_code(file);
		cc->write();

		delete top;
		delete builder;
		delete file;
	}
}

//Assemble the assembly
//TODO: Update not to use system calls
void Compiler::compile() {
	for (int i = 0; i<asm_files.size(); i++) {
		std::string cmd = "as -g " + asm_files[i] + " -o ";
		cmd += obj_files[i];
		
		if (config.arch == CpuArch::LLVM) {
			cmd = "llc " + asm_files[i] + ".ll -o ";
			cmd += asm_files[i] + ".asm";
			system(cmd.c_str());
			
			cmd = "as -g " + asm_files[i] + ".asm -o ";
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
			ld_line += "/usr/lib/x86_64-linux-gnu/crti.o ";
			ld_line += "/usr/lib/x86_64-linux-gnu/crtn.o ";
			ld_line += "/usr/lib/x86_64-linux-gnu/crt1.o ";
		}
		
		ld_line += "-lc ";

		for (auto obj : obj_files) {
			ld_line += obj + " ";
		}

		if (config.arch == CpuArch::Arm7)
			ld_line += "-dynamic-linker /lib/ld-linux-armhf.so.3 ";
		else if (config.arch == CpuArch::Intel32)
			ld_line += "-dynamic-linker /lib32/ld-linux.so.2 ";
		else
			ld_line += "-dynamic-linker /lib64/ld-linux-x86-64.so.2 ";
		
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


