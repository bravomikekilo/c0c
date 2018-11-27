
#pragma once

#include "Inst.h"
#include <list>

namespace C0 {

using std::list;

class InstList {
    
private:
    
    vector<pair<string, int>> const_section;
    vector<pair<string, string>> string_section;
    list<unique_ptr<Inst>> insts;

public:

    string toString(); // generate whole assembly file
    
    void addInst(unique_ptr<Inst>&& inst) {
        insts.push_back(std::move(inst));
    }

    template <typename I, typename ...Args>
    void pushInst(Args&&... args) {
        insts.push_back(std::move(make_unique<I>(std::forward<Args>(args)...)));
    }

    
    void addGlobal(const string &name, int val) {
        
    }

    void addString(int str_id, string val) {
        
    }

};



}



