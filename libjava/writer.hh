#pragma once

#include <fstream>

class BinWriter {
public:
	explicit BinWriter(std::string path);
	void write_opcode(unsigned char op);
	void write_short(unsigned short op);
	void write_int(int arg);
	void write_double(double arg);
	void write_str(const char *str, bool write_size=true);
	void write();
private:
	std::ofstream writer;
};
