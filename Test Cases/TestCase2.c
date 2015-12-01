#include <stdio.h>

int main()
{
int**a, *b, *d, *f,
c, e;
int z=7;
int y=3;
int **x;
while(z!=y){
    x = &b;
    x = &a;
    a = x;
    *a = &c;
    z--;
}
x = &d;
x= &b;
a = x;
*a = &e;
return 0;
}
