//
// Created by baomingkun on 18-11-27.
//

#include "InstList.h"
#include "labelGen.h"
#include <sstream>

using std::stringstream;

namespace C0 {
string InstList::genDirectiveByType(Type t) const
{
    if (t.isArray()) {
        return ".space " + std::to_string(t.sizeOf());
    } else {
        return ".word";
    }
}
string InstList::toString() {
    stringstream stream;

    stream << ".data" << std::endl;
    stream << "        # ----- string section -----" << std::endl;
    for (const auto &p : string_section) {
        stream << "        ";
        stream << Asm::genStringLabel(p.first) << ": .asciiz ";
        stream << "\"" << p.second << "\"";
        stream << std::endl;
    }
    
    stream << "        # ----- global section -----" << std::endl;
    for (const auto &p : global_section) {
        stream << "        ";
        stream << Asm::genGlobalLabel(p.first) << ": ";
        stream << genDirectiveByType(p.second);
        stream << std::endl;
    }

    stream << std::endl << ".text" << std::endl;
    stream << "jal main \n li $v0 10 \n syscall" << std::endl;

    for(const auto &inst: insts) {
        stream << inst->toString() << std::endl;
    }

    return stream.str();
}

void InstList::addGlobal(const string & name, Type type) {
    global_section.emplace_back(name, type);
}

void InstList::addString(int str_id, string val) {
    string_section.emplace_back(str_id, val);
}

}
