#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Pass.h>
#include <llvm/PassManager.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/SourceMgr.h>

#include "llvm_utils.h"
#include "dataflow.h"


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        llvm::errs() << "Usage:\t" << argv[0] << "<IR FILE>\n";
        return 1;
    }

    llvm::SMDiagnostic Error;
    std::unique_ptr<llvm::Module> Mod(llvm::parseIRFile(argv[1], Error, llvm::getGlobalContext()));
    if(!Mod)
    {
        Error.print(argv[0], llvm::errs());
        return 1;
    }

    llvm::PassManager PM;
    PM.add(new datautils::DataWorker());
    PM.run(*Mod);

return 0;
}
