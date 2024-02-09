#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void **vect=NULL;
int size=0;
void *start=NULL, *end=NULL;
void add(void *ptr){
    // printf("{%p}\n",ptr);
    if(ptr<start)
        start=ptr;
    if(ptr>end)
        end=ptr;
    vect=realloc(vect,sizeof(void*)*++size);
    vect[size-1]=ptr;
    end=ptr;
}
int search(void *ptr){
    printf("-%p\n",ptr);
    if(ptr>=start && ptr<=end)
        return 1;
    return 0;
}
int main(){
    add("hello");
    add("hi");
    add("world");
    add("hooo");
    add(5);
    printf("start : %p, end=%p\n",start,end);
    if(search(5))
        printf("Yes it is present \n");
    if(search("hello"))
        printf("Element is present\n");
    // for(int i=0;i<size;i++){
    //     printf("%p\n",*(vect+i));
    // }
    // printf("\n");
    // printf("%p\n%p\n",(vect),vect+1);
    // printf("%s\n\n",vect[0]);
    printf("\n\n");
}