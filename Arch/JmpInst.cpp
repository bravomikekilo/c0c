#include "JmpInst.h"

namespace C0 {
string BInst::toString(const unordered_map<string, int> &funcs)
{
    return "b " + label;
}

}
