cmake_minimum_required(VERSION 3.0.0)
project(extcc)

include_directories(parser quik clang compiler)

set(PARSER_SRC
	parser/ast.cpp
	parser/lex.cpp
	parser/syntax.cpp
	parser/ltac/ltac.cpp
	parser/ltac/ltac_array.cpp
	parser/ltac/ltac_build.cpp
	parser/ltac/ltac_flow.cpp
	parser/ltac/ltac_func.cpp
	parser/ltac/ltac_vars.cpp
)

set(C_SRC
	clang/c_lex.cpp	
	clang/c_parser.cpp
)

set(QK_SRC
	quik/qk_lex.cpp
)

set(COMPILER_SRC
	compiler/arm7/arm7_funcs.cpp
	compiler/arm7/asm_arm7.cpp
	compiler/base/asm_base.cpp
	compiler/x64/asm_x64.cpp
	compiler/x64/x64_array.cpp
	compiler/x64/x64_flow.cpp
	compiler/x64/x64_funcs.cpp
	compiler/x64/x64_vars.cpp
	compiler/compiler.cpp
	compiler/main.cpp
	compiler/utils.cpp
)

# Build all the frontend libraries
add_library(parser ${PARSER_SRC})
add_library(cparser ${C_SRC})
add_library(qkparser ${QK_SRC})

# Build the utilities
add_executable(clex clang/test/c_lex_test.cpp)
add_executable(cast clang/test/c_ast.cpp)
add_executable(qklex quik/test/qk_lex_test.cpp)

add_executable(ccasm parser/test/ltac_test.cpp)
set_target_properties(ccasm PROPERTIES COMPILE_FLAGS "-DBUILD_CLANG")

# Build the compilers
add_executable(cc ${COMPILER_SRC})
set_target_properties(cc PROPERTIES COMPILE_FLAGS "-DBUILD_CLANG")

# Link everything
target_link_libraries(qkparser parser)
target_link_libraries(cparser parser)

target_link_libraries(clex cparser)
target_link_libraries(qklex qkparser)

target_link_libraries(cast cparser)
target_link_libraries(ccasm cparser)

target_link_libraries(cc cparser)
