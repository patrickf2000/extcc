cmake_minimum_required(VERSION 3.0.0)
project(extcc)

include_directories(parser quik clang lasm compiler)
include_directories(compiler/ltac)

add_compile_options(-std=c++14 -g)

set(PARSER_SRC
	parser/ast.cpp
	parser/lex.cpp
	parser/syntax.cpp
	
	parser/ltac/ltac.cpp
	parser/ltac/ltac_array.cpp
	parser/ltac/ltac_build.cpp
	parser/ltac/ltac_flow.cpp
	parser/ltac/ltac_func.cpp
	parser/ltac/ltac_math.cpp
	parser/ltac/ltac_struct.cpp
	parser/ltac/ltac_vars.cpp
	parser/ltac/ltac_writer.cpp
	
	parser/pasm/pasm_builder.cpp
	parser/pasm/pasm_func.cpp
	parser/pasm/pasm_math.cpp
	parser/pasm/pasm_var.cpp
	parser/pasm/unwriter.cpp
)

set(C_SRC
	clang/c_flow.cpp
	clang/c_func.cpp
	clang/c_lex.cpp	
	clang/c_parser.cpp
	clang/c_struct.cpp
	clang/c_var.cpp
)

set(COMPILER_SRC
	compiler/compiler.cpp
	compiler/main.cpp
	compiler/utils.cpp
)

#TEMPORARY
set(COMPILER1
	compiler/ltac/arm7/arm7_funcs.cpp
	compiler/ltac/arm7/arm7_var.cpp
	compiler/ltac/arm7/asm_arm7.cpp
	
	compiler/ltac/i386/asm_i386.cpp
	
	compiler/ltac/llvm/asm_llvm.cpp
	compiler/ltac/llvm/llvm_func.cpp
	compiler/ltac/llvm/llvm_var.cpp
	
	compiler/ltac/x64/asm_x64.cpp
	compiler/ltac/x64/x64_array.cpp
	compiler/ltac/x64/x64_flow.cpp
	compiler/ltac/x64/x64_funcs.cpp
	compiler/ltac/x64/x64_math.cpp
	compiler/ltac/x64/x64_store.cpp
	compiler/ltac/x64/x64_vars.cpp
	compiler/ltac/x64/x64_vector.cpp
	
	compiler/ltac/base/asm_base.cpp
)

#TEMPORARY
set(COMPILER2
	compiler/x86-64/asm_x64.cpp
	compiler/x86-64/x64_func.cpp
	compiler/x86-64/x64_math.cpp
	compiler/x86-64/x64_syscall.cpp
	compiler/x86-64/x64_var.cpp
)

# Build all the frontend libraries
add_library(parser ${PARSER_SRC})
add_library(cparser ${C_SRC})

# Build the utilities
add_executable(clex clang/test/c_lex_test.cpp)

add_executable(ast parser/test/ast_test.cpp)
add_executable(pasm parser/test/pasm_test.cpp)

# Build the compilers
add_executable(cc ${COMPILER_SRC} ${COMPILER1})
set_target_properties(cc PROPERTIES COMPILE_FLAGS "-DBUILD_CLANG")

add_executable(cc2 ${COMPILER_SRC} ${COMPILER2})
set_target_properties(cc2 PROPERTIES COMPILE_FLAGS "-DBUILD_PASM -DBUILD_CLANG")

# Link everything
target_link_libraries(cparser parser)

target_link_libraries(clex cparser)

target_link_libraries(ast cparser)
target_link_libraries(pasm cparser)

target_link_libraries(cc cparser)
target_link_libraries(cc2 cparser)

