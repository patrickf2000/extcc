#pragma once

#include <string>
#include <fstream>

#include <pasm/pasm.hh>

using namespace PASM;

/**
 * CompilerBase
 *
 * The base of all compiler backends. In order to generate complete code,
 * the backends must override all the virtual functions here.
 */
class CompilerBase {
public:
	/**
	 * CompilerBase
	 *
	 * @param name The output name of the generated Assembly file.
	 */
	CompilerBase(std::string name) {
		writer = std::ofstream(name);
	}
	
	/**
	 * Generates the data section of an Assembly file.
	 *
	 * @param file The input file (you can use the data vector to generate)
	 */
	virtual void build_data(PasmFile *file) {}
	
	/**
	 * Iterates through the PASM code vector and calls one of the functions below to
	 * generate Assembly for that instruction. This will remain the same in all cases;
	 * you only need to implement the functions below for code to be generated.
	 *
	 * @param file The input file
	 */
	void build_code(PasmFile *file);
	
	/**
	 * Write and close the Assembly file.
	 */
	void write();
	
	/**
	 * Builds a function declaration (note: this is distinct from a label)
	 */
	virtual void build_func(PasmNode *ln) {}
	
	/**
	 * Store an integer function parameter to a variable
	 */
	virtual void build_ildarg(PasmNode *ln) {}
	
	/**
	 * Store a 32-bit float function parameter to a variable
	 */
	virtual void build_f32_ldarg(PasmNode *ln) {}
	
	/**
	 * Store a 64-bit float function parameter to a variable (note that a double
	 * type in C and other languages corresponds to a 64-bit float).
	 */
	virtual void build_f64_ldarg(PasmNode *ln) {}
	
	/**
	 * Store a pointer function parameter to a variable
	 */
	virtual void build_ptr_ldarg(PasmNode *ln) {}
	
	/**
	 * Load an integer value to the return register
	 */
	virtual void build_ildret(PasmNode *ln) {}
	
	/**
	 * Store the value in the return register to an integer variable
	 */
	virtual void build_istret(PasmNode *ln) {}
	
	/**
	 * Store the value in the return register to a pointer variable
	 */
	virtual void build_ptr_stret(PasmNode *ln) {}
	
	/**
	 * Store the value of pointer access to integer variable
	 */
	virtual void build_istrptr(PasmNode *ln) {}
	
	/**
	 * Return from a function (Note: If you are managing a stack, this is where you
	 * clean it up)
	 */
	virtual void build_ret() {}
	
	/**
	 * Set a string value to be used as a function argument
	 */
	virtual void build_str_pusharg(PasmNode *ln) {}
	
	/**
	 * Set a byte to be used as a function argument
	 */
	virtual void build_bpusharg(PasmNode *ln) {}
	
	/**
	 * Set an integer value to be used as a function argument
	 */
	virtual void build_ipusharg(PasmNode *ln) {}
	
	/**
	 * Set a 32-bit floating point value to be used as a function argument
	 */
	virtual void build_f32_pusharg(PasmNode *ln) {}
	
	/**
	 * Set a 64-bit floating point value to be used as a function argument
	 */
	virtual void build_f64_pusharg(PasmNode *ln) {}
	
	/**
	 * Set a pointer to be used as a function argument
	 */
	virtual void build_ptr_pusharg(PasmNode *ln) {}
	
	/**
	 * Call a function
	 */
	virtual void build_call(PasmNode *ln) {}
	
	/**
	 * Store a byte constant to byte variable
	 */
	virtual void build_bstorec(PasmNode *ln) {}
	
	/**
	 * Store an integer constant to an integer variable
	 */
	virtual void build_istorec(PasmNode *ln) {}
	
	/**
	 * Store a 32-bit float constant to a float-32 variable
	 */
	virtual void build_f32_storec(PasmNode *ln) {}
	
	/**
	 * Store a 64-bit float constant to a float-64 variable
	 */
	virtual void build_f64_storec(PasmNode *ln) {}
	
	/**
	 * Store a string constant to a string variable (in C, char *)
	 */
	virtual void build_str_storec(PasmNode *ln) {}
	
	/**
	 * Move the contents of one variable to another
	 */
	virtual void build_move_vv(PasmNode *ln) {}
	
	/**
	 * Load an integer variable to a register
	 */
	virtual void build_ildr(PasmNode *ln) {}
	
	/**
	 * Store the value of a register to a variable
	 */
	virtual void build_str(PasmNode *ln) {}
	
	/**
	 * Load value from location in pointer (ie, an array)
	 */
	virtual void build_ldptr(PasmNode *ln) {}
	
	/**
	 * Store value to a pointer (ie, an array)
	 */
	virtual void build_ptr_str(PasmNode *ln) {}
	
	/**
	 * Perform integer math between a register and an immediate value
	 */
	virtual void build_imath_ri(PasmNode *ln) {}
	
	/**
	 * Perform integer math between two registers. If the register is denoted with -1, the value in the return
	 * register will be used. If the register is -2, the value in the pointer operations (ie, array access) register
	 * will be used.
	 */
	virtual void build_imath_rr(PasmNode *ln) {}
	
	/**
	 * Perform integer math between a register and a variable (memory location)
	 */
	virtual void build_imath_rv(PasmNode *ln) {}
	
	/**
	 * Perform integer math between a variable (memory location) and an immediate value
	 */
	virtual void build_imath_vi(PasmNode *ln) {}
	
	/**
	 * Perform an integer comparison
	 */
	virtual void build_icmp(PasmNode *ln) {}
	
	/**
	 * Perform a branch operation
	 */
	virtual void build_br(PasmNode *ln) {}
	
	/**
	 * Load an integer as a system call parameter
	 */
	virtual void build_isysarg(PasmNode *ln) {}
	
	/**
	 * Load a string as a system call parameter
	 */
	virtual void build_str_sysarg(PasmNode *ln) {}
	
	/**
	 * Call the kernel (system call- some architectures have a single command for this)
	 */
	virtual void build_syscall(PasmNode *ln) {}
protected:
	/**
	 * The output stream to write text to the Assembly file.
	 */
	std::ofstream writer;
	
	/**
	 * Generate a fatal error
	 *
	 * @param msg The message to display
	 */
	void fatalError(std::string msg);
	
	/**
	 * Generate a warning
	 *
	 * @param msg The warning to display
	 */
	void warning(std::string msg);
};
