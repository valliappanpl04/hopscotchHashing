#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
// Structure for Integer
typedef struct Integer{
    void (*add)(void***, int*, void*);
    int (*get)(void**, int);
    void (*sort)(void***, int );
    int (*search)(void**, int, void*);
    void (*print)(void**, int);
    void (*delete)(void***, int*);
}in;

// Structure for Character
typedef struct Character{
    void (*add)(void***, int*, void*);
    char (*get)(void**, int);
    void (*sort)(void***, int );
    int (*search)(void**, int, void*);
    void (*print)(void**, int);
    void (*delete)(void***, int*);
}ch;

//  Structure for String
typedef struct String{
    void (*add)(void***, int*, void*);
    char* (*get)(void**, int);
    void (*sort)(void***, int );
    int (*search)(void**, int, void*);
    void (*print)(void**, int);
    void (*delete)(void***, int*);
}st;

int compareInt(void *a, void *b){
    int num1=(int*)a, num2=(int*)b;
    return num1>num2?1:num1<num2?-1:0;
    return 0;
}
int compareChar(void *a, void *b){
    char ch1=(char*)a, ch2=(char*)b;
    return ch1>ch2?1:ch1<ch2?-1:0;
}
int compareStr(void *a, void *b){
    if(strcmp(a,b)>0)
        return 1;
    else if(strcmp(a,b)<0)
        return -1;
    else if (strcmp(a,b)==0)
        return 0;
}
// common functions
void add(void***, int*, void*);
void sort(void***, int, int (*fp)(void*,void*));
void delete(void***, int*);

// functions for Integers
int getInt(void**, int);
void sortInt(void***, int);
int binarySearchInt(void **, int, void*);
void printInt(void **, int);
// functions for Characters
char getChar(void**, int);
void sortChar(void***, int);
int binarySearchChar(void**, int, void*);
void printChar(void**, int);
// functions for string
char* getStr(void **, int);
void sortStr(void***, int );
int binarySearchStr(void**, int, void*);
void printStr(void**, int);

// Initializing Integer functions
in* initializeWithInt(){
    in *obj=(in*)malloc(sizeof(in));
    obj->add=add;
    obj->get=getInt;
    obj->sort=sortInt;
    obj->search=binarySearchInt;
    obj->print=printInt;
    obj->delete=delete;
    return obj;
}

// Initializing Character functions
ch* initializeWithChar(){
    ch *obj=(ch*)malloc(sizeof(ch));
    obj->add=add;
    obj->get=getChar;
    obj->sort=sortChar;
    obj->search=binarySearchChar;
    obj->print=printChar;
    obj->delete=delete;
    return obj;
}

// Initializing String functions
st* initializeWithStr(){
    st *obj=(st*)malloc(sizeof(st));
    obj->add=add;
    obj->get=getStr;
    obj->sort=sortStr;
    obj->search=binarySearchStr;
    obj->print=printStr;
    obj->delete=delete;
    return obj;
}
void add(void*** vect, int *size, void *ptr){
    *vect=realloc(*vect, sizeof(void*)*++*size);
    (*vect)[*size-1]=ptr;
}
void delete(void ***vect, int *size){
    *vect=realloc(*vect, sizeof(void*)*--*size);
}
int binarySeach(void **vect, int size, void *ptr, int (*compare)(void*, void*)){
    int start=0, end=size-1;
    while(start<=end){
        int mid=(start+end)/2;
        if(compare(vect[mid],ptr)==0){
            return mid;
        }
        else if(compare(vect[mid],ptr)==1)
            end=mid-1;
        else
            start=mid+1;
    }
    return -1;
}
void sort(void ***vect, int size, int (*compare)(void*, void*)){
    for(int i=0;i<size;i++){
        for(int j=i+1;j<size;j++){
            if(compare((*vect)[i], (*vect)[j])==1){
                void *ptr=(*vect)[i];
                (*vect)[i]=(*vect)[j];
                (*vect)[j]=ptr;
            }
        }
    }
}
// fucntions for int
int getInt(void **vect, int index){
    return (int*)vect[index];
}
void sortInt(void*** vect, int size){
    sort(vect, size, compareInt);
}
int binarySearchInt(void **vect, int size, void *ptr){
    return binarySeach(vect, size, ptr, compareInt);
}
void printInt(void **vect, int size){
    for(int i=0;i<size;i++){
        printf("%d ",vect[i]);
    }
    printf("\n");
}
// functions for char
char getChar(void **vect, int index){
    return (char*)vect[index];
}
void sortChar(void*** vect, int size){
    sort(vect, size, compareChar);
}
int binarySearchChar(void **vect, int size, void *ptr){
    return binarySeach(vect, size, ptr, compareChar);
}
void printChar(void **vect, int size){
    for(int i=0;i<size;i++){
        printf("%c ",vect[i]);
    }
    printf("\n");
}
// functions for string
char* getStr(void **vect, int index){
    return vect[index];
}
void sortStr(void*** vect, int size){
    sort(vect, size, compareStr);
}
int binarySearchStr(void **vect, int size, void *ptr){
    return binarySeach(vect, size, ptr, compareChar);
}
void printStr(void **vect, int size){
    for(int i=0;i<size;i++){
        printf("%s ",vect[i]);
    }
    printf("\n");
}

// char getChar(void **vect, int index){
//     return (char*)vect[index];
// }



int main(){
    void **vect=NULL;
    int size=0;

    in *obj=initializeWithInt();
    int n=100000000;
    printf("size before adding  : %d\n", size);
    for(int i=0;i<n;i++){
        obj->add(&vect, &size, i);
    }
    printf("Size of the array after adding 1 lakh data is : %d\n",size);
    for(int i=0;i<n;i++){
        obj->delete(&vect, &size);
    }
    printf("size of array after deleting all the elements : %d\n", size);
    // //Main Code for Integer
    // in *obj=initializeWithInt();
    // obj->add(&vect,&size,14);
    // obj->add(&vect,&size,80);
    // obj->add(&vect,&size,95);
    // obj->add(&vect,&size,30);
    // printf("Array before sorting is : ");
    // obj->print(vect, size);
    // printf("Element at index 2 : %d\n", obj->get(vect, 2));
    // obj->sort(&vect, size);
    // printf("After sorting the array is : ");
    // obj->print(vect, size);
    // printf("Search Element found in index : %d\n",obj->search(vect, size, 80));
    // printf("\n");

    // // Main Code for Character
    // ch *obj=initializeWithChar();
    // obj->add(&vect, &size, 'a');    
    // obj->add(&vect, &size, 'h');
    // obj->add(&vect, &size, 'e');
    // obj->add(&vect, &size, 'b');
    // obj->add(&vect, &size, 'c');
    // printf("Array before sorting is : ");
    // obj->print(vect, size);
    // printf("Element at Index 2 is : %c\n",obj->get(vect, 2));
    // obj->sort(&vect, size);
    // printf("Array After sorting is : ");
    // obj->print(vect, size);
    // printf("Search Element found at : %d\n",obj->search(vect, size, 'c'));
    
    
    // // Main Code for string
    // st *obj=initializeWithStr();
    // obj->add(&vect, &size, "hello");
    // obj->add(&vect, &size, "world");
    // obj->add(&vect, &size, "hey");
    // obj->add(&vect, &size, "hi");
    // obj->add(&vect, &size, "cat");
    // obj->add(&vect, &size, "dog");
    // printf("Array before sorting is : ");
    // obj->print(vect, size);
    // printf("Element found at index 3 : %s\n", obj->get(vect, 3));
    // obj->sort(&vect, size);
    // printf("Array after sorting is : ");
    // obj->print(vect, size);
    // printf("Search Element found at : %d", obj->search(vect, size, "hello"));



    printf("\n\n\n");
}