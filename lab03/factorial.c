#include <stdio.h>
int factorial(int n);

int main(){
    int res=factorial(3);
    printf("%d\n",res);
    return 0;
}

int factorial(int n){
    int result=1;
    for(int i=n;i>0;i--){
        result*=i;
    }
    return result;
}


