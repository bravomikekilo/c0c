
#pragma once

#include "Inst.h"
#include <list>
#include "Type.h"

namespace C0 {

using std::list;

class InstList {
    
private:
    
    vector<pair<string, Type>> global_section;
    vector<pair<int, string>> string_section;
    list<unique_ptr<Inst>> insts;

    string genDirectiveByType(Type t) const ;

public:

    string toString(); // generate whole assembly file
    
    void addInst(unique_ptr<Inst>&& inst) {
        insts.push_back(std::move(inst));
    }

    template <typename I, typename ...Args>
    void pushInst(Args&&... args) {
        insts.push_back(std::move(make_unique<I>(std::forward<Args>(args)...)));
    }

    
    void addGlobal(const string &name, Type type);
    void addGlobal(vector<pair<string, Type>>&& vec) {
        global_section = std::move(vec);
    };

    void addString(int str_id, string val);
    void addString(vector<pair<int, string>>&& vec) {
        string_section = std::move(vec);
    };


};



}



