//
// Created by baomingkun on 18-12-15.
//

#include "ProjN.h"
#include "SCCP.h"
#include "ControlN.h"

namespace C0 {


string ProjGlobalN::asText() {
    return Node::asText() + " " + std::to_string(id);
}

bool ProjGlobalN::same(const Node &other) {
    if(!Node::same(other)) return false;
    const auto &o = (const ProjGlobalN &)(other);
    return o.id == id;
}

string IfProjN::asText() {
    auto region = (RegionN *)(*getUser().begin());
    return fmt::format("v{} IfProj {} to BB{}", serial,
            field ? "true" : "false", region->bid);
}
}
