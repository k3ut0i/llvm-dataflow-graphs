#include <stdio.h>
/*{{{*////Function Declarations.
int f0(int x0, int x1, int x2, int x3, int x4, int x5, int x6){return x0*x0 + x1 + x2 + x3 + x4 + x5 +x6 ;}
/*}}}*/
int piped_x0, piped_x1, piped_x2, piped_x3, piped_x4, piped_x5, piped_x6;

int main()
{
    piped_x0 = 0;
    piped_x1 = 1;
    piped_x2 = 2;
    piped_x3 = 3;
    piped_x4 = 4;
    piped_x5 = 5;
    piped_x6 = 6;
    printf("%d\n", f0(piped_x0, piped_x1, piped_x2, piped_x3, piped_x4, piped_x5, piped_x6));
}
