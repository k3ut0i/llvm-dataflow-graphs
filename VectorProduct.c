
/*{{{*////Function Declarations.
int f0(int x0, int x1, int x2, int x3 ){return x0*x0 + x1 + x2 + x3;}
int f1(int x0, int x1, int x2, int x3 ){return x0 + x1*x1 + x2 + x3;}
int f2(int x0, int x1, int x2, int x3 ){return x0 + x1 + x2*x2 + x3;}
int f3(int x0, int x1, int x2, int x3 ){return x0 + x1 + x2 + x3*x3;}
/*}}}*/
int piped_x0, piped_x1, piped_x2, piped_x3, piped_x4, piped_x5, piped_x6;

int top_non_blocking_entity()/*{{{*/
{
    piped_x0 = f0(piped_x0,piped_x1,piped_x2,piped_x3);
    piped_x1 = f1(piped_x1,piped_x2,piped_x3,piped_x4);
    piped_x2 = f2(piped_x2,piped_x3,piped_x4,piped_x5);
    piped_x3 = f3(piped_x3,piped_x4,piped_x5,piped_x6);
    return 0;
}/*}}}*/

    //series of functions starting with top_blocking_entity
int top_blocking_entity0()
{
    // Any set of calls blocking one another
    return 0;}

