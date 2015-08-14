#include "flow/dataflow.h"
#include "utils/llvm_utils.h"

#include <llvm/Support/raw_ostream.h>

#include <list>

char datautils::DataWorker::ID = 0;

static llvm::RegisterPass<datautils::DataWorker> X("dot-dataflow", "Print data flow graph to a dotfile");

datautils::DataWorker::DataWorker() : llvm::ModulePass(ID){}

std::string datautils::getvaluestaticname(llvm::Value* val)
{
    std::string ret_val = "val";
    if(val->getName().empty()) {ret_val += std::to_string(num);num++;}
    else ret_val = val->getName().str();

    if(llvm::isa<llvm::Instruction>(val))ret_val += ":"+llvmutils::LLVMInstructionAsString(llvm::dyn_cast<llvm::Instruction>(val));

    return ret_val;
}

bool datautils::DataWorker::runOnModule(llvm::Module &M){/*{{{*/
    ///TODO: create the graph data structure and then dump it.
    //
    ///UPDATE: use exsisting library such as cgraph
    ///This shit is complicated to debug. Those missing nodes labels are a headache.
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
                switch(II->getOpcode()){
                    case llvm::Instruction::Call:
                        {
                            llvm::CallInst * callinst = llvm::dyn_cast<llvm::CallInst>(II);
                            llvm::Function * func = callinst->getCalledFunction();
                            func_calls[II]= func;
                            for(auto arg_idx = func->arg_begin(), arg_end = func->arg_end();arg_idx != arg_end; ++arg_idx)
                            {
                                func_args[func].push_back(node(arg_idx, datautils::getvaluestaticname(arg_idx)));
                                data_flow_edges.push_back(edge(node(II, datautils::getvaluestaticname(II)), node(arg_idx, datautils::getvaluestaticname(arg_idx))));
                                // ///TODO:Use iterations over the arguments of the functions
                                // for(llvm::Value::use_iterator UI = arg_idx->use_begin(), UE = arg_idx->use_end(); UI != UE; ++UI)
                                // {

                                //     data_flow_edges.push_back(edge(node(arg_idx, datautils::getvaluestaticname(arg_idx)), node(UI->get(), datautils::getvaluestaticname(UI->get()))));
                                // }
                            }
                        }
                        break;
                    case llvm::Instruction::Store:
                        {
                            llvm::StoreInst* storeinst = llvm::dyn_cast<llvm::StoreInst>(II);
                            llvm::Value* storevalptr = storeinst->getPointerOperand();
                            llvm::Value* storeval    = storeinst->getValueOperand();
                            data_flow_edges.push_back(edge(node(II, datautils::getvaluestaticname(II)), node(storevalptr, datautils::getvaluestaticname(storevalptr))));
                            data_flow_edges.push_back(edge(node(storeval, datautils::getvaluestaticname(storeval)), node(II, datautils::getvaluestaticname(II))));
                        }
                        break;
                    case llvm::Instruction::Load:
                        {
                            llvm::LoadInst* loadinst = llvm::dyn_cast<llvm::LoadInst>(II);
                            llvm::Value* loadvalptr = loadinst->getPointerOperand();
                            data_flow_edges.push_back(edge(node(loadvalptr, datautils::getvaluestaticname(loadvalptr)), node(II, datautils::getvaluestaticname(loadvalptr))));
                        }break;
                    default :
                        {
                            for(llvm::Instruction::op_iterator op = II->op_begin(), ope = II->op_end(); op != ope; ++op)
                            {
                                if(llvm::dyn_cast<llvm::Instruction>(*op)||llvm::dyn_cast<llvm::Argument>(*op))
                                {
                                    data_flow_edges.push_back(edge(node(op->get(), datautils::getvaluestaticname(op->get())), node(II, datautils::getvaluestaticname(II))));
                                }
                            }
                        }break;
                }


                llvm::BasicBlock::iterator previous = II;
                ++previous;
                func_nodes_ctrl[&*FI].push_back(node(II, datautils::getvaluestaticname(II)));
                if(previous != IE)func_edges_ctrl[&*FI].push_back(edge(node(II, datautils::getvaluestaticname(II)), node(previous, datautils::getvaluestaticname(previous))));

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
    Out << indent << "compound=true;\n";
    Out << indent << "nodesep=1.0;\n";
    Out << indent << "rankdir=TB;\n";
    Out << indent << "subgraph cluster_globals{\n";
    Out << indent << "label=globalvaldefinitions;\n";
    Out << indent << "color=green;\n";
    if(dumpGlobals(Out))return true;
    Out << indent << "}\n";
    for(auto func_node_pair : func_nodes_ctrl)
    {
        indent = "\t";
        if(dumpFunctionArguments(Out, *func_node_pair.first)) return true;
        Out << indent << "subgraph cluster_" << func_node_pair.first->getName().str() << "{\n";
        indent = "\t\t";
        Out << indent << "label=\""<< func_node_pair.first->getName().str() << "\";\n";
        Out << indent << "color=blue;\n";
        if(dumpNodes(Out, *func_node_pair.first)) return true;
        if(dumpControlflowEdges(Out, *func_node_pair.first)) return true;
        indent = "\t";
        Out << indent << "}\n\n";
    }
    dumpDataflowEdges(Out);
    dumpFunctionCalls(Out);
    Out << indent << "label=control_and_data_flow_graph;\n";
    indent = "";
    Out << indent << "}\n";
    return false;
}/*}}}*/

bool datautils::DataWorker::dumpNodes(std::ofstream& Out, llvm::Function &F)/*{{{*/
{
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

bool datautils::DataWorker::dumpFunctionCalls(std::ofstream& Out)/*{{{*/
{
    for(auto call_l : func_calls)
        Out << indent << "\tNode" << call_l.second->front().begin() << " -> Node"<< call_l.first <<"[ltail = cluster_"<< remove_special_chars(call_l.second->getName().str())<<", color=red, label=return];\n";
    return false;
}/*}}}*/

bool datautils::DataWorker::dumpFunctionArguments(std::ofstream& Out, llvm::Function &F)/*{{{*/
{
    ///TODO:Argument in graphviz struct style. i.e., as collection of nodes
    for(auto arg_l : func_args[&F])
        Out << indent << "\tNode" << arg_l.first<<"[label="<<arg_l.second<<", shape=doublecircle, style=filled, color=blue , fillcolor=red];\n";
    return false;
}/*}}}*/

std::string datautils::DataWorker::remove_special_chars(std::string in_str)
{
    std::string ret_val = in_str;
    size_t pos;
    while((pos = ret_val.find('.')) != std::string::npos)
    {
        ret_val[pos] = '_';
    }

    return ret_val;
}
