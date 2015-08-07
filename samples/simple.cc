#include <stdio.h>

int f0(int x0 , int x1){
    int ans = 0;
    ans += (x0>x1)? x0: x1;
    return ans;
}

int piped_x0, piped_x1;

int main()
{
    piped_x0 = 0;
    piped_x1 = 1;
    printf("%d\n", f0(piped_x0, piped_x1));
}
