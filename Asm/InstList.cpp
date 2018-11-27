//
// Created by baomingkun on 18-11-27.
//

#include "InstList.h"
#include <sstream>

using std::stringstream;

namespace C0 {

string InstList::toString() {
    stringstream stream;

    for(const auto &inst: insts) {
        stream << inst->toString() << std::endl;
    }

    return stream.str();
}

}
