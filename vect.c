#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
int size=0;
__uint8_t filter[101];

int hash1(int x){   
    // x*=x;
    // int len=log10(x);
    // printf("-%d-",len);
    // return 25;
    // printf("-%d-",x);
    return x%13;
}
int hash2(int x){
    return x%37;
}
int hash3(int x){
    return x%41;
}
void** add(void **v, void *x){
    // for(int i=0;i<101;i++)
    //     printf("%d",filter[i]);
    // printf("\n");
    if(filter[hash1(x)%101]!=1 || filter[hash2(x)%101]!=1 || filter[hash3(x)%101]!=1){
        void **arr=realloc(v,sizeof(void*)*(size+1));
        arr[size++]=x;
        filter[hash1(x)%101]=1;
        filter[hash2(x)%101]=1;
        filter[hash3(x)%101]=1;
        printf("added\n");
        return arr;
    }
    else{
        printf("!! Already present\n");
        return v;
    }
}
int main(){
    void **v=malloc(sizeof(void*));
    memset(filter,0, sizeof(filter));
    // add(v,"hello");
    // add(v,10);
    // add(v,"world");
    for(int i=110;i<150;i++)
        add(v,i);
    for(int i=3;i<13;i++)
        printf("%d ",v[i]);
    // printf("%d",v[25]);
    printf("\n");
}