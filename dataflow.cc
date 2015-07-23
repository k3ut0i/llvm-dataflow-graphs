#include "dataflow.h"

#include <llvm/Support/raw_ostream.h>

#include <list>

char datautils::DataWorker::ID = 0;

static llvm::RegisterPass<datautils::DataWorker> X("dot-dataflow", "Print data flow graph to a dotfile");

datautils::DataWorker::DataWorker() : llvm::ModulePass(ID){}

bool datautils::DataWorker::runOnModule(llvm::Module &M){/*{{{*/

    for(auto globalVariableIdx = M.getGlobalList().begin(),
            globalVariableEnd = M.getGlobalList().end();
            globalVariableIdx != globalVariableEnd;
            ++globalVariableIdx)
    {
        //llvm::errs() << *globalVariableIdx << "\n";
        node_list.push_back(node(globalVariableIdx,globalVariableIdx->getName().str()));

    }
    llvm::errs() << "global variable node list size:\t" << node_list.size() << "\n";
    return false;
}/*}}}*/
