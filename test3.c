#include<stdio.h>
#include<stdlib.h>
int size=0;
void resize(void **v){
    size++;
    void **arr=realloc(v,size*sizeof(void*));
    v=arr;
}

void add(void **v, void *data){
    // int size=sizeof(v)/sizeof(v[0]);
    // printf("size : %d",size);
    resize(v);
    v[size-1]=data;
}

int search(void **ve, void* obj){
    for(int i=0;i<size;i++){
        printf("%u %u\n",ve[i],obj);
        if(ve[i]==obj){
            return i;
        }
    }
    
    return 0;
}

int main(){
    void **v=malloc(sizeof(void*));
    char str[]="hello1";
    add(v,str);
    add(v,"hello2");
    add(v,"hello3");
    add(v,"hello4");
    printf("%p\n",v);
    printf("%s\n",(char*)v[0]);
    printf("%p\n",v[1]);
    printf("%p\n",v[2]);
    add(v,4);
    // // add(v,1.4);
    // for(int i=0;i<10;i++){
    //     add(v,i);
    // }

    // add(v,"hello4");
    // char str[50]="hello3";
    // // printf("%s\n",(char*)v[3]);
    }