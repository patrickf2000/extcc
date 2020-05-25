#include <iostream>

#include "javafile.hh"

//Create the java file and write some initial information
JavaFile::JavaFile(std::string cname) {
	class_name = cname;
	writer = new BinWriter(class_name + ".class");
	
	//Write the magic number
	writer->write_opcode(0xCA);
	writer->write_opcode(0xFE);
	writer->write_opcode(0xBA);
	writer->write_opcode(0xBE);
	
	//Write the version number
	writer->write_short(0x00);
	writer->write_opcode(0x00);
	writer->write_opcode(0x34);
}

//Write all functions to our file
void JavaFile::writeFuncs(JavaPool *pool) {
	for (auto func : funcs) {
		writer->write_opcode(0x00);
		writer->write_opcode(func->getAttributes());
		
		//Find the indexes
		int name_index = labels[func->name];
		int type_index = labels[func->type];
		
		if (func->name == "<init>") {
			for (auto f : pool->funcRefs) {
				if (f.name == func->name) {
					name_index = f.pos + 2;
					break;
				}
			}
		}
		
		//Write the name
		writer->write_opcode(0x00);
		writer->write_opcode((unsigned char)name_index);
		
		//Write the type
		writer->write_opcode(0x00);
		writer->write_opcode((unsigned char)type_index);
		
		//Attribute
		writer->write_opcode(0x00);
		writer->write_opcode(0x01);
		
		//Get the location of the code section in the stack
		int code_index = labels["Code"];
		writer->write_opcode(0x00);
		writer->write_opcode((unsigned char)code_index);
		
		//Code attribute size
		if (func->name == "main")
			writer->write_int(func->code.size()+12);
		else
			writer->write_int(func->code.size()+24);
		
		//Stack size
		writer->write_opcode(0x00);
		writer->write_opcode(0x07);
		
		//Local variables
		//writer->write_opcode(0x00);
		//writer->write_opcode(0x02);
		writer->write_short((short)func->locals);
		
		//Write the code size (is an int)
		writer->write_int(func->code.size());
		
		//Write the code
		for (auto op : func->code)
			writer->write_opcode(op);
		
		//Padding
		//This goes at end
		writer->write_opcode(0x00);
		writer->write_opcode(0x00);
		
		//added
		if (func->name != "main") {
			writer->write_opcode(0x00);
			writer->write_opcode(0x01);
			
			writer->write_opcode(0x00);
			int lnt_index = labels["LineNumberTable"];
			writer->write_opcode((unsigned char)lnt_index);
			writer->write_int(6);
			
			writer->write_opcode(0x00);
			writer->write_opcode(0x01);
			
			writer->write_int(0x01);
			//writer->write_opcode(0x00);
			//writer->write_opcode(0x01);
		}
	}
}

//Close and do the final writing
void JavaFile::write(JavaPool *pool) {
	labels = pool->pool;

	//Write the constant pool size
	writer->write_opcode(0x00);
	writer->write_opcode((unsigned char)pool->getPoolSize());
	
	//Write the constant pool
	for (auto c : pool->getCode()) {
		writer->write_opcode(c);
	}
	//writeConsts();
		
	//Access modifier
	writer->write_opcode(0x00);
	writer->write_opcode(0x21);
	
	//Index of class in constant pool
	writer->write_opcode(0x00);
	writer->write_opcode(0x01);
	
	//Index of super class in constant pool
	writer->write_opcode(0x00);
	writer->write_opcode(0x03);
	
	//Zero interfaces
	writer->write_short(0);
	
	//Zero fields
	writer->write_short(0);
	
	//Number of methods
	int count = funcs.size();
	
	writer->write_opcode(0x00);
	writer->write_opcode((unsigned char)count);
	
	//Write the functions
	writeFuncs(pool);
		
	//EOF padding
	writer->write_int(0);
		
	writer->write();
}


