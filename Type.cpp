#include "Type.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {


size_t baseTypeSize(BaseTypeK base) {
    switch (base) {
    case BaseTypeK::Char:
        return 1;
    case BaseTypeK::Int:
        return 4;
    case BaseTypeK::Void:
    case BaseTypeK::Error:
        return 0;
    }
}

string baseTypeToString(BaseTypeK t) {
    switch(t) {
        case BaseTypeK::Int:
            return "int";
        case BaseTypeK::Char:
            return "char";
        case BaseTypeK::Void:
            return "void";
        case BaseTypeK::Error:
            return "error";
    }
}


string ArrayT::toString() const {
    return fmt::format("{}[{}]",
            baseTypeToString(base),
            std::to_string(length)
            );

}

}
