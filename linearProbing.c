#include<stdio.h>
#include<stdlib.h>

#define tablesize 10000
#define datasize 10000

int hash(int key){
    return key%tablesize;
}

void add(int arr[], int flag[], int key){
    if(search(arr, flag, key)){
        printf("%d already present\n",key);
    }
    int h=hash(key);
    for(int i=h;i<h+tablesize;i++){
        int index=i%tablesize;
        if(flag[index]==0){
            arr[index]=key;
            flag[index]=1;
            return;
        }
    }
    printf("Table is full cannot add %d\n",key);
}
int search(int arr[], int flag[], int key){
    int h=hash(key);
    for(int i=h;i<h+tablesize;i++){
        int index=i%tablesize;
        if(flag[index]==1 && arr[index]==key){
            return 1;
        }
    }
    return 0;
}
int main(){
    int arr[tablesize], flag[tablesize];
    // memset(arr, -1, sizeof(arr));
    memset(flag, 0, sizeof(flag));
    for(int i=0;i<datasize*2;i+=2){
        add(arr, flag, i);
    }
    for(int i=0;i<datasize*2;i+=2){
        search(arr, flag, i);
    }
    // if(search(arr, flag, )){
    //     printf("Found");
    // }
    // else    
    //     printf("Not found");
    printf("\n");
}