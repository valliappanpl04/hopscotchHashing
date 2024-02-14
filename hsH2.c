#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define H 32
#define noofSegments x
#define segmentSize y
#define datasize z
int x=10,y=50000,z=5000000,cnt=0;

int* loadfactor=NULL;

typedef struct hashtable{
    int key;
    void* val;
    int hop_info;
    int flag;
}table;

// returns a table with size mentioned in the macros
table** initializeTable(){
    table** arr=(table**)malloc(sizeof(table*)*noofSegments);
    for(int i=0;i<noofSegments;i++){
        arr[i]=(table*)malloc(sizeof(table)*segmentSize);
        for(int j=0;j<segmentSize;j++){
            arr[i][j].flag=0;
            arr[i][j].hop_info=0;
        }
    }
    return arr;
}

// deletes and free up space of the table
void deleteTable(table*** arr){
    for(int i=0;i<noofSegments;i++){
        free((*arr)[i]);
    }
    free(*arr);
}

// returns a hashvalue for the given key
int hashfunction(int key){
    return key;
    // return (((key*15)/4)*7)/2;
    return 9;
}

// generates a random string
char* generateStr(){
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

// adds element to the table
int add(table ***arr, int key, void *val){
    int total=noofSegments*segmentSize;
    int hash=hashfunction(key);
    int seg=hash%noofSegments;
    int buck=hash%segmentSize;
    int current_pos=(seg*segmentSize)+buck;
    int hop=0,i;
    for(i=0;i<total;i++){
        int index=(i+current_pos)%(total);
        int s=index/segmentSize,b=index%segmentSize;
        if((*arr)[s][b].flag==0)
            break;
        hop++;
    }
    if(i==total){
        printf("Segmentsize failed");
        return 0;
    }
    int free_index=(current_pos+hop)%(total);
    while(1){
        if(free_index<current_pos)
            free_index+=total;
        if(current_pos+H-1>=free_index){
            int s=(free_index%total)/segmentSize,b=(free_index%total)%segmentSize;
            (*arr)[s][b].flag=1;
            (*arr)[s][b].val=val;
            (*arr)[s][b].key=key;
            (*arr)[seg][buck].hop_info|=(1<<(free_index-current_pos));
            return 1;
        }
        int step=1;
        while(step<H){
            int xindex=(free_index-H+step+total)%total;
            int s=xindex/segmentSize,b=xindex%segmentSize;
            int xhash=(hashfunction((*arr)[s][b].key));
            int xseg=xhash%noofSegments,xbuck=xhash%segmentSize;
            int xcur_pos=(xseg*segmentSize)+xbuck;
            if(free_index<xcur_pos){
                free_index+=total;
            }
            if(xcur_pos+H-1>=free_index){
                int s1=(free_index%total)/segmentSize,b1=(free_index%total)%segmentSize;

                (*arr)[s1][b1].flag=1;
                (*arr)[s1][b1].key=(*arr)[s][b].key;
                (*arr)[s1][b1].val=(*arr)[s][b].val;

                (*arr)[s][b].flag=0;
                (*arr)[s][b].val=NULL;
                (*arr)[s][b].key=0;

                (*arr)[xseg][xbuck].hop_info&=~((xindex-xcur_pos)<<1);
                (*arr)[xseg][xbuck].hop_info|=((free_index-xcur_pos)<<1);

                free_index=xindex;
                break;
            }
            step++;
        }
        if(step==H){
            int hopinfo=(*arr)[seg][buck].hop_info;
            for(int abc=0;abc<H;abc++){
                if(((hopinfo>>abc)&1)==0){
                    return 0;
                }
            }
            cnt++;
            return -1;
            // return 0;
        }
    }
}

// resizes by doubling the size of the segment
table** resize(table*** arr){
    y*=2;
    table** temp=initializeTable();
    for(int i=0;i<noofSegments;i++){
        for(int j=0;j<segmentSize/2;j++){
            if((*arr)[i][j].flag==1)
                add(&temp, (*arr)[i][j].key, (*arr)[i][j].val);
        }
    }
    y/=2;
    deleteTable(arr);
    y*=2;
    return temp;
}

// to print the entire table
void print(table **arr){
    for(int i=0;i<noofSegments;i++){
        for(int j=0;j<segmentSize;j++){
            if(arr[i][j].flag==1)
                printf("%d:%s  ",j,arr[i][j].key, arr[i][j].val);
            else    
                printf(" -:-  ");
        }
        printf("\n");
    }
    printf("\n\n\n");
}

void deleteall(table** arr){
    for(int i=0;i<noofSegments;i++){
        for(int j=0;j<segmentSize;j++){
            if(arr[i][j].flag==1){
                free(arr[i][j].val);
            }
        }
    }
}

int contains(table** arr, int key){
    int hash=hashfunction(key);
    int seg=hash%noofSegments;
    int buck=hash%segmentSize;
    int hopinfo=arr[seg][buck].hop_info;
    int mask=1;
    for(int i=0;i<H;i++,mask<<=1){
        if(hopinfo&mask==mask){
            int s=seg+((buck+i)/segmentSize),b=(buck+i)%segmentSize;
            if(arr[s][b].key==key)
                return 1;
        }
    }
    return 0;
}

int main(){
    table** arr=initializeTable();
    char** wordarr=(char**)malloc(sizeof(char*)*datasize);
    int failedCount=0,resizeCount=0;
    // int inputdataset[]={9,39,48,78,18,1,3,4,5,6};
    for(int i=0;i<datasize;i++){
        wordarr[i]=generateStr();
        int in=add(&arr, i, wordarr[i]);
        if(in==0){
            resizeCount++;
            arr=resize(&arr);
            add(&arr,i,wordarr[i]);
        }
        if(in==-1)
            failedCount++;
        // print(arr);
    }
    printf("Segment size : %d\nNo of resizes(failed) : %d\n",segmentSize,resizeCount);
    printf("No of failed : %d\n",failedCount);
    free(wordarr);
    deleteall(arr);
    deleteTable(&arr);
    
}