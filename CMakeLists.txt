CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
project(C0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

file(GLOB CLI_FILE "cli/*_cli.*")
file(GLOB AST_FILE "AST/*")
file(GLOB QUAD_FILE "Quad/*")

include_directories(${C0_SOURCE_DIR})


add_executable(Lexer Lexer.cpp Lex.cpp util.cpp cli/lexer.cpp AST/ExprAST.cpp)

add_executable(Quad cli/quad.cpp Lexer.cpp Lex.cpp util.cpp Type.cpp Parser.cpp SymTable.cpp ${AST_FILE} ${QUAD_FILE} AST/ExprAST.cpp TypeChecker.cpp TypeChecker.h)

add_executable(Grammar Lexer.cpp Lex.cpp util.cpp Type.cpp Parser.cpp ${AST_FILE} SymTable.cpp cli/grammar.cpp AST/ExprAST.cpp TypeChecker.cpp TypeChecker.h)

add_executable(C0 cli/C0.cpp Lexer.cpp Lex.cpp Parser.cpp util.cpp Type.cpp ${CLI_FILE} ${AST_FILE} IR/IRNode.cpp IR/IRNode.h SymTable.h SymTable.cpp AST/FuncAST.h Quad/BasicBlock.cpp Quad/BasicBlock.h Quad/Quads.cpp Quad/Quads.h Quad/CFGDrawer.cpp Quad/CFGDrawer.h Quad/CFGConverter.cpp Quad/CFGConverter.h AST/ExprAST.cpp TypeChecker.cpp TypeChecker.h)
