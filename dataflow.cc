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

    for(auto FI = M.getFunctionList().begin(), FE = M.getFunctionList().end(); FI != FE; ++FI)
    {

        for(auto BB = FI->getBasicBlockList().begin(), BE = FI->getBasicBlockList().end(); BB != BE; ++BB)
        {

            for(auto II = BB->begin(), IE = BB->end(); II != IE; ++II)
            {
                llvm::BasicBlock::iterator previous = II;
                ++previous;
                node_list.push_back(node(II, datautils::getvaluestaticname(II)));
                edge_list.push_back(edge(node(II, datautils::getvaluestaticname(II)), node(previous, datautils::getvaluestaticname(previous))));
            }
        }
    }
    std::ofstream outgraphfile("ctrl-data.dot");
    dumpCompleteDiGraph(outgraphfile);
    return false;
}/*}}}*/

bool datautils::DataWorker::dumpCompleteDiGraph(std::ofstream& Out){
    Out << "digraph \"control_and_data_flow\"{\n";
    if(datautils::DataWorker::dumpNodes(Out)) return true;
    if(datautils::DataWorker::dumpControlflowEdges(Out)) return true;
    Out << "}\n";
return false;}

bool datautils::DataWorker::dumpNodes(std::ofstream& Out){
    for(auto node_l: node_list)
        Out << "\tNode" << node_l.first << "[shape=record, label=\"" << node_l.second << "\"]\n";
    return false;
}

bool datautils::DataWorker::dumpControlflowEdges(std::ofstream& Out)
{
    for(auto edge_l : edge_list)
        Out << "\tNode" << edge_l.first.first << " -> Node" << edge_l.second.first << "\n";
    return false;
}
