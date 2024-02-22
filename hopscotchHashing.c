#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
//  Setting the constant H as 4
#define H 32
#define tablesize x
#define datasize y
#define upperLimit 100000000
#define lowerLimit 1000
// int x=5000000, y=5000000;
int x=5000000, y=5000000;

typedef struct hashtable{
    void *val;
    void *key; 
    int flag;
    int hopinfo;
}table;

table* initializeTable(){
    table* arr=(table*)malloc(sizeof(table)*tablesize);
    int as=tablesize;
    for(int i=0;i<tablesize;i++){
        arr[i].hopinfo=0;
        arr[i].flag=0;
    }
    return arr;
}

int generateInt(){
    return (rand()%(upperLimit-lowerLimit))+lowerLimit;
}

char* generateStr(){
    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    int stringLen = strlen(string);        
    char *randomString = malloc(sizeof(char) * (11));
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

__uint64_t hashfunction(void* key , int len){
    int keyval=*(int*)key;
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

int contains(table* arr, void *key){
    int k=*(int*)key;
    int hashval=hashfunction(key, sizeof(int))%tablesize;
    int hopinfo=arr[hashval].hopinfo;
    int size=tablesize;
    for(int i=0;i<H;i++){
        if((hopinfo>>i)&1==1){
            if(*(int*)arr[(hashval+i)%tablesize].key==*(int*)key)
                return 1;
        }
    }
    return 0;
}

int add(table **arr, void *key, void* val){
    int k=*(int*)key;
    if(contains(*arr, key))
        return -2;
    unsigned long long int hash=hashfunction(key, sizeof(int));
    int hashval=hash%tablesize;
    int hop=0,i;
    for(i=0;i<tablesize;i++){
        int in=(hashval+i)%tablesize;
        int flag=(*arr)[in].flag;
        if((*arr)[in].flag==0){
            break;
        }
        hop++;
    }
    if(i==tablesize){
        // tell element can't be inserted and needs resize
        return 0;
    }
    int index=(hop+hashval)%tablesize;
    
    while(1){
        if(index<hashval)
            index+=tablesize;
        if((hashval+H-1)-index>=0){
            (*arr)[index%tablesize].key=key;
            (*arr)[index%tablesize].val=val;
            (*arr)[index%tablesize].flag=1;
            int h=(*arr)[hashval].hopinfo;
            (*arr)[hashval].hopinfo|=(1<<(index-hashval));
            h=(*arr)[hashval].hopinfo;
            return 1;
        }
        int step=1,in=index;    
        while(step<H){
            in=(index-H+step+tablesize)%tablesize;
            int hash1=hashfunction((*arr)[in].key, sizeof(int));
            int xhash=hash1%tablesize;
            if(index<xhash)
                index+=tablesize;
            if((xhash+H-1)-index>=0){
                (*arr)[in].flag=0;
                (*arr)[index%tablesize].flag=1;

                (*arr)[index%tablesize].key=(*arr)[in].key;
                (*arr)[index%tablesize].val=(*arr)[in].val;

                (*arr)[in].key=NULL;
                (*arr)[in].val=NULL;

                (*arr)[xhash].hopinfo&=~(1<<(in-xhash));
                (*arr)[xhash].hopinfo|=(1<<(index-xhash));

                index=in;
                break;
            }  
            step++;
        }
        if(step==H){
            // tell element can't be inserted and needs resize
            return 0;
        }
    }
}

table* resize(table** arr){
    x*=2;
    table* temp=initializeTable();
    for(int i=0;i<tablesize/2;i++){
        if((*arr)[i].flag==1){
            add(&temp, (*arr)[i].key, (*arr)[i].val);
        }
    }
    return temp;
}

void print(table* arr){
    // Printing the element at each position
    for(int i=0;i<tablesize;i++){
        if(arr[i].flag==1)
            printf("%d:%s ",*(int*)arr[i].key, (char*)arr[i].val);
        else
            printf(" -:- ");
    }
    printf("\n");
}

void delete(table** arr, void* key){
    if(!contains(*arr, key)){
        printf("Element not present\n");
        return;
    }
    int hashval=hashfunction(key, sizeof(int))%tablesize;
    for(int i=0;i<H;i++){
        int index=(i+hashval)%tablesize;
        if((*arr)[index].flag==1 && (*arr)[index].key==key){
            (*arr)[index].flag=0;
            (*arr)[index].key=NULL;
            (*arr)[index].val=NULL;
        }
    }
}

int main(){
    int failedCount=0, present=0, resizecount=0, success=0;
    table *arr=initializeTable();
    char** word=(char**)malloc(sizeof(char*)*datasize);
    int* keyarr=(int*)malloc(sizeof(int)*datasize);



    // Adding elements to the hashtable

    struct timeval start, end;
    gettimeofday(&start, NULL);
 

    for(int i=0;i<datasize;i++){
        word[i]=generateStr();
        keyarr[i]=generateInt();
        int k=keyarr[i];
        int in=add(&arr, &keyarr[i], word[i]);
        if(in==0){
            resizecount++;
            arr=resize(&arr);
            add(&arr, &keyarr[i], word[i]);
        }
        else if(in ==-1)
            failedCount++;
        else if (in ==-2)
            present++;
        else
            success++;
    }

    gettimeofday(&end, NULL);
    printf("execution time is : %ld microseconds\n", (end.tv_sec-start.tv_sec)*1000000L+(end.tv_usec-start.tv_usec));
   
    int missing=0;
    for(int i=0;i<datasize;i++){
        if(contains(arr, &keyarr[i])==0)
            missing++;
    }
    printf("Missing elements : %d\n", missing);

    printf("Elements successfully inserted : %d\n", success);
    printf("Duplicate count : %d\n", present);
    printf("No of resizes : %d\n", resizecount);
    printf("Size of table is : %d\n", tablesize);
    // for(int i=0;i<size;i++){
    //     if(contains(check[i],arr))
    //         printf("%d is present\n",check[i]);
    //     else
    //         printf("%d is not present\n",check[i]);
    // }
    // int x=0;
    // for(int i=0;i<datasize;i++){
    //     if(contains(arr, &keyarr[i]))
    //         x++;
    // }
    // printf("No of Elements : %d",x);
}