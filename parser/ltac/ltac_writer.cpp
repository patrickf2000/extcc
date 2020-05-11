#include <fstream>
#include <iostream>
#include <string>

#include <ltac/ltac.hh>

//The writer
std::ofstream writer;

//Writes the data section
void write_data(LtacNode *data) {
	writer << "section data" << std::endl;

	for (auto d : data->children) {
		switch (d->type) {
			case ltac::String: {
				auto str = static_cast<LtacString *>(d);
				writer << "\t string " << str->name << " \"";
				writer << str->val << "\"" << std::endl;
			} break;

			case ltac::Float: {

			} break;
		}
	}

	writer << std::endl;
}

//Writes the code section
void write_code(LtacNode *code) {
	writer << "section code" << std::endl;

	for (auto ln : code->children) {
		switch (ln->type) {
			case ltac::Func: {
				auto func = static_cast<LtacFunc *>(ln);
				writer << "func " << func->name << std::endl;
			} break;
		}
	}

	writer << std::endl;
}

//The main ltac output writer
void output_ltac(LtacFile *file) {
	writer = std::ofstream(file->name + ".as");

	//Write the data section
	write_data(file->data);

	//Write the code section
	write_code(file->code);

	writer.close();
}
