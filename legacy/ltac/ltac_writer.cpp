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

//Translates a datatype to a string
std::string type2str(DataType t) {
	switch (t) {
		case DataType::Int: return "int";
		case DataType::Float: return "float";
		case DataType::Double: return "double";
		case DataType::Str: return "string";
		case DataType::Void: return "void";
		case DataType::Any: return "any";
	}

	return "";
}

//Writes the code section
void write_code(LtacNode *code) {
	writer << "section code" << std::endl;

	for (auto ln : code->children) {
		switch (ln->type) {
			//Labels
			case ltac::Label: {
				auto lbl = static_cast<LtacLabel *>(ln);
				writer << "lbl " << lbl->name << std::endl;
			} break;

			//Functions
			case ltac::Func: {
				auto func = static_cast<LtacFunc *>(ln);
				writer << "func " << func->name << std::endl;
			} break;

			//External functions
			case ltac::Extern: {
				auto ext = static_cast<LtacExtern *>(ln);
				writer << "extern " << type2str(ext->ret_type) << " ";
				writer << ext->name;

				for (auto p : ext->params) {
					writer << " " << type2str(p);
				}

				writer << std::endl;
			} break;

			//Variables
			case ltac::Var: {
				auto var = static_cast<LtacVar *>(ln);
				auto child = var->children[0];
				std::string name = "var" + std::to_string(var->pos);

				switch (child->type) {
					//Store register operation
					case ltac::Reg: {
						auto reg = static_cast<LtacReg *>(child);

						switch (var->d_type) {
							case DataType::Int: writer << "\tstr "; break;
							case DataType::Float: writer << "\tfstr "; break;
							case DataType::Double: writer << "\tf64.str "; break;

							//TODO: add the reset
						}

						writer << reg->pos << " " << name << std::endl;
					} break;

					//Assign one variable to another
					case ltac::Var: {

					} break;

					//Other types
					//TODO: Add
				}
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
