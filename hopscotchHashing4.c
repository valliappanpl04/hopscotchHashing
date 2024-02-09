#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int cnt=0;
#define H 4
#define noOfSegments x
#define segmentSize y
#define datasize z
int x=10,y=5000, z=50000;
typedef struct hashtable{
    int key;
    char *val;
    int flag;
}table;
int contains(table**, int);
void delete(table***, int);
void deleteall(table***);
table** resize(table***);
int add(table***, int, char*);
void print(table**);
char* get(table**, int);
int hashfunction(int key){
    return (((key*13)/3)*7)/2;
    // return 0;
}
int contains(table **arr, int key){
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
void delete(table ***arr, int key){
    if(!contains((*arr), key))
        return;
    int hash=hashfunction(key);
    int seg=hash%noOfSegments;
    int buck=hash%segmentSize;
    for(int i=0;i<H;i++){
        int index=(buck+i)%segmentSize;
        if((*arr)[seg][index].flag==1 && (*arr)[seg][index].key==key){
            free((*arr)[seg][index].val);
            (*arr)[seg][index].flag=0;
            return;
        }
    }
}
void deleteall(table ***arr){
    // print(*arr);
    for(int i=0;i<noOfSegments;i++){
        for(int j=0;j<segmentSize;j++){
            if((*arr)[i][j].flag==1){
                // printf("y");
                (*arr)[i][j].key=0;
                free((*arr)[i][j].val);
                (*arr)[i][j].flag=0;
            }
        }
    }
}
// table** resize(table ***arr){
//     // for(int i=0;i<datasize;i++){
//     //     printf("%d : %s",(*arr)[0][i].key, (*arr)[0][i].val);
//     // }
//     y*=2;
//     table **temp=(table**)calloc(noOfSegments,sizeof(table*));
//     // printf("\n");
//     for(int i=0;i<noOfSegments;i++){
//         temp[i]=(table*)calloc(segmentSize,sizeof(table));
//         for(int j=0;j<segmentSize;j++)
//             temp[i][j].flag=0;
//     }
//     for(int i=0;i<noOfSegments;i++){
//         for(int j=0;j<segmentSize/2;j++){
//             if((*arr)[i][j].flag==1)
//                 add(&temp, (*arr)[i][j].key, (*arr)[i][j].val);
//         }
//     }
//     y/=2;
//     // for(int i=0;i<noOfSegments;i++){
//     //     for(int j=0;j<segmentSize;j++){
//     //         if((*arr)[i][j].flag==1){
//     //             (*arr)[i][j].flag=0;
//     //             (*arr)[i][j].key=0;
//     //             free((*arr)[i][j].val);
//     //         }
//     //     }
//     // }
//     deleteall(arr);
//     for(int i=0;i<noOfSegments;i++){
//         free((*arr)[i]);
//     }
//     free((*arr));
//     y*=2;
//     return temp;
//     // return *arr;
// }
table** resize(table ***arr){
    y*=2;
    table **temp=(table**)calloc(noOfSegments, sizeof(table*));
    for(int i=0;i<noOfSegments;i++){
        temp[i]=(table*)calloc(segmentSize, sizeof(table));
    }
    for(int s=0;s<noOfSegments;s++){
        for(int b=0;b<segmentSize/2;b++){
            int hash=hashfunction((*arr)[s][b].key);
            int segment = hash%noOfSegments;
            int bucket = hash%segmentSize;
            int hop=0,i;
            for(i=0;i<segmentSize;i++){
                int index=(bucket+i)%segmentSize;
                if(temp[segment][index].flag==0)
                    break;
                hop++;
            }
            if(i==segmentSize)
                continue;
            int index=hop+bucket;
            while(1){
                if(hash+H-1-index>=0){
                    temp[segment][index%segmentSize].key=(*arr)[s][b].key;
                    temp[segment][index%segmentSize].flag=1;
                    temp[segment][index%segmentSize].val=(*arr)[s][b].val;
                    // strcpy((*arr)[segment][index%segmentSize].val, val);
                    // arr[segment][index%segmentSize].val=val;
                    break;
                }
                int step=1;
                while(step<H){
                    int xindex=(index-H+step+segmentSize)%segmentSize;
                    int xhash=hashfunction(temp[segment][xindex].key)%segmentSize;
                    if(xhash+H-1-index>=0){
                        temp[segment][index%segmentSize].key=temp[segment][xindex].key;
                        // strcpy(arr[segment][index%segmentSize].val, arr[segment][xindex].val);
                        temp[segment][index%segmentSize].val=temp[segment][xindex].val;
                        // strcpy((*arr)[segment][index%segmentSize].val,(*arr)[segment][xindex].val);
                        free(temp[segment][xindex].val);
                        temp[segment][xindex].val=NULL;
                        temp[segment][index%segmentSize].flag=1;
                        temp[segment][xindex].flag=0;
                        index=xindex;
                        break;
                    }
                    step++;
                }
                if(step==H)
                    break;
            }
        }
    }
    for(int i=0;i<noOfSegments;i++){
        free((*arr)[i]);
    }
    free((*arr));
    return temp;
    // return (*arr);
}

int add(table ***arr, int key, char *val){
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
    int index=hop+bucket;
    while(1){
        if(hash+H-1-index>=0){
            (*arr)[segment][index%segmentSize].key=key;
            (*arr)[segment][index%segmentSize].flag=1;
            (*arr)[segment][index%segmentSize].val=(char*)malloc(sizeof(char)*10000);
            strcpy((*arr)[segment][index%segmentSize].val, val);
            // arr[segment][index%segmentSize].val=val;
            return 1;
        }
        int step=1;
        while(step<H){
            int xindex=(index-H+step+segmentSize)%segmentSize;
            int xhash=hashfunction((*arr)[segment][xindex].key)%segmentSize;
            if(xhash+H-1-index>=0){
                (*arr)[segment][index%segmentSize].key=(*arr)[segment][xindex].key;
                // strcpy(arr[segment][index%segmentSize].val, arr[segment][xindex].val);
                (*arr)[segment][index%segmentSize].val=(char*)malloc(sizeof(char)*10000);
                strcpy((*arr)[segment][index%segmentSize].val,(*arr)[segment][xindex].val);
                free((*arr)[segment][xindex].val);
                (*arr)[segment][xindex].val=NULL;
                (*arr)[segment][index%segmentSize].flag=1;
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
}



char* get(table **arr, int key){
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
    // table arr[noOfSegments][segmentSize];
    table **arr=(table**)calloc(noOfSegments,sizeof(table*));
    for(int i=0;i<noOfSegments;i++){
        arr[i]=(table*)calloc(segmentSize,sizeof(table));
    }
    // int failed[datasize],k=0;
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

        if(!add(&arr, i, str));
            // failed[k++]=i;
    }
    // print(arr);
    // printf("failed : %d\n",k);
    printf("\n");
    // arr=resize(&arr);
    // print(arr);
    arr=resize(&arr);
    // print(arr);
    deleteall(&arr);
    // print(a  rr);
    // add(&arr, 2, "hi");
    printf("%d",segmentSize);
    // print(arr);

    printf("\n");

}