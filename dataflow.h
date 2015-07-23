#include <llvm/IR/Instruction.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <list>
#include <iostream>
#include <fstream>

namespace datautils{
    typedef std::pair<llvm::Value*, std::string> node;
    typedef std::pair<node, node> edge;
    struct DataWorker : llvm::ModulePass{
    static char ID;
    DataWorker();
    bool runOnModule(llvm::Module &M);
        private:
    bool dumpDataflowEdges(std::ofstream&);
    bool dumpNodes(std::ofstream&);
    bool dumpControlflowEdges(std::ofstream&);
    bool dumpCompleteDiGraph(std::ofstream&);
    std::list<node> node_list;
    std::list<edge> edge_list;
    };
    static unsigned int num = 0;
    std::string getvaluestaticname(llvm::Value* val)
    {
        std::string ret_val = "val";
        if(val->getName().empty()) {ret_val += std::to_string(num);num++;}
        else ret_val = val->getName().str();

        return ret_val;
    }
}
