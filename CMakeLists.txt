cmake_minimum_required(VERSION 3.0.0)
project(extcc)

include_directories(parser quik)

set(PARSER_SRC
	parser/lex.cpp
)

set(QK_SRC
	quik/qk_lex.cpp
)

add_library(parser ${PARSER_SRC})
add_library(qkparser ${QK_SRC})

add_executable(qklex quik/test/qk_lex_test.cpp)

target_link_libraries(qkparser parser)
target_link_libraries(qklex qkparser)
