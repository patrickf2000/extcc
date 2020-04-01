cmake_minimum_required(VERSION 3.0.0)
project(extcc)

include_directories(parser quik clang)

set(PARSER_SRC
	parser/ast.cpp
	parser/lex.cpp
	parser/parser.cpp
)

set(C_SRC
	clang/c_lex.cpp	
	clang/c_parser.cpp
)

set(QK_SRC
	quik/qk_lex.cpp
)

add_library(parser ${PARSER_SRC})
add_library(cparser ${C_SRC})
add_library(qkparser ${QK_SRC})

add_executable(clex clang/test/c_lex_test.cpp)
add_executable(cast clang/test/c_ast.cpp)
add_executable(qklex quik/test/qk_lex_test.cpp)

target_link_libraries(qkparser parser)
target_link_libraries(cparser parser)

target_link_libraries(clex cparser)
target_link_libraries(qklex qkparser)

target_link_libraries(cast cparser)
