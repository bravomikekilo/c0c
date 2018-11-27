#include "labelGen.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0::Asm {

string genBlockLabel(const string & func_name, int bid) {
    return fmt::format(".BB_{}_{}", func_name, bid);
}

string genGlobalLabel(const string & name) {
    return fmt::format(".global_{}", name);
}

}
