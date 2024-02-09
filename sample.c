#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#define noOfSegments 1
#define segmentSize 10
typedef struct hashtable{
    int key;
    char* val;
    int flag;
}table;

void resize(int arr[]){
    arr=(int*)realloc(arr, sizeof(int)*1000);
}

int main(){
    int **arr=(int**)calloc(10,sizeof(int*));
    for(int i=0;i<10;i++){
        arr[i]=(int*)calloc(100,sizeof(int));
    }
    for(int i=0;i<10;i++){
        for(int j=0;j<100;j++){
            printf("%d ",arr[i][j]);
        }
        printf("\n");
    }
}