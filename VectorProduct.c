#include <stdio.h>
/*{{{*////Function Declarations.
int f0(int x0 , int x6){
    int ans = 0;
    ans += (x0>x6)? x0: x6;
    return ans;
}
/*}}}*/
int piped_x0, piped_x1;

int main()
{
    piped_x0 = 0;
    piped_x1 = 1;
    printf("%d\n", f0(piped_x0, piped_x1));
}
