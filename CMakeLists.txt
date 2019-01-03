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
file(GLOB ASM_FILE "Asm/*")
file(GLOB NAIVE_GEN_FILE "NaiveGen/*")
file(GLOB SON_FILE "SON/*")
file(GLOB ARCH_FILE "Arch/*")

include_directories(${C0_SOURCE_DIR})


add_executable(Lexer Lexer.cpp Lex.cpp util.cpp cli/lexer.cpp AST/ExprAST.cpp SourceStream.cpp SourceStream.h)

add_executable(Quad cli/quad.cpp Lexer.cpp Lex.cpp util.cpp Type.cpp Parser.cpp SymTable.cpp ${AST_FILE} ${QUAD_FILE} AST/ExprAST.cpp TypeChecker.cpp TypeChecker.h Pos.h SourceStream.cpp SourceStream.h Quad/CFGWriter.cpp Quad/CFGWriter.h)

add_executable(Grammar Lexer.cpp Lex.cpp util.cpp Type.cpp Parser.cpp ${AST_FILE} SymTable.cpp cli/grammar.cpp AST/ExprAST.cpp TypeChecker.cpp TypeChecker.h Pos.h SourceStream.cpp SourceStream.h)

add_executable(QuadGen cli/quadGen.cpp Lexer.cpp Lex.cpp util.cpp Type.cpp Parser.cpp SymTable.cpp ${AST_FILE} ${QUAD_FILE} ${ASM_FILE} ${NAIVE_GEN_FILE} TypeChecker.cpp Pos.h SourceStream.cpp SourceStream.h NaiveGen/ConstFolder.cpp NaiveGen/ConstFolder.h)

add_executable(SON cli/seaOfNode.cpp Lexer.cpp Lex.cpp util.cpp Type.cpp Parser.cpp SymTable.cpp TypeChecker.cpp ${SON_FILE} ${AST_FILE} ${ARCH_FILE} Pos.h SourceStream.cpp SourceStream.h SON/FineDrawer.cpp SON/FineDrawer.h SON/LSect.cpp SON/LSect.h SON/liveness.h SON/IRGraph.cpp SON/IRGraph.h SON/CodeGen.cpp SON/CodeGen.h)
add_executable(IR cli/irdump.cpp Lexer.cpp Lex.cpp util.cpp Type.cpp Parser.cpp SymTable.cpp TypeChecker.cpp ${SON_FILE} ${AST_FILE} ${ARCH_FILE} Pos.h SourceStream.cpp SourceStream.h SON/FineDrawer.cpp SON/FineDrawer.h SON/LSect.cpp SON/LSect.h SON/liveness.h SON/IRGraph.cpp SON/IRGraph.h SON/CodeGen.cpp SON/CodeGen.h)

