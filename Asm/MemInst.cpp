#include "MemInst.h"

namespace C0 {

string C0::MemInst::toString() {
    string ret = fmt::format("{} {}, ", op(), src->toString());
    if (label.has_value()) {
        ret += label.value();
        if (offset != 0) {
            ret.push_back('+');
            ret += std::to_string(offset);
        } 
    } else {
        if (offset != 0) {
            ret += std::to_string(offset);
        } 
    }

    if (reg != nullptr) {
        ret += fmt::format("({})", reg->toString());
    }

    return ret;
}

} // end namespace C0

