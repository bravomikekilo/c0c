//
// Created by baomingkun on 18-12-12.
//

#ifndef C0_ASM_H
#define C0_ASM_H

#include "common.h"
#include "Type.h"
#include "InstBase.h"
#include <list>

namespace C0 {

class Asm {

private:
    const bool escape;
    vector<pair<string, Type>> global_section;
    vector<pair<int, string>> string_section;
    std::list<unique_ptr<InstBase>> insts;

    unordered_map<string, int> frame_size;

    string genDirectiveByType(Type t) const ;

    string escapeStr(const string &str) const;

public:

    explicit Asm(bool escape=false)
        :escape(escape) {}

    string toString(); // generate whole assembly file

    void addInst(unique_ptr<InstBase>&& inst) {
        insts.push_back(std::move(inst));
    }

    template <typename I, typename ...Args>
    void pushInst(Args&&... args) {
        insts.push_back(std::move(make_unique<I>(std::forward<Args>(args)...)));
    }


    void addGlobal(const string &name, Type type) {
        global_section.emplace_back(name, type);
    }

    void addGlobal(vector<pair<string, Type>>&& vec) {
        global_section = std::move(vec);
    };

    void addFrameSize(unordered_map<string, int>&& frame_size) {
        frame_size = std::move(frame_size);
    }

    void addString(int str_id, string val) {
        string_section.emplace_back(str_id, val);
    }

    void addString(vector<pair<int, string>>&& vec) {
        string_section = std::move(vec);
    };


};



}


#endif //C0_ASM_H
