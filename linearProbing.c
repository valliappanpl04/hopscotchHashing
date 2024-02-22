#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<string.h>
#define tablesize 50000
#define datasize 50000
#define upperLimit 100000000
#define lowerLimit 1000

// structue for defining the bucket in hashtable
typedef struct hashtable{
    int flag;
    void *key;
    void *val;
} table;

// returns the hashvalue of the given key
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

// generates a random integer within upper and lower limit
int generateInt(){
    return (rand()%(upperLimit-lowerLimit))+lowerLimit;
}

// generates a random string 
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

// Initializes a table of size "tablesize" and returns the table
table* initializeTable(){
    table* arr=(table*)malloc(sizeof(table)*tablesize);
    for(int i=0;i<tablesize;i++)
        arr[i].flag=0;
    return arr;
}

// checks whether a key is found in the table
int contains(table* arr, void* key){
    unsigned long long int hash=hashfunction(key, sizeof(int));
    int buck=hash%tablesize;
    for(int i=0;i<tablesize;i++){
        int index=(buck+i)%tablesize;
        if(arr[index].flag==1 && *(int*)arr[index].key==*(int*)key)
            return 1;
    }
    return 0;
}

// Adds a new entry into the table
int add(table** arr, void* key, void* val){
    if(contains(*arr, key))
        return -1;
    unsigned long long int hash=hashfunction(key, sizeof(int));
    int buck=hash%tablesize;
    for(int i=0;i<tablesize;i++){
        int index=(buck+i)%tablesize;
        if((*arr)[index].flag==0){
            (*arr)[index].flag=1;
            (*arr)[index].key=key;
            (*arr)[index].val=val;
            return 1;
        }
    }
    return 0;
}

// deletes a entry with the given key
int deleteentry(table** arr, void *key){
    unsigned long long int hash=hashfunction(key, sizeof(int));
    int buck=hash%tablesize;
    for(int i=0;i<tablesize;i++){
        int index=(buck+i)%tablesize;
        if((*arr)[index].flag==1 && *(int*)(*arr)[index].key==*(int*)key){
            (*arr)[index].flag=0;
            free((*arr)[index].key);
            free((*arr)[index].val);
            (*arr)[index].val=NULL;
            (*arr)[index].key=NULL;
            return 1;
        }
    }
    return 0;
}

int main(){
    table* arr=initializeTable();
    int *keyarr=(int*)malloc(sizeof(int)*datasize);
    char **wordarr=(char**)malloc(sizeof(char*)*datasize);
    int success=0, duplicate=0, missing=0, found=0;


    // get the starting time
    struct timeval start, end;
    gettimeofday(&start, NULL);


    // add all the entries into the table
    for(int i=0;i<datasize;i++){
        keyarr[i]=generateInt();
        wordarr[i]=generateStr();
        int res=add(&arr, &keyarr[i], wordarr[i]);
        if(res==1)
            success++;
        else if (res==-1)
            duplicate++;
    }

    // get the ending time
    gettimeofday(&end, NULL);

    // check whether all the entries are present in the table
    for(int i=0;i<datasize;i++){
        if(!contains(arr, &keyarr[i]))
            missing++;
    }

    // checks whether all the entris are form the input data set
    for(int i=0;i<tablesize;i++){
        if(arr[i].flag==1){
            for(int j=0;j<datasize;j++){
                if(*(int*)arr[i].key==keyarr[j]){
                    found++;
                    break;
                }
            }
        }
    }

    
    printf("Number of elements successfully added is : %d\n", success);
    printf("Number of duplicate elements are : %d\n", duplicate);
    printf("Number of elements missing in the table is : %d\n", missing);
    printf("Number of elements found in the input data set : %d\n", found);
    printf("Executiong time is : %ld microseconds\n", (end.tv_sec-start.tv_sec)*1000000L+(end.tv_usec-start.tv_usec));
}