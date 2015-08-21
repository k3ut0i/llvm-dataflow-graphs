#include<stdlib.h>

int f1(int, int);
int f2(int, int);

const unsigned int top_non_blocking_input_var_num = 3;


void top_non_blocking_entity(int piped_1, int piped_2,int piped_3,int piped_0)
{
    /**
     * Initializations
     */
    int piped_4 = 0;

    /**
     * Function Declarations
     */
    piped_4 = f1(piped_1, piped_3);
    piped_0 = f2(piped_2, piped_4);
}

#if 0
void top_non_blocking_entity()
{
    minimum(inp, out);
}
#endif
