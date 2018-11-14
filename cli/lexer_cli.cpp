#include "Lexer.h"
#include "lexer_cli.h"
#include "util.h"
#include <iostream>
#include <cstring>
#include <string>

const std::string helpString =
"C0 lexer [source-name] {options}\n"
"source-name name of source file"
"{options}"
"-h --help show this help message"
""
;


int lexerMain(int argc, char **argv) {
    if (argc == 1) {
        std::cout << helpString << std::endl;
        return 0;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        std::cout << helpString << std::endl;
        return 0;
    }
    
    std::cout << argv[1] << std::endl;
    std::string source = C0::getFileContents(argv[1]);
    std::cout << source << std::endl;
    C0::Lexer lexer(source);

    while (!lexer.peek().is(C0::LexKind::Eof)) {
        std::cout << lexer.peek().toString() << std::endl;
        lexer.next();
    }

    std::cout << lexer.peek().toString() << std::endl;

    const auto& errors = lexer.getErrors();
    if(!errors.empty()) {
        std::cout << "have catch error" << std::endl << std::endl;

        for(const auto& err : lexer.getErrors()) {
            std::cout << err.toString() << std::endl;
        }

    }

    return 0;

}



