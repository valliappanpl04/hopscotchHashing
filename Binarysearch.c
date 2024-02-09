#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void add(void ***vect, int *size,void *ptr){
    *vect=realloc(*vect,sizeof(void*)*++(*size));
    (*vect)[*size-1]=ptr;
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
void sort(void ***vect, int *size, int (*compare)(void*,void*)){
    for(int i=0;i<*size;i++){
        for(int j=i+1;j<*size;j++){
            if(compare((*vect)[i],(*vect)[j])>0){
                void *ptr=(*vect)[i];
                (*vect)[i]=(*vect)[j];
                (*vect)[j]=ptr;
            }
        }
    }
}
int binarysearch(void ***vect, int *size, void *ptr, int (*compare)(void*, void*)){
    int start=0, end=*size-1;
    int mid=(start+end)/2;
    while(start<=end){
        mid=(start+end)/2;
        if(compare((*vect)[mid],ptr)==0)
            return mid;
        if(compare((*vect)[mid],ptr)>0){
            end=mid-1;
        }
        else{
            start=mid+1;
        }
    }
    return -1;
}

// void initVectorWithInt(){
    
// }



int main(){
    //  Declaring the vector
    void **vect=NULL;
    int size=0;

    // Adding Elements onto the array
    // add(&vect,&size,'d');
    // add(&vect,&size,'x');
    // add(&vect,&size,'c');
    // add(&vect,&size,'a');
    // add(&vect,&size,1);
    // add(&vect,&size,2);
    // add(&vect,&size,5);
    // add(&vect,&size,4);
    add(&vect,&size,"Hello");
    add(&vect,&size,"Apple");
    add(&vect,&size,"Hi");
    add(&vect,&size,"world");
    add(&vect,&size,"bat");
    add(&vect,&size,"Hey!!");

    int (*compare)(void*, void*)=&comparestr; // declaring function pointer

    sort(&vect, &size, compare);
    printf("Sorted Array: ");
    for(int i=0;i<size;i++){
        printf("%s ",vect[i]);  //Printing Array after sorting 
    }

    printf("\n");

    // Implementing Binary Search
    int index=binarysearch(&vect, &size, "Hi",compare);
    if(index!=-1)
        printf("Element found at : %d\n",index);
    else
        printf("Element not found\n");


    printf("\n\n");
}