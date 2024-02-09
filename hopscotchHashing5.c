#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define H 4
#define noOfSegments 10
#define segmentSize 1000
#define datasize 1000
typedef struct hashtable{
    int key;
    char *val;
    int flag;
}table;
int hashfunction(int key){
    return (((key*13)/3)*7)/2;
    // return key%10;
    // return (key*2)/3;
}
int contains(table arr[noOfSegments][segmentSize], int key){
    int hash=hashfunction(key);
    int seg=hash%noOfSegments;
    int buck=hash%segmentSize;
    for(int i=0;i<H;i++){
        int index=(buck+i)%segmentSize;
        if(arr[seg][index].flag==1 && arr[seg][index].key==key)
            return 1;
    }
    return 0;
}
// int contains(table**, int);
int add(table arr[noOfSegments][segmentSize], int key, char *val){
    if(contains(arr, key))
        return 0;
    int hash=hashfunction(key);
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
            arr[segment][index%segmentSize].key=key;
            arr[segment][index%segmentSize].flag=1;
            arr[segment][index%segmentSize].val=(char*)malloc(sizeof(char)*10000);
            strcpy(arr[segment][index%segmentSize].val, val);
            // arr[segment][index%segmentSize].val=val;
            return 1;
        }
        int step=1;
        while(step<H){
            int xindex=(index-H+step+segmentSize)%segmentSize;
            int xhash=hashfunction(arr[segment][xindex].key)%segmentSize;
            if(xhash+H-1-index>=0){
                arr[segment][index%segmentSize].key=arr[segment][xindex].key;
                // strcpy(arr[segment][index%segmentSize].val, arr[segment][xindex].val);
                arr[segment][index%segmentSize].val=(char*)malloc(sizeof(char)*10000);
                strcpy(arr[segment][index%segmentSize].val,arr[segment][xindex].val);
                // free(arr[segment][xindex].val);
                arr[segment][xindex].val=NULL;
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
                printf("%d:%s ",arr[i][j].key,arr[i][j].val);
            // else    
            //     printf(" - : - ");
        }
        printf("\n");
    }
    printf("\n\n");
}


void delete(table arr[noOfSegments][segmentSize], int key){
    if(!contains(arr, key))
        return;
    int hash=hashfunction(key);
    int seg=hash%noOfSegments;
    int buck=hash%segmentSize;
    for(int i=0;i<H;i++){
        int index=(buck+i)%segmentSize;
        if(arr[seg][index].flag==1 && arr[seg][index].key==key){
            free(arr[seg][index].val);
            arr[seg][index].flag=0;
            return;
        }
    }
}
void deleteall(table arr[noOfSegments][segmentSize]){
    for(int i=0;i<noOfSegments;i++){
        for(int j=0;j<segmentSize;j++){
            if(arr[i][j].flag==1){
                arr[i][j].key=0;
                free(arr[i][j].val);
                arr[i][j].flag=0;
            }
        }
    }
}

char* get(table arr[noOfSegments][segmentSize], int key){
    if(!contains(arr, key))
        return NULL;
    int hash=hashfunction(key);
    int seg=hash%noOfSegments;
    int buck=hash%segmentSize;
    for(int i=0;i<H;i++){
        int index=(buck+i)%segmentSize;
        if(arr[seg][index].key==key)
            return arr[seg][index].val;
    }
}

int main(){
    printf("\n");
    int wordlen=1, ascii=97;
    table arr[noOfSegments][segmentSize];
    // int failed[datasize],k=0;
    char word[datasize][100]={"hello","hello","hello","hello","hello","hello","hello","hello","hello","hello","hello","hello","hello","hello"};
    for(int i=0;i<noOfSegments;i++)
        for(int j=0;j<segmentSize;j++)
            arr[i][j].flag=0;
    for(int i=0;i<datasize;i++){
        // generating word
        char str[100];
        for(int i=0;i<wordlen;i++){
            str[i]=ascii++;
            if(ascii==122)
                ascii=97;
        }
        wordlen++;
        if(wordlen==100)
            wordlen=1;
        // generating word

        if(!add(arr, i, str)){}
            // failed[k++]=i;
    }
    print(arr);
    // printf("The elements are : ");
    // for(int i=0;i<;i++){
    //     int index=i;
    //     printf("The element with key %d is : %s\n",index, get(arr, index));
    // }
    deleteall(arr);
    add(arr, 2, "hi");
    print(arr);
    delete(arr,2);
    // printf("No. of elements failed to add are : %d\n",k);
    // printf("They are : ");
    // for(int i=0;i<k;i++)
    //     printf("%d ",failed[i]);
    // printf("\n\n");
    // delete(arr, 9999);
    // printf("\n");
    // int index=1;


    // if(contains(arr,1)){
    //     printf("Element present\n");
    // }
    // else    
    //     printf("Element not present\n");
    // if(contains(arr,1)){
    //     printf("Element present\n");
    // }
    // else    
    //     printf("Element not present\n");

    printf("\n");

}