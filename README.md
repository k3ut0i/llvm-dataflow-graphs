###LLVM Pass to write data and control flow graph for each statement
[![Build](https://circleci.com/gh/k3ut0i/xmonad-conf.svg?style=shield&circle-token=:circle-token)](https://circleci.com/gh/k3ut0i/llvm-dataflow-graphs)

This is similar to graph-llvm-ir by pfalcon. but there seems to be some problems for it.

Current Status:
1.  Control flow is good per function. should add?? between function blocks and calling
    statements??.
2.  Data flow just getting started.
It's sample test code.
```c
~~~{.c}
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
~~~
```c

Output Dot image
![dotfile](https://raw.githubusercontent.com/k3ut0i/llvm-dataflow-graphs/master/ctrl-data.png)


