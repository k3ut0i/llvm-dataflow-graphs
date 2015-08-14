#ifndef _DATAFLOW_H
#define _DATAFLOW_H

#include <llvm/IR/Instruction.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <list>
#include <iostream>
#include <fstream>

#include "utils/llvm_utils.h"

namespace datautils{
    typedef std::pair<llvm::Value*, std::string> node;
    typedef std::pair<node, node> edge;
    typedef std::list<node> node_list;
    typedef std::list<edge> edge_list;
    struct DataWorker : llvm::ModulePass{
        static char ID;
        DataWorker();
        bool runOnModule(llvm::Module &M);
        private:
        bool dumpGlobals(std::ofstream &);
        bool dumpDataflowEdges(std::ofstream&, llvm::Function &);
        bool dumpNodes(std::ofstream&, llvm::Function &);
        bool dumpControlflowEdges(std::ofstream&, llvm::Function &);
        bool dumpFunctionArguments(std::ofstream&, llvm::Function&);
        bool dumpDataflowEdges(std::ofstream&);
        bool dumpFunctionCalls(std::ofstream&);
        bool dumpCompleteDiGraph(std::ofstream&);

        std::string remove_special_chars(std::string);
        std::string indent = "";
        std::list<node> globals;
        std::map<llvm::Function*, edge_list> func_edges_ctrl;
        std::map<llvm::Function*, node_list> func_nodes_ctrl;
        std::map<llvm::Value*, llvm::Function*> func_calls;
        std::map<llvm::Function*, node_list> func_args;
        edge_list data_flow_edges;
        std::list<std::string> defined_clusters;
        std::list<std::string> defined_functions;
    };
    static unsigned int num = 0;
    std::string getvaluestaticname(llvm::Value* val);
}

#endif
