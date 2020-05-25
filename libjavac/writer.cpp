#include <cstring>
#include <iostream>
#include <arpa/inet.h>

#include "writer.hh"

//Set up the writer
BinWriter::BinWriter(std::string path) {
	writer = std::ofstream(path.c_str(), std::ios::binary);
}

//Write an opcode
void BinWriter::write_opcode(unsigned char op) {
	writer.write((const char *)&op, sizeof(unsigned char));
}

//Write a short
void BinWriter::write_short(unsigned short op) {
	unsigned char buf[2];
	buf[0] = (op & 0xff00) >> 8;
	buf[1] = (op & 0x00ff);
	write_opcode(buf[0]);
	write_opcode(buf[1]);
	//writer.write((const char *)&op, sizeof(unsigned short));
}

//Write an integer
void BinWriter::write_int(int arg) {
	arg = htonl(arg);
	writer.write((const char *)&arg, sizeof(unsigned int));
}

//Write a double
void BinWriter::write_double(double arg) {
	writer.write((const char *)&arg, sizeof(double));
}

//Write a string
void BinWriter::write_str(const char *str, bool write_size) {
	int size = strlen(str);
	if (write_size)
		write_opcode((char)size);
	writer.write((const char *)str, sizeof(char)*size);
}

//Save the file
void BinWriter::write() {
	writer.close();
}


