#include "Type.h"

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


}
