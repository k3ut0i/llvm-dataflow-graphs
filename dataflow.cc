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
                func_nodes_ctrl[&*FI].push_back(node(II, datautils::getvaluestaticname(II)));
                if(previous != IE)func_edges_ctrl[&*FI].push_back(edge(node(II, datautils::getvaluestaticname(II)), node(previous, datautils::getvaluestaticname(previous))));
                for(llvm::Instruction::op_iterator op = II->op_begin(), ope = II->op_end(); op != ope; ++op)
                {
                    if(llvm::dyn_cast<llvm::Instruction>(*op)||llvm::dyn_cast<llvm::Argument>(*op))
                    {
                        data_flow_edges.push_back(edge(node(op->get(), datautils::getvaluestaticname(op->get())), node(II, datautils::getvaluestaticname(II))));
                    }
                }
            }

            llvm::TerminatorInst* TI = BB->getTerminator();
            for(unsigned int succ_idx  = 0, succ_num = TI->getNumSuccessors(); succ_idx != succ_num; ++succ_idx)
            {
                llvm::BasicBlock * Succ = TI->getSuccessor(succ_idx);
                llvm::Value* succ_inst = Succ->begin();
                func_edges_ctrl[&*FI].push_back(edge(node(TI, datautils::getvaluestaticname(TI)), node(succ_inst, datautils::getvaluestaticname(succ_inst))));
            }
        }
    }
    std::ofstream outgraphfile("ctrl-data.dot");
    dumpCompleteDiGraph(outgraphfile);
    return false;
}/*}}}*/

bool datautils::DataWorker::dumpCompleteDiGraph(std::ofstream& Out){/*{{{*/
    Out << indent << "digraph \"control_and_data_flow\"{\n";
    indent = "\t";
    Out << indent << "subgraph cluster_globals{\n";
    Out << indent << "label=globalvaldefinitions;\n";
    Out << indent << "color=green;\n";
    if(datautils::DataWorker::dumpGlobals(Out))return true;
    Out << indent << "}\n";
    for(auto func_node_pair : func_nodes_ctrl)
    {
        indent = "\t";
        Out << indent << "subgraph cluster_" << func_node_pair.first->getName().str() << "{\n";
        indent = "\t\t";
        Out << indent << "label=\""<< func_node_pair.first->getName().str() << "\";\n";
        Out << indent << "color=blue;\n";
        if(datautils::DataWorker::dumpNodes(Out, *func_node_pair.first)) return true;
        if(datautils::DataWorker::dumpControlflowEdges(Out, *func_node_pair.first)) return true;
        indent = "\t";
        Out << indent << "}\n\n";
    }
    datautils::DataWorker::dumpDataflowEdges(Out);
    Out << indent << "label=control_and_data_flow_graph;\n";
    indent = "";
    Out << indent << "}\n";
    return false;
}/*}}}*/

bool datautils::DataWorker::dumpNodes(std::ofstream& Out, llvm::Function &F){/*{{{*/
    for(auto node_l: func_nodes_ctrl[&F])
        Out << indent << "\tNode" << node_l.first << "[shape=record, label=\"" << node_l.second << "\"];\n";
    return false;
}/*}}}*/

bool datautils::DataWorker::dumpControlflowEdges(std::ofstream& Out, llvm::Function &F)/*{{{*/
{
    for(auto edge_l : func_edges_ctrl[&F])
        Out << indent << "\tNode" << edge_l.first.first << " -> Node" << edge_l.second.first << ";\n";
    return false;
}/*}}}*/

bool datautils::DataWorker::dumpGlobals(std::ofstream& Out)/*{{{*/
{
    for(auto globalval : globals)
        Out << indent << "\tNode" << globalval.first << "[shape=record, label=\"" << globalval.second << "\"];\n";
    return false;
}/*}}}*/

bool datautils::DataWorker::dumpDataflowEdges(std::ofstream& Out)/*{{{*/
{
    for(auto edge_l : data_flow_edges)
        Out << indent << "\tNode" << edge_l.first.first << " -> Node" << edge_l.second.first << "[color=red];\n";
    return false;
}/*}}}*/
