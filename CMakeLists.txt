cmake_minimum_required(VERSION 3.0.0)
project(extcc)

include_directories(parser quik clang lasm compiler)

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
)

set(C_SRC
	clang/c_flow.cpp
	clang/c_func.cpp
	clang/c_lex.cpp	
	clang/c_parser.cpp
	clang/c_struct.cpp
	clang/c_var.cpp
)

set(LASM_SRC
	lasm/lasm_array.cpp
	lasm/lasm_flow.cpp
	lasm/lasm_funcs.cpp
	lasm/lasm_lex.cpp
	lasm/lasm_parser.cpp
	lasm/lasm_register.cpp
)

set(QK_SRC
	quik/qk_lex.cpp
)

set(COMPILER_SRC
	compiler/arm7/arm7_funcs.cpp
	compiler/arm7/arm7_var.cpp
	compiler/arm7/asm_arm7.cpp
	
	compiler/i386/asm_i386.cpp
	
	compiler/llvm/asm_llvm.cpp
	compiler/llvm/llvm_func.cpp
	compiler/llvm/llvm_var.cpp
	
	compiler/x64/asm_x64.cpp
	compiler/x64/x64_array.cpp
	compiler/x64/x64_flow.cpp
	compiler/x64/x64_funcs.cpp
	compiler/x64/x64_math.cpp
	compiler/x64/x64_vars.cpp
	compiler/x64/x64_vector.cpp
	
	compiler/base/asm_base.cpp
	compiler/compiler.cpp
	compiler/main.cpp
	compiler/utils.cpp
)

# Build all the frontend libraries
add_library(parser ${PARSER_SRC})
add_library(cparser ${C_SRC})
add_library(lasm_parser ${LASM_SRC})
add_library(qkparser ${QK_SRC})

# Build the utilities
add_executable(clex clang/test/c_lex_test.cpp)
add_executable(qklex quik/test/qk_lex_test.cpp)

add_executable(ast parser/test/ast_test.cpp)
add_executable(asm parser/test/ltac_test.cpp)

# Build the compilers
add_executable(cc ${COMPILER_SRC})
set_target_properties(cc PROPERTIES COMPILE_FLAGS "-DBUILD_CLANG")

add_executable(ltasm ${COMPILER_SRC})
set_target_properties(ltasm PROPERTIES COMPILE_FLAGS "-DBUILD_ASM")

# Link everything
target_link_libraries(qkparser parser)
target_link_libraries(cparser parser)
target_link_libraries(lasm_parser parser)

target_link_libraries(clex cparser)
target_link_libraries(qklex qkparser)

target_link_libraries(ast cparser qkparser)
target_link_libraries(asm cparser qkparser lasm_parser)

target_link_libraries(cc cparser)
target_link_libraries(ltasm lasm_parser)

