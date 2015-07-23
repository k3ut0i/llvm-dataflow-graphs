#include <llvm/IR/Instruction.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <list>

namespace datautils{
    typedef std::pair<llvm::Value*, std::string> node;
    struct DataWorker : llvm::ModulePass{
    static char ID;
    DataWorker();
    bool runOnModule(llvm::Module &M);
        private:
    std::string dumpDataflowEdges();
    std::string dumpControlflowEdges();
    std::string dumpCompleteDiGraph();
    std::list<node> node_list;
    std::list<std::pair<node, node>> edge_list;
    };

}
