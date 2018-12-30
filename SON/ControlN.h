//
// Created by baomingkun on 18-12-4.
//

#ifndef C0_CONTROLN_H
#define C0_CONTROLN_H

#include "Node.h"
#include "common.h"

#include <list>
#include <functional>

namespace C0 {

class RegionN : public Node {
private:

    std::list<UseE> order;

public:

    int bid = -1;

    explicit RegionN(const vector<UseE> &use) :Node(Nop::Region, use.size()) {
       for(int i = 0; i < num_uses; ++i) {
           uses[i] = use[i];
       }
    }

    RegionN() : Node(Nop::Region, 0) {}

    explicit RegionN(UseE use) :Node(Nop::Region, 1) {
        uses[0] = use;
    }

    explicit RegionN(UseE one, UseE two) :Node(Nop::Region, 2) {
        uses[0] = one;
        uses[1] = two;
    }

    std::list<UseE> &getOrder() {
        return order;
    }

    void schedule();

    void SCCPType() override;

    UseE SCCPIdentity(Sea &sea) override;

    void visitPred(std::function<void(RegionN *)>);

    template<typename T>
    void visitPred(void *func(RegionN *, T *env), T *env) {
        for(auto use : *this) {
            auto use_op = use->getOp();
            if(use_op == Nop::Region) {
                func(use, env);
            } else if(use_op == Nop::IfProj) {
                func(use->front()->front(), env);
            }
        }
    }

    void visitPost(std::function<void(RegionN *)>);

    template<typename T>
    void visitPost(void (*func)(RegionN *, T *env), T *env) {
        for(auto user: getUser()) {
            auto user_op = user->getOp();
            if(user_op == Nop::Region) {
                func(reinterpret_cast<RegionN *>(user), env);
            } else if(user_op == Nop::If) {
                for(auto proj : user->getUser()) {
                    auto region = *proj->getUser().begin();
                    func(reinterpret_cast<RegionN *>(region), env);
                }
            }
        }
    }

    string str() override;


};

class IfN : public Node {
public:
    IfN(UseE up, UseE predict) :Node(Nop::If, 2) {
        uses[0] = up;
        uses[1] = predict;
    }

    void SCCPType() override;

    void SCCPType(ProjN *n) override;

    UseE SCCPIdentity(Sea &sea) override;

    UseE SCCPIdentity(Sea &sea, ProjN *projection) override;
};


class StopN : public Node {
private:
    bool has_ret;
public:
    explicit StopN(UseE last_block, const vector<UseE> &globals, UseE world)
        :Node(Nop::Stop,  globals.size() + 2), has_ret(false) {
        int j = 2;
        uses[0] = last_block;
        uses[1] = world;

        for(int i = 0; i < globals.size(); ++i, ++j) {
            uses[j] = globals[i];
        }

    }

    explicit StopN(UseE last_block, const vector<UseE> &globals, UseE world, UseE ret)
        :Node(Nop::Stop, globals.size() + 3), has_ret(false) {
        int j = 3;
        uses[0] = last_block;
        uses[1] = ret;
        uses[2] = world;

        for(int i = 0; i < globals.size(); ++i, ++j) {
            uses[j] = globals[i];
        }

    }

    string str() override {
        return fmt::format("{}:{}", Node::str(), has_ret ? "ret" : "void") ;
    }

    void SCCPType() override;

};





}

#endif //C0_CONTROLN_H
