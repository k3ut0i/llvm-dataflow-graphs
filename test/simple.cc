#include<gtest/gtest.h>

namespace{

    class SimpleTest : public testing::Test {
        protected:
            SimpleTest()
            {

            }

            virtual ~SimpleTest()
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
GTEST_API_ int main(int argc, char **argv) {
    printf("Running main() from gtest_main.cc\n");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

