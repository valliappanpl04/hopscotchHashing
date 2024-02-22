#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <sys/time.h>
#include <math.h>

#define H 32
#define noofSegments x
#define segmentSize y
#define datasize z
#define upperLimit 100000000
#define lowerLimit 1000
int x=10,y=500000,z=5000000;
int hHsize=0, segsize=0;
// int x=1,y=10,z=10;
// int x=3, y=10, z=10;

int* loadfactor=NULL;
int load=0;

typedef struct hashtable{
    void* key;
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

// returns hash value 
__uint64_t hashfunction(void* key , int len){
    const __uint64_t m = 0xc6a4a7935bd1e995;
    const int r = 47;
    long long int seed=123;
    __uint64_t h = seed ^ (len * m);

    const __uint64_t * data = (const __uint64_t *)key;
    const __uint64_t * end = data + (len/8);

    while(data != end)
    {
    __uint64_t k = *data++;

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;
    }

    const unsigned char * data2 = (const unsigned char*)data;
 
    switch(len & 7) {
        case 7: h ^= (__uint64_t)((__uint64_t)data2[6] << (__uint64_t)48);
        case 6: h ^= (__uint64_t)((__uint64_t)data2[5] << (__uint64_t)40);
        case 5: h ^= (__uint64_t)((__uint64_t)data2[4] << (__uint64_t)32);
        case 4: h ^= (__uint64_t)((__uint64_t)data2[3] << (__uint64_t)24);
        case 3: h ^= (__uint64_t)((__uint64_t)data2[2] << (__uint64_t)16);
        case 2: h ^= (__uint64_t)((__uint64_t)data2[1] << (__uint64_t)8 );
        case 1: h ^= (__uint64_t)((__uint64_t)data2[0]);
        h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;
    return h;
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

int contains(table** arr, void* key){
    unsigned long long int hash=hashfunction(key, sizeof(int));
    // int seg=hash&(noofSegments-1);
    // int buck=hash&(segmentSize-1);
    int seg=hash%noofSegments;
    int buck=hash%segmentSize;
    int hopinfo=arr[seg][buck].hop_info;
    int mask=1;
    for(int i=0;i<H;i++,mask<<=1){
        if((hopinfo>>i)&1==1){
            int s=(seg+((buck+i)/segmentSize))%noofSegments,b=(buck+i)%segmentSize;
            if(*(int*)arr[s][b].key==*(int*)key)
                return 1;
        }
    }
    return 0;
}

// adds element to the table
int add(table ***arr, void* key, void *val){
    if(contains(*arr, key)){
        free(val);
        return -2;
    }
    int total=noofSegments*segmentSize;
    unsigned long long int hash=hashfunction(key, sizeof(int));
    // int seg=hash&(noofSegments-1);
    // int buck=hash&(segmentSize-1);
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
            unsigned long long int xhash=(hashfunction((*arr)[s][b].key, sizeof(int)));
            // int xseg=xhash&(noofSegments-1),xbuck=xhash&(segmentSize-1);
            int xseg=xhash%noofSegments, xbuck=xhash%segmentSize;
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
            // printf("%d %d\n", seg, buck);
            // int hopinfo=(*arr)[seg][buck].hop_info;
            // for(int abc=0;abc<H;abc++){
            //     if(((hopinfo>>abc)&1)==0){
            //         printf("Load factor is : %f%%\n", ((float)load/(float)total)*100);
            //         printf("Load factors of the segment are : \n");
            //         for(int i=0;i<noofSegments;i++){
            //             printf("%f%% ", ((float)loadfactor[i]/(float)segmentSize)*100);
            //         }
            //         printf("\n");
            //         hHsize++;
            //         return 0;
            //     }
            // }
            // return -1;
            return 0;
        }
    }
}

// resizes by doubling the size of the segment
table** resize(table*** arr){
    y*=2;
    load=0;
    table** temp=initializeTable();
    for(int i=0;i<noofSegments;i++){
        loadfactor[i]=0;
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
                printf("%d:%s  ",*((int*)arr[i][j].key), (char*)arr[i][j].val);
            else    
                printf(" -:-  ");
        }
        printf("\n");
    }
    printf("\n");
}

//  deletes the entire data
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
    int failedCount=0,resizeCount=0,present=0;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for(int i=0;i<datasize;i++){
        wordarr[i]=generateStr();
        keyarr[i]=generateInt();
        int in=add(&arr, &keyarr[i], wordarr[i]);
        if(in==0){  
            resizeCount++;
            arr=resize(&arr);
            add(&arr,&keyarr[i],wordarr[i]);
        }
        if(in==-1)
            failedCount++;
        if(in==-2){
            present++;
        }
    }

    gettimeofday(&end, NULL);
    printf("execution time is : %ld microseconds\n", (end.tv_sec-start.tv_sec)*1000000L+(end.tv_usec-start.tv_usec));
   

    int failedToFetch=0;
    for(int i=0;i<datasize;i++){
        if(!contains(arr, &keyarr[i]))
            failedToFetch++;
    }
    
    printf("Segment size : %d\nNo of resizes(failed) : %d\n",segmentSize,resizeCount);
    printf("No of failed : %d\n",failedCount);
    printf("Elements failed to fetch : %d\n",failedToFetch);
    printf("Duplicate elements : %d\n",present);
    printf("No of resizes due to H size overflow : %d\n",hHsize);
    printf("No of resizes due to segment size overflow : %d\n", segsize);
   
    // for(int i=0;i<100;i++){
    //     struct timeval start, end;
    //     gettimeofday(&start, NULL);
    //     if(contains(arr, generateInt()));
    //     gettimeofday(&end, NULL);
    //     printf("execution time is : %ld microseconds\n", (end.tv_sec-start.tv_sec)*1000000L+(end.tv_usec-start.tv_usec));
    // }


    // int notpresent=0, check=0;
    // for(int i=0;i<100000;i++){
    //     int key=generateInt();
    //     if(!contains(arr, key)){
    //         notpresent++;
    //         int flag=0;
    //         for(int j=0;j<datasize;j++){
    //             if(keyarr[j]==key)
    //                 flag=1;
    //         }
    //         if(flag==0)
    //             check++;
    //     }
    // }
    // printf("No of elements not present in the array : %d\nNo of elements not present in the tabel : %d\n", check, notpresent);

    free(wordarr);
    free(keyarr);
    deleteall(arr);
    deleteTable(&arr);
    
}