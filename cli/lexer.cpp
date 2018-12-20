#include "Lexer.h"
#include "util.h"
#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>

using namespace std;

int main() {
    
    string source_name;
    cin >> source_name;

    string source = C0::getFileContents(source_name.c_str());
    C0::Lexer lexer(source);

    while (!lexer.peek().is(C0::LexKind::Eof)) {
        std::cout << lexer.peek().toString() << " @";
        std::cout << lexer.headPos().toStr() << std::endl;
        lexer.next();
    }

    std::cout << lexer.peek().toString() << std::endl;

    const auto& errors = lexer.getErrors();
    if (!errors.empty()) {
        std::cout << "have catch error" << std::endl << std::endl;

        for (const auto& err : lexer.getErrors()) {
            std::cout << err.toString() << std::endl;
        }

    }

    cin >> source_name;
    return 0;
}