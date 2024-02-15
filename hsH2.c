#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define H 32
#define noofSegments x
#define segmentSize y
#define datasize z
#define upperLimit 10000000
#define lowerLimit 1000
int x=10,y=5000,z=50000;
int hHsize=0, segsize=0;
// int x=1,y=10,z=10;
// int x=3, y=10, z=10;

int* loadfactor=NULL;
int load=0;

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
    // return (((key*15)/4)*7)/2;
    return (13*key+2367)%17401;
}

// generates a random string
char* generateStr(){
    // static int mySeed = 25011984;
    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    int stringLen = strlen(string);        
    char *randomString = malloc(sizeof(char) * (11));
    // srand(time(NULL) * 10 + ++mySeed);
    // srand(time(0));
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

// generates a random number within a range
int generateInt(){
    // srand(time(0));
    return (rand()%(upperLimit-lowerLimit))+lowerLimit;
}

int contains(table** arr, int key){
    int hash=hashfunction(key);
    int seg=hash%noofSegments;
    int buck=hash%segmentSize;
    int hopinfo=arr[seg][buck].hop_info;
    int mask=1;
    for(int i=0;i<H;i++,mask<<=1){
        if((hopinfo>>i)&1==1){
            int s=(seg+((buck+i)/segmentSize))%noofSegments,b=(buck+i)%segmentSize;
            if(arr[s][b].key==key)
                return 1;
        }
    }
    return 0;
}

// adds element to the table
int add(table ***arr, int key, void *val){
    if(contains(*arr, key)){
        free(val);
        return -2;
    }
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
        segsize++;
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
            loadfactor[s]++;
            load++;
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

                (*arr)[xseg][xbuck].hop_info&=~(1<<(xindex-xcur_pos));
                (*arr)[xseg][xbuck].hop_info|=(1<<(free_index-xcur_pos));

                free_index=xindex;
                break;
            }
            step++;
        }
        if(step==H){
            printf("%d %d\n", seg, buck);
            int hopinfo=(*arr)[seg][buck].hop_info;
            for(int abc=0;abc<H;abc++){
                if(((hopinfo>>abc)&1)==0){
                    printf("Load factor is : %f%%\n", ((float)load/(float)total)*100);
                    printf("Load Factor of the segment is : %f%%\n", ((float)loadfactor[seg]/(float)segmentSize)*100);
                    hHsize++;
                    return 0;
                }
            }
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
                printf("%d:%s  ",arr[i][j].key, (char*)arr[i][j].val);
            else    
                printf(" -:-  ");
        }
        printf("\n");
    }
    printf("\n");
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


int main(){
    table** arr=initializeTable();
    char** wordarr=(char**)malloc(sizeof(char*)*datasize);
    int* keyarr=(int*)malloc(sizeof(int)*datasize);
    loadfactor=(int*)calloc(noofSegments, sizeof(int));
    // int inputdataset[]={9, 39, 18, 48, 78, 1,2,3,4.5};
    int failedCount=0,resizeCount=0,present=0;
    for(int i=0;i<datasize;i++){
        wordarr[i]=generateStr();
        keyarr[i]=generateInt();
        int in=add(&arr, keyarr[i], wordarr[i]);
        if(in==0){  
            resizeCount++;
            arr=resize(&arr);
            add(&arr,keyarr[i],wordarr[i]);
        }
        if(in==-1)
            failedCount++;
        if(in==-2){
            // for(int j=0;j<=i;j++){
            //     if(keyarr[i]==keyarr[j]){
            //         printf("%d %d\n",i, j);
            //         break;
            //     }
            // }
            present++;
        }
        // print(arr);
    }
    int failedToFetch=0;
    for(int i=0;i<datasize;i++){
        if(!contains(arr, keyarr[i]))
            failedToFetch++;
    }
    printf("Segment size : %d\nNo of resizes(failed) : %d\n",segmentSize,resizeCount);
    printf("No of failed : %d\n",failedCount);
    printf("Elements failed to fetch : %d\n",failedToFetch);
    printf("Duplicate elements : %d\n",present);
    printf("No of resizes due to H size overflow : %d\n",hHsize);
    printf("No of resizes due to segment size overflow : %d\n", segsize);
    // struct timeval stop, start;
    // gettimeofday(&start, NULL);
    // for(int i=0;i<1;i++){
    //     if(contains(arr, generateInt()));
    // }
    // gettimeofday(&stop, NULL);
    // printf("took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
    for(int i=0;i<100;i++){
        struct timeval start, end;
        gettimeofday(&start, NULL);
        if(contains(arr, generateInt()));
        gettimeofday(&end, NULL);
        printf("execution time is : %ld microseconds\n", (end.tv_sec-start.tv_sec)*1000000L+(end.tv_usec-start.tv_usec));
    }


    int notpresent=0, check=0;
    for(int i=0;i<100000;i++){
        int key=generateInt();
        if(!contains(arr, key)){
            notpresent++;
            int flag=0;
            for(int j=0;j<datasize;j++){
                if(keyarr[j]==key)
                    flag=1;
            }
            if(flag==0)
                check++;
        }
    }
    printf("No of elements not present in the array : %d\nNo of elements not present in the tabel : %d\n", check, notpresent);
    free(wordarr);
    free(keyarr);
    deleteall(arr);
    deleteTable(&arr);
    
}