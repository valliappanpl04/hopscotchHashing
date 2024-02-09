#include<stdio.h>

typedef struct Integer{
    void (*fp)(int*);

}in;

void printint(int *a){
    printf("%d",*a);
}
void printchar(char *a){
    printf("%c",*a);
}
in* initializeint(){
    in* obj=(in*)malloc(sizeof(in));
    obj->fp=&printint;
}
int main(){
    // in* intobj=initializeint();
    // int a=5;
    // intobj->fp(&a);
    // printf("\n\n\n");
    void (*fp)(char*)=printint;
    fp = &printchar;
    // fp(1);
    fp('a');
    // int *a;
    // int (*a);
}