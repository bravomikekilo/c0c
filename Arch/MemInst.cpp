#include "MemInst.h"

namespace C0 {

string C0::MemInst::toString(const unordered_map<string, int> &funcs) {
    string ret = fmt::format("{} {}, ", op(), src.toStr());
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

    if (reg.has_value()) {
        ret += fmt::format("({})", reg.value().toStr());
    }

    return ret;
}

} // end namespace C0

