
#pragma once

#include "common.h"


namespace C0::Asm {

string genBlockLabel(const string &func_name, int bid);

string genGlobalLabel(const string &name);

string genStringLabel(int str_id);


}
