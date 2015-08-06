
#include "llvm_utils.h"


//TODO: introduce a template kinda thing to do this auto matically
std::string llvmutils::LLVMTypeAsString(const llvm::Type *T)
{
    std::string typeName;
    llvm::raw_string_ostream N(typeName);
    T->print(N);
    return N.str();
}

std::string llvmutils::LLVMInstructionAsString(llvm::Instruction* I)
{
    std::string instString;
    llvm::raw_string_ostream N(instString);
    I->print(N);
    return N.str();
}
