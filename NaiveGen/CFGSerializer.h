//
// Created by baomingkun on 18-11-26.
//

#ifndef C0_CFGSERIALIZER_H
#define C0_CFGSERIALIZER_H

#include "Asm/InstList.h"
#include "Asm/labelGen.h"

#include "BfsCFGWalker.h"
#include "RegAlloc.h"
#include "frameTable.h"
#include "AST/FuncAST.h"

namespace C0 {

class CFGSerializer : public BfsCFGWalker {
private:
    InstList &list;
    shared_ptr<SymTable> sym_table;
    shared_ptr<FuncAST> curr_func;
    shared_ptr<frameTable> frame_table;
    const unordered_map<string, int>& func_offset;

    string getBlockLabel(BasicBlock *block) {
        return getBlockLabel(block->getBid());
    }

    string getBlockLabel(int bid) {
        return Asm::genBlockLabel(curr_func->name, bid);
        // return ".BB_" + curr_func->name + "_" + std::to_string(bid);
    }

    string getGlobalLabel(const string &name) {
        return Asm::genGlobalLabel(name);
        // return "global_" + name;
    }

    void handleArith(Quad &q, const RegTable *table);
    void handleJmp(Quad &q, const RegTable *table);
    void handleCopy(Quad &q, const RegTable *table);
    void handleGet(Quad &q, const RegTable *table);
    void handleSet(Quad &q, const RegTable *table);
    void handleB(Quad &q, const RegTable *table);
    void handlePrint(Quad &q, const RegTable *table);
    void handleRead(Quad &q, const RegTable *table);
    void handleRet(Quad &q, const RegTable *table);
    void handleCall(Quad &q, const RegTable *table);

    unique_ptr<Reg> getVal(QuadVal &val, size_t n, const RegTable *table) ;
    void loadGlobalScalar(const string &name, unique_ptr<Reg>&& reg);
    void storeGlobalScalar(const string &name, unique_ptr<Reg>&& reg);
    void loadGlobalArrayAddr(const string &name, unique_ptr<Reg>&& reg);
    void putValTo(const QuadVal &val, unique_ptr<Reg> reg, const RegTable *table);
    void saveReg();
    void recoverReg();
    void issueRet();

    bool evalCond(QuadOp op, int lhs, int rhs);


protected:
    void visit(BasicBlock *block) override ;
public:

    explicit CFGSerializer(InstList &list, const unordered_map<string, int>& offsets)
    : list(list), func_offset(offsets) { }

    void serialize(shared_ptr<FuncAST> func, BasicBlock *start, shared_ptr<frameTable> frame);


};

}


#endif //C0_CFGSERIALIZER_H
