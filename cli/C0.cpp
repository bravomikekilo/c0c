// C0.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstring>
#include <string>
#include "lexer_cli.h"
#include "grammar_cli.h"


const std::string helpString =
"C0 a mini C0 compiler for mips\n"
"C0 <subcommand>\n"
"<subcommand>\n"
"lexer\n"
"grammar\n"
"compiler\n"
;

int main(int argc, char **argv)
{
    if (argc == 1) {
        std::cout << helpString << std::endl;
        return 0;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        std::cout << helpString << std::endl;
        return 0;
    }

    // jump first argument, dispatch argument to subcommand
    int newArgc = argc - 1;
    auto newArgv = argv + 1;

    // dispatch to lexer
    if (strcmp(argv[1], "lexer") == 0) {
        return lexerMain(newArgc, newArgv);
    }

    // dispatch to grammar
    if (strcmp(argv[1], "grammar") == 0) {
        return grammarMain(newArgc, newArgv);
    }




    std::cout << "unknown subcommand:" << argv[1] << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

