#include "grammar_cli.h"
#include <string>
#include <memory>
#include "AST/ASTDrawer.h"
#include "Parser.h"
#include "AST/ExprAST.h"
#include <cstring>

const std::string helpString =
"grammar draw out programe AST\n"
""
""
;


int grammarMain(int argc, char **argv) {

    if(argc == 1) {
        std::cout << helpString << std::endl;
        return 0;
    }

    if(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        std::cout << helpString << std::endl;
        return 0;
    }

    std::cout << argv[1] << std::endl;
    std::string source = C0::getFileContents(argv[1]);
    std::cout << source << std::endl;

    auto parser = C0::Parser::fromStr(source);
    auto stmt = parser.parseStmt();
    std::cout << C0::ASTDrawer::dot(*stmt) << std::endl;
    return 0;
}




