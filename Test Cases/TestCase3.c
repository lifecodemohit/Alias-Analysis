#include <stdio.h>

//All possible values for Anderson

int main(){

int p, a, q, r;

p = &a;
p = q;
p = *r;
*p = &a;
*p = q;
*p = *r;

return 0;
}
