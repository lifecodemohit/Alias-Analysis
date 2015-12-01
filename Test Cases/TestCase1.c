#include <stdio.h>

int main(){

int x, y,*r,*s,**p,**q;
y=1;
x=1;
if (y==x){
    r=&x;
    s=&y;
    p=&r;
    }
else{
    q=&s;
    *p=&y;}
return 0;

}
