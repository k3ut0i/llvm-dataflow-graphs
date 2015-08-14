#include<gtest/gtest.h>
#include<iostream>

#include "utils/llvm_utils.h"

#include <llvm/IR/Constants.h>
#include <llvm/Support/DataTypes.h>

namespace{

    class UtilTest : public testing::Test {
        protected:
            UtilTest()
            {

            }

            virtual ~UtilTest()
            {

            }

            virtual void SetUp()
            {

            }

            virtual void TearDown()
            {

            }
    };

}
TEST_F(UtilTest, LLVMTYPEASSTRING) {
    const std::string output = "i32";
    llvm::LLVMContext Context;
    llvm::Value* int_32_llvm_ir_type = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 1) ;
    //std::cout << llvmutils::LLVMTypeAsString(int_32_llvm_ir_type->getType()) << std::endl;
    EXPECT_EQ(output,llvmutils::LLVMTypeAsString(int_32_llvm_ir_type->getType()));
}

TEST_F(UtilTest, LLVM_INSTRUCTION_AS_STRING){
    const std::string output = "store i32 1, i32* %var_name";
    llvm::LLVMContext Context;
    llvm::Value* var_name;

}


int main(int argc, char **argv) {
    printf("Running main() from gtest_main.cc\n");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
