#include "dataflow.h"
#include "llvm_utils.h"

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
        globals.push_back(node(globalVariableIdx,globalVariableIdx->getName().str()));

    }

    for(auto FI = M.getFunctionList().begin(), FE = M.getFunctionList().end(); FI != FE; ++FI)
    {
        //TODO:Get a fucntion wise list so the operands defined in the function make sense when
        //use in store instructions. else in the graph they appear outof nowhere.

        for(auto BB = FI->getBasicBlockList().begin(), BE = FI->getBasicBlockList().end(); BB != BE; ++BB)
        {

            for(auto II = BB->begin(), IE = BB->end(); II != IE; ++II)
            {
                llvm::BasicBlock::iterator previous = II;
                ++previous;
                func_nodes[&*FI].push_back(node(II, datautils::getvaluestaticname(II)));
                func_edges[&*FI].push_back(edge(node(II, datautils::getvaluestaticname(II)), node(previous, datautils::getvaluestaticname(previous))));
            }
        }
    }
    std::ofstream outgraphfile("ctrl-data.dot");
    dumpCompleteDiGraph(outgraphfile);
    return false;
}/*}}}*/

bool datautils::DataWorker::dumpCompleteDiGraph(std::ofstream& Out){
    Out << "digraph \"control_and_data_flow\"{\n";
    if(datautils::DataWorker::dumpGlobals(Out))return true;
    for(auto func_node_pair : func_nodes)
    {
        Out << "subgraph \"" << func_node_pair.first->getName().str() << "\"{\n";
        Out << "style=filled;\n";
        Out << "label=\""<< func_node_pair.first->getName().str() << "\";\n";
        if(datautils::DataWorker::dumpNodes(Out, *func_node_pair.first)) return true;
        if(datautils::DataWorker::dumpControlflowEdges(Out, *func_node_pair.first)) return true;
        Out << "}";
    }
    Out << "}\n";
return false;}

bool datautils::DataWorker::dumpNodes(std::ofstream& Out, llvm::Function &F){
    for(auto node_l: func_nodes[&F])
        Out << "\tNode" << node_l.first << "[shape=record, label=\"" << node_l.second << "\"];\n";
    return false;
}

bool datautils::DataWorker::dumpControlflowEdges(std::ofstream& Out, llvm::Function &F)
{
    for(auto edge_l : func_edges[&F])
        Out << "\tNode" << edge_l.first.first << " -> Node" << edge_l.second.first << ";\n";
    return false;
}

bool datautils::DataWorker::dumpGlobals(std::ofstream& Out)
{
    for(auto globalval : globals)
        Out << "\tNode" << globalval.first << "[shape=record, label=\"" << globalval.second << "\"];\n";
    return false;
}
