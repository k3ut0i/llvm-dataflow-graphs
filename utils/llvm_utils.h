
#ifndef _LLVM_UTILS_H
#define _LLVM_UTILS_H

#include <llvm/IR/Type.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/raw_ostream.h>


///The implementqation of llvmtypeasstring fucntion was copied from some code which i don't remember
///tell me if anyone cares.

    /// The implementation of LLVMTypeAsString:
    /// https://github.com/eliben/llvm-clang-samples/blob/master/src_llvm/simple_module_pass.cpp
namespace llvmutils {
    std::string LLVMTypeAsString(const llvm::Type *T);
    std::string LLVMInstructionAsString(llvm::Instruction *I);
}

#endif
