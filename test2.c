#include<stdio.h>
#include<stdlib.h>

void add(void*** vect, int *size, void *ptr){
    (*vect)=realloc((*vect),sizeof(void*)*++(*size));
    (*vect)[(*size)-1]=ptr;
}

int main(){
    void **vect=NULL;
    int size=0;
    add(&vect,&size,1);
    add(&vect,&size,2);
    add(&vect,&size,3);
    add(&vect,&size,4);
    add(&vect,&size,5);
    printf("size of : %d\n",size);
    for(int i=0;i<5;i++)
        printf("%d ",vect[i]);
    printf("\n\n");
}