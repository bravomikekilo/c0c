#pragma once
#include "common.h"
#include "c0.h"


namespace C0 {

optional<Keyword> isKeyword(const string& raw);



string sepToString(Sep sep);
string opToString(Op op);
string cmpToString(Cmp cmp);
string keyToString(Keyword key);

std::string getFileContents(const char *filename);
} // end namespace C0


