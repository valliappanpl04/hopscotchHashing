#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

// define constants
#define H 32
#define noOfSegments x
#define segmentSize y
#define dataSize z
int x=10, y=1000, z=10000;
// int x=10, y=1000, z=10000;

// structure for bucket
typedef struct hashtable{
    int key;
    void* val;
    int flag;
    int hopinfo;
}table;

// function declaration
void print (table**);
table** resize(table***);

// to generate hashvalue
int hashfunction(int key){
    // return (key*123435)/6;
    return (((key*15)/4)*7);
    // return key%5;
    return key;
    // return ((key*key));
}

// to delete the table itself
void deletetable(table*** arr){
    for(int i=0;i<noOfSegments;i++){
        free((*arr)[i]);
    }
    free(*arr);
}

// to initialize the table
table** initializeTable(){
    table** arr=(table**)malloc(sizeof(table*)*noOfSegments);
    for(int i=0;i<noOfSegments;i++){
        arr[i]=(table*)malloc(sizeof(table)*segmentSize);
        for(int j=0;j<segmentSize;j++){
            arr[i][j].flag=0;
        }
    }
    return arr;
}

int contains(table** arr, int key){
    int hash=hashfunction(key);
    int seg=hash%noOfSegments;
    int buck=hash%segmentSize;
    for(int i=0;i<H;i++){
        if((arr[seg][buck].hopinfo>>i)&1==1){
            int index=buck+i;
            if(arr[seg][index].key==key)
                return 1;
        }
    }
}

// to add elements inside the table
int add(table*** arr, int key, void *val){
    int hash=hashfunction(key);
    int seg=hash%noOfSegments;
    int buck=hash%segmentSize;
    int hop=0,i=0;
    for(i=0;i<segmentSize;i++){
        int index=buck+i;
        if((*arr)[seg][index%segmentSize].flag==0){
            break;
        }
        hop++;
    }
    if(i==segmentSize){
        (*arr)=resize(arr);
        free(val);
        return 0;
    }
    int index=(hop+buck);
    while(1){
        if((buck+H-1-index)>=0){
            (*arr)[seg][index%segmentSize].flag=1;
            (*arr)[seg][index%segmentSize].key=key;
            (*arr)[seg][index%segmentSize].val=val;
            (*arr)[seg][buck].hopinfo|=(1<<(index-buck));
            return 1;
        }
        int step=1;
        while(step<H){
            int xindex=(index-H+step+segmentSize)%segmentSize;
            int xhash=hashfunction((*arr)[seg][xindex].key);
            int xseg=xhash%noOfSegments;
            int xbuck=xhash%segmentSize;
            if((xbuck+H-1-index)>=0){
                (*arr)[seg][index%segmentSize].flag=1;
                (*arr)[seg][index%segmentSize].val=(*arr)[seg][xindex].val;
                (*arr)[seg][index%segmentSize].key=(*arr)[seg][xindex].key;
                (*arr)[seg][xbuck].hopinfo|=(1<<(index-xbuck));

                (*arr)[seg][xindex].flag=0;
                (*arr)[seg][xindex].val=NULL;
                (*arr)[seg][xindex].key=0;
                (*arr)[seg][xbuck].hopinfo&=~(1<<(xindex-xbuck));
                index=xindex;
                break;
            }
            step++;
        }
        if(step==H){
            free(val);
            return 0;
        }
    }
}

// to resize the table
table** resize(table ***arr){
    y*=2;
    table** temp=initializeTable();
    for(int i=0;i<noOfSegments;i++){
        for(int j=0;j<segmentSize/2;j++){
            add(&temp, (*arr)[i][j].key, (*arr)[i][j].val);
        }
    }
    y/=2;
    deletetable(arr);
    y*=2;
    return temp;
}

// to delete the entire data inside the table
void deleteall(table*** arr){
    int cnt=0;
    for(int i=0;i<noOfSegments;i++){
        for(int j=0;j<segmentSize;j++){
            if((*arr)[i][j].flag==1){
                cnt+=1;
                free((*arr)[i][j].val);
            }
        }
    }
    printf("-%d-",cnt);
}

// to print the entire table
void print (table** arr){
    for(int i=0;i<noOfSegments;i++){
        for(int j=0;j<segmentSize;j++){
            printf("%d ",arr[i][j].hopinfo);
        }
    }
    printf("\n");
    for(int i=0;i<noOfSegments;i++){
        for(int j=0;j<segmentSize;j++){
            if(arr[i][j].flag==1)
                printf("%d:%s ",arr[i][j].key, arr[i][j].val);
            else    
                printf("- : - ");
        }
    }
    printf("\n\n");
}

// to generate random string 
char* generateStr2(){
    static int mySeed = 25011984;
    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    int stringLen = strlen(string);        
    char *randomString = malloc(sizeof(char) * (11));
    srand(time(NULL) * 10 + ++mySeed);
    if (randomString) {
        short key = 0;
        for (int n = 0;n < 10;n++) {            
            key = rand() % stringLen;          
            randomString[n] = string[key];
        }
        randomString[10] = '\0';
        return randomString;        
    }
}
// to check value
int containselement(table** arr, int key, void *val){
    int hash=hashfunction(key);
    int seg=hash%noOfSegments;
    int buck=hash%segmentSize;
    for(int i=0;i<H;i++){
        int index=buck+i;
        if((arr[seg][buck].hopinfo>>i)&1==1 && arr[seg][index].val==val){
            return 1;
        }
    }
    return 0;
}
int main(){
    table** arr=initializeTable();
    int k=0,cnt=0;
    char** wordarr=(char**)malloc(sizeof(char*)*dataSize);
    for(int i=0;i<dataSize;i++){
        wordarr[i]=generateStr2();
        if(!add(&arr, i, wordarr[i]))
            // failed[k++]=i;
            k++;
    }
    for(int i=0;i<dataSize;i++){
        // if(!containselement(arr, i, wordarr[i])){
        //     cnt++;
        // }
        if(!containselement(arr, i, wordarr[i]))
            cnt++;
    }
    printf("failed to add : %d\n",k);
    printf("Elements not present : %d\n",cnt);
    free(wordarr);
    // arr=resize(&arr);
    deleteall(&arr);
    deletetable(&arr);
}