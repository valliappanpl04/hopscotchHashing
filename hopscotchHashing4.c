#include<stdio.h>   
#include<stdlib.h>
#include<string.h>


#define H 4
#define noOfSegments x
#define segmentSize y
#define datasize z
int x=1,y=100, z=100;


typedef struct hashtable{
    int key;
    void *val;
    int hopinfo;
    int flag;
}table;

int contains(table**, int);
void delete(table***, int);
void deleteall(table***);
table** resize(table***);
int add(table***, int, void*);
void print(table**);
char* get(table**, int);


// int hashfunction -> for creating hashvalue
int hashfunction(int key){
    return (((key*13)/3)*7)/2;
    // return key-(key%2);
    // return 0;
}

// int contains -> To check whether a key is present or not
int contains(table **arr, int key){
    int hash=hashfunction(key);
    int seg=hash%noOfSegments;
    int buck=hash%segmentSize;
    for(int i=0;i<H;i++){
        if((arr[seg][buck].hopinfo>>i)&1==1){
            int index=(buck+i)%segmentSize;
            if(arr[seg][index].flag==1 && arr[seg][index].key==key)
                return 1;
        }
    }
    return 0;
}

// void delete -> To delete a particular key from the table
void delete(table ***arr, int key){
    if(!contains((*arr), key))
        return;
    int hash=hashfunction(key);
    int seg=hash%noOfSegments;
    int buck=hash%segmentSize;
    for(int i=0;i<H;i++){
        int index=(buck+i)%segmentSize;
        if(((*arr)[seg][index].hopinfo>>i)&1==1){
            if((*arr)[seg][index].flag==1 && (*arr)[seg][index].key==key){
                (*arr)[seg][buck].hopinfo&=~(1<<i);
                free((*arr)[seg][index].val);
                (*arr)[seg][index].flag=0;
                return;
            }
        }
    }
}

// void deleteall -> to delete all the values inside the table
void deleteall(table ***arr){
    for(int i=0;i<noOfSegments;i++){
        for(int j=0;j<segmentSize;j++){
            if((*arr)[i][j].flag==1){
                (*arr)[i][j].hopinfo=0;
                (*arr)[i][j].key=0;
                free((*arr)[i][j].val);
                (*arr)[i][j].flag=0;
            }
        }
    }
}

// void resize -> to resize(double) the table size and rehash the entries
table** resize(table ***arr){
    y*=2;
    table **temp=(table**)calloc(noOfSegments, sizeof(table*));
    for(int i=0;i<noOfSegments;i++){
        temp[i]=(table*)calloc(segmentSize, sizeof(table));
    }
    for(int s=0;s<noOfSegments;s++){
        for(int b=0;b<segmentSize/2;b++){
            add(&temp, (*arr)[s][b].key, (*arr)[s][b].val);
        }
    }
    for(int i=0;i<noOfSegments;i++){
        free((*arr)[i]);
    }
    free((*arr));
    return temp;
    // return (*arr);
}

// adding element to the table
int add(table ***arr, int key, void *val){
    if(contains((*arr), key))
        return 0;
    int hash=hashfunction(key);
    int segment = hash%noOfSegments;
    int bucket = hash%segmentSize;
    int hop=0,i;
    for(i=0;i<segmentSize;i++){
        int index=(bucket+i)%segmentSize;
        if((*arr)[segment][index].flag==0)
            break;
        hop++;
    }
    if(i==segmentSize)
        return 0;
    int index=(hop+bucket);
    while(1){
        if((bucket+H-1)%segmentSize<=index){
            (*arr)[segment][index].key=key;
            (*arr)[segment][index].flag=1;
            // (*arr)[segment][index%segmentSize].val=(char*)malloc(sizeof(char)*10000);
            (*arr)[segment][index].val=val;
            (*arr)[segment][bucket].hopinfo|=(1<<hop);
            // strcpy((*arr)[segment][index%segmentSize].val, val);
            return 1;
        }
        int step=1;
        while(step<H){
            int xindex=(index-H+step+segmentSize)%segmentSize;
            int xhash=hashfunction((*arr)[segment][xindex].key)%segmentSize;
            int xbuck=xhash%segmentSize,xseg=xhash%noOfSegments;
            if((xbuck+H-1)%segmentSize<=index){
                (*arr)[segment][index].key=(*arr)[segment][xindex].key;
                // (*arr)[segment][index%segmentSize].val=(char*)malloc(sizeof(char)*10000);
                (*arr)[segment][index].val=(*arr)[segment][xindex].val;
                (*arr)[xseg][xbuck].hopinfo|=(1<<(index-xbuck));
                (*arr)[xseg][xbuck].hopinfo&=~(1<<(xindex-xbuck));
                // strcpy((*arr)[segment][index%segmentSize].val,(*arr)[segment][xindex].val);
                // free((*arr)[segment][xindex].val);
                (*arr)[segment][xindex].val=NULL;
                (*arr)[segment][index].flag=1;
                (*arr)[segment][xindex].flag=0;
                index=xindex;
                break;
            }
            step++;
        }
        if(step==H)
            return 0;
    }
}

// void print -> to print the entire table
void print(table **arr){
    for(int i=0;i<noOfSegments;i++){
        for(int j=0;j<segmentSize;j++){
            if(arr[i][j].flag==1)
                printf("%d:%s ",arr[i][j].key,arr[i][j].val);
            else    
                printf(" - : - ");
        }
        printf("\n");
    }
    printf("\n\n");
    for(int i=0;i<noOfSegments;i++){
        for(int j=0;j<segmentSize;j++){
            printf("%d ",arr[i][j].hopinfo);
        }
        printf("\n");
    }
}


// char* get-> to get the value for a particular key

char* get(table **arr, int key){
    if(!contains(arr, key))
        return NULL;
    int hash=hashfunction(key);
    int seg=hash%noOfSegments;
    int buck=hash%segmentSize;
    for(int i=0;i<H;i++){
        int index=(buck+i)%segmentSize;
        if((arr[seg][buck].hopinfo>>i)&1==1){
            if(arr[seg][index].key==key)
                return arr[seg][index].val;
        }
    }
}

int wordlen=1, ascii=97;
char* generatestr(){
    char* str=(char*)malloc(sizeof(char)*1000);
    for(int i=0;i<wordlen;i++){
        str[i]=ascii++;
        if(ascii==122)
            ascii=97;
    }
    wordlen++;
    if(wordlen==100)
        wordlen=1;
        return str;
}

int main(){
    printf("\n");
    table **arr=(table**)calloc(noOfSegments,sizeof(table*));
    for(int i=0;i<noOfSegments;i++){
        arr[i]=(table*)calloc(segmentSize,sizeof(table));
    }
    int failed[datasize],k=0;
    for(int i=0;i<noOfSegments;i++)
        for(int j=0;j<segmentSize;j++)
            arr[i][j].flag=0;
    for(int i=0;i<datasize;i++){
        if(!add(&arr, i, generatestr()));
            failed[k++]=i;
    }
    // print(arr);
    printf("%d",k);
    printf("\n");
    arr=resize(&arr);
    // print(arr);
    deleteall(&arr);

    // for(int i=0;i<datasize;i++){
    //     if(!add(&arr, i, generatestr()));
    //         // failed[k++]=i;
    // }
}