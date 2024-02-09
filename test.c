#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void **vect=NULL;
int size=0;
void add(void *ptr){
    vect=realloc(vect,sizeof(void*)*++size);
    vect[size-1]=ptr;
}
int compareint(void* a, void *b){
    int num1=a, num2=b;
    if(a==b)
        return 0;
    else if(a>b)
        return 1;
    else
        return -1;
}
int comparechar(char ch1, char ch2){
    if((char*)ch1==(char*)ch2)
        return 0;
    else if((char*)ch1>(char*)ch2)
        return 1;
    else
        return -1;
}
int comparestr(char *str1, char *str2){
    if(strcmp(str1,str2)==0)
        return 0;
    else if(strcmp(str1,str2)>0)
        return 1;
    else
        return -1;
}
void sort(int (*compare)(void*,void*)){
    for(int i=0;i<size;i++){
        for(int j=i+1;j<size;j++){
            if(compare(vect[i],vect[j])>0){
                void *ptr=vect[i];
                vect[i]=vect[j];
                vect[j]=ptr;
            }
        }
    }
}
int binarysearch(void *ptr, int (*compare)(void*, void*)){
    int start=0, end=size-1;
    int mid=(start+end)/2;
    while(start<=end){
        mid=(start+end)/2;
        if(compare(vect[mid],ptr)==0)
            return mid;
        if(compare(vect[mid],ptr)>0){
            end=mid-1;
        }
        else{
            start=mid+1;
        }
    }
    return -1;
}
int main(){
    // void **vect=NULL;
    // int size=0;
    // add(&v,&size,'d');
    // add(&v,&size,'x');
    // add(&v,&size,'c');
    // add(&v,&size,'a');
    add(1);
    add(2);
    add(5);
    add(4);
    // add(&v,&size,"Hello");
    // add(&v,&size,"Apple");
    // add(&v,&size,"Hi");
    // add(&v,&size,"Hello1");
    // add(&v,&size,"Apple2");
    // add(&v,&size,"Hi1");
    int (*compare)(void*, void*)=&compareint;
    sort(compare);
    printf("Sorted Array: ");
    for(int i=0;i<size;i++){
        printf("%d ",vect[i]);
    }
    printf("\n");
    int index=binarysearch(5,compare);
    if(index!=-1)
        printf("Element found at : %d\n",index);
    else
        printf("Element not found\n");
    printf("\n\n");
}