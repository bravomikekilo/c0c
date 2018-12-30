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
        case BaseTypeK::Num:
            return 4;
    }
}

string baseTypeToString(BaseTypeK t) {
    switch (t) {
        case BaseTypeK::Int:
            return "int";
        case BaseTypeK::Char:
            return "char";
        case BaseTypeK::Void:
            return "void";
        case BaseTypeK::Error:
            return "error";
        case BaseTypeK::Num:
            return "num";
    }
}


string Type::toString() const {
    if (length != 0) {
        return fmt::format("{}[{}]", baseTypeToString(base_type), length);
    } else {
        return baseTypeToString(base_type);
    }
}

}
