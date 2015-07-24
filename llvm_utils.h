#include <llvm/IR/Type.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Support/raw_ostream.h>


///The implementqation of llvmtypeasstring fucntion was copied from some code which i don't remember
///tell me if anyone cares.
namespace llvmutils {
    std::string LLVMTypeAsString(const llvm::Type *T);
    std::string LLVMInstructionAsString(llvm::Instruction *I);
}
