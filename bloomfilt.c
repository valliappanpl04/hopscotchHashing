#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef enum datatype{
    d,s,c
}type;
#define FILTER_SIZE 200 //  size of the bloom filter
void **vect=NULL;
type *t=NULL;
int size=0;
__uint8_t filter[FILTER_SIZE];
int hash1(void *x, type datatype){
    switch(datatype){
        case d:
            int n=(((int)x)*((int)x))%101;
            return n;
        case s:
            int num=0;
            char *str=(char*)x;
            for(int i=0;str[i]!='\0';i++){
                num=(num*10)+str[i];
            }
            return num;
        case c:
            char ch=(char)x;
            for(int i=0;i<7;i++){
                num+=(num*8)+(int)ch;
            }
            return num;
    }
}
int hash2(void *x, type datatype){
    switch(datatype){
        case d:
            return (((int)x*31)/10)%643;
        case s:
            int num=0;
            char *str=(char*)x;
            for(int i=0;str[i]!='\0';i++){
                num=(num*23)+str[i];
            }
            return num;
        case c:
            char ch=(char)x;
            for(int i=0;i<7;i++){
                num+=(num*23)+(int)ch;
            }
            return num;
    }
}
int hash3(void *x, type datatype){
    switch(datatype){
        case d:
            return (int)x%41;
        case s:
            int num=0;
            char *str=(char*)x;
            for(int i=0;str[i]!='\0';i++){
                num=(num*17)+str[i];
            }
            return num;
        case c:
            char ch=(char)x;
            for(int i=0;i<11;i++){
                num+=(num*23)+(int)ch;
            }
            return num;
    }
}
void printelement(void *x, type datatype){
    switch(datatype){
        case d:
            printf("%d",x);
            break;
        case s:
            printf("%s",x);
            break;
        case c:
            printf("%c",x);
            break;
    }
}
void add(void *x, type datatype){
    int i1=hash1(x,datatype)%FILTER_SIZE;
    int i2=hash2(x,datatype)%FILTER_SIZE;
    int i3=hash3(x,datatype)%FILTER_SIZE;
    if(!(filter[i1]&filter[i2]&filter[i3])){
        filter[i1]=1;
        filter[i2]=1;
        filter[i3]=1;
        vect=realloc(vect,sizeof(void*)*++size);
        t=realloc(t,sizeof(type)*size);
        t[size-1]=datatype;
        vect[size-1]=x;
    }
    else{
        printf("The Element ");
        printelement(x,datatype);
        printf(" is already present\n");
    }
}

// for datatype (enum)
// d-> for integer 
// c-> for character
// s-> for string

int main(){
    memset(filter, 0, sizeof(filter));
    add("hi",s); // adding string
    // add(1,d);    // add Integer
    add("hi",s); // adding string
    add("hi",s); // Adding string
    for(int i=1;i<50;i++)
        add(i,d);//Adding integer
    // printf("\n\n\n");
}