cmake_minimum_required(VERSION 3.0.0)
project(extcc)

include_directories(parser clang compiler)

add_compile_options(-std=c++14 -g)

set(PARSER_SRC
	parser/ast.cpp
	parser/lex.cpp
	parser/syntax.cpp

	parser/pasm/pasm_builder.cpp
	parser/pasm/pasm_flow.cpp
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
	
	compiler/base/cbase.cpp
	
	compiler/aarch64/asm_aarch64.cpp
	compiler/aarch64/aarch64_flow.cpp
	compiler/aarch64/aarch64_func.cpp
	compiler/aarch64/aarch64_math.cpp
	compiler/aarch64/aarch64_syscall.cpp
	compiler/aarch64/aarch64_var.cpp
	
	compiler/ppc64/asm_ppc64.cpp
	compiler/ppc64/ppc64_flow.cpp
	compiler/ppc64/ppc64_func.cpp
	compiler/ppc64/ppc64_math.cpp
	compiler/ppc64/ppc64_syscall.cpp
	compiler/ppc64/ppc64_var.cpp
	
	compiler/x86-64/asm_x64.cpp
	compiler/x86-64/x64_flow.cpp
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

# Build the compiler
add_executable(cc ${COMPILER_SRC})

# Link everything
target_link_libraries(cparser parser)

target_link_libraries(clex cparser)

target_link_libraries(ast cparser)
target_link_libraries(pasm cparser)

target_link_libraries(cc cparser)

