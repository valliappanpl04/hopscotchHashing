#include<stdio.h>
#define H 4
#define noOfSegments 10
#define segmentSize 10000
#define datasize 100000
typedef struct hashtable{
    int key;
    int flag;
}table;

int hashfunction(int key){
    return (((key*13)/3)*7)/2;
}
int contains(table arr[noOfSegments][segmentSize], int val){
    int hash=hashfunction(val);
    int seg=hash%noOfSegments;
    int buck=hash%segmentSize;
    for(int i=0;i<H;i++){
        int index=(buck+i)%segmentSize;
        if(arr[seg][index].flag==1 && arr[seg][index].key==val)
            return 1;
    }
    return 0;
}
// int contains(table**, int);
int add(table arr[noOfSegments][segmentSize], int val){
    if(contains(arr, val))
        return 0;
    int hash=hashfunction(val);
    int segment = hash%noOfSegments;
    int bucket = hash%segmentSize;
    int hop=0,i;
    for(i=0;i<segmentSize;i++){
        int index=(bucket+i)%segmentSize;
        if(arr[segment][index].flag==0)
            break;
        hop++;
    }
    if(i==segmentSize)
        return 0;
    int index=hop+bucket;
    while(1){
        if(hash+H-1-index>=0){
            arr[segment][index%segmentSize].key=val;
            arr[segment][index%segmentSize].flag=1;
            return 1;
        }
        int step=1;
        while(step<H){
            int xindex=(index-H+step+segmentSize)%segmentSize;
            int xhash=hashfunction(arr[segment][xindex].key)%segmentSize;
            if(xhash+H-1-xindex>=0){
                arr[segment][index%segmentSize].key=arr[segment][xindex].key;
                arr[segment][index%segmentSize].flag=1;
                arr[segment][xindex].flag=0;
                index=xindex;
                break;
            }
            step++;
        }
        if(step==H)
            return 0;
    }
}
void print(table arr[noOfSegments][segmentSize]){
    for(int i=0;i<noOfSegments;i++){
        for(int j=0;j<segmentSize;j++){
            if(arr[i][j].flag==1)
                printf("%d ",arr[i][j].key);
            else    
                printf(" - ");
        }
        printf("\nHello\n");
    }
    printf("\n\n");
}


void delete(table arr[noOfSegments][segmentSize], int val){
    if(!contains(arr, val))
        return;
    int hash=hashfunction(val);
    int seg=hash%noOfSegments;
    int buck=hash%segmentSize;
    for(int i=0;i<H;i++){
        int index=(buck+i)%segmentSize;
        if(arr[seg][index].key==val){
            arr[seg][index].flag=0;
            return;
        }
    }
}
void deleteall(table arr[noOfSegments][segmentSize]){
    for(int i=0;i<noOfSegments;i++){
        for(int j=0;j<segmentSize;j++){
            arr[i][j].key=0;
            arr[i][j].flag=0;
        }
    }
}
int main(){
    printf("\n");
    table arr[noOfSegments][segmentSize];
    int failed[datasize],k=0;
    for(int i=0;i<noOfSegments;i++)
        for(int j=0;j<segmentSize;j++)
            arr[i][j].flag=0;
    for(int i=1;i<datasize;i++){
        if(!add(arr, i))
            failed[k++]=i;
        // print(arr);
    }
    // print(arr);
    printf("No. of elements failed to add are : %d\n",k);
    printf("They are : ");
    for(int i=0;i<k;i++)
        printf("%d ",failed[i]);
    printf("\n");
    // delete(arr, 9999);
    if(contains(arr,1)){
        printf("Element present\n");
    }
    else    
        printf("Element not present\n");
    deleteall(arr);
    if(contains(arr,1)){
        printf("Element present\n");
    }
    else    
        printf("Element not present\n");

}