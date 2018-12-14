//
// Created by baomingkun on 18-12-12.
//

#include "Asm.h"
#include "labelGen.h"
#include <sstream>

using std::stringstream;

namespace C0 {


std::string C0::Asm::genDirectiveByType(C0::Type t) const {
    if (t.isArray()) {
        return ".space " + std::to_string(t.sizeOf());
    } else {
        return ".space 4";
    }
}

std::string C0::Asm::escapeStr(const std::string &str) const {
    string ret;
    for (auto c: str) {
        ret.push_back(c);
        if (c == '\\') {
            ret.push_back('\\');
        }
    }
    return ret;
}

string Asm::toString() {
    stringstream stream;

    stream << ".data" << std::endl;
    stream << "        # ----- string section -----" << std::endl;
    for (const auto &p : string_section) {
        stream << "        " << ".align 2" << std::endl;
        stream << "        ";
        stream << genStringLabel(p.first) << ": .asciiz ";
        if (escape) {
            stream << "\"" << escapeStr(p.second) << "\"";
        } else {
            stream << "\"" << p.second << "\"";
        }
        stream << std::endl;
    }

    stream << "        # ----- global section -----" << std::endl;
    for (const auto &p : global_section) {
        stream << "        " << ".align 2" << std::endl;
        stream << "        ";
        stream << genGlobalLabel(p.first) << ": ";
        stream << genDirectiveByType(p.second);
        stream << std::endl;
    }

    stream << std::endl << ".text" << std::endl;
    stream << "jal main \nli $v0, 10 \nsyscall" << std::endl;

    for (const auto &inst: insts) {
        stream << inst->toString(frame_size) << std::endl;
    }

    return stream.str();
}

}
