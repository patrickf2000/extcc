#pragma once

#include <string>
#include <fstream>

class X64 {
public:
	explicit X64(std::string name);
	void write();
private:
	std::ofstream writer;
};
