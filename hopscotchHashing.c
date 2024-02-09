#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//  Setting the constant H as 4
#define H 1
#define tablesize 10000
#define datasize 10000

typedef struct hashtable{
    int key, flag;
}table;

int hash(int key){
    return (key*13)%20000%tablesize;
}


int add(table arr[], int val){
    if(contains(val, arr))
        return 0;
    int hashval=hash(val),hop=0,i;
    for(i=0;i<tablesize;i++){
        int in=(hashval+i)%tablesize;
        if(arr[in].flag==0){
            break;
        }
        hop++;
    }
    if(i==tablesize){
        printf("Element can't be inserted\n");
        return 0;
    }
    int index=(hop+hashval);
    
    while(1){
        if((hashval+H-1)-index>=0){
            arr[index%tablesize].key=val;
            arr[index%tablesize].flag=1;
            return 1;
        } 
        int step=1,in=index;    
        while(step<H){
            in=(index-H+step+tablesize)%tablesize;
            int xhash=hash(arr[in].key);
            if((xhash+H-1)-index>=0){
                arr[in].flag=0;
                arr[index].flag=1;
                arr[index].key=arr[in].key;
                index=in;
                break;
            }  
            step++;
        }
        if(step==H){
            printf("Element can't be inserted\n");
            return 0;
        }
    }
}
int contains(int key, table arr[]){
    int hashval=hash(key);
    for(int i=0;i<H;i++){
        if(arr[(hashval+i)%tablesize].flag==1 && arr[(hashval+i)%tablesize].key==key)
            return 1;
    }
    return 0;
}
void print(table arr[]){
    // Printing the element at each position
    for(int i=0;i<tablesize;i++){
        printf("val @ %d : ", i);
        if(arr[i].flag!=0)
            printf("%d",arr[i].key);
        else
            printf("-");
        printf("\n");
    }
    printf("\n");
}

void delete(table arr[], int key){
    if(!contains(key, arr)){
        printf("Element not present\n");
        return;
    }
    int hashval=hash(key);
    for(int i=0;i<H;i++){
        int index=(i+hashval)%tablesize;
        if(arr[index].flag==1 && arr[index].key==key){
            arr[index].flag=0;
            arr[index].key=0;
        }
    }
}

int main(){
    table arr[tablesize];

    int cnt=0;
    int failed[tablesize];


    for(int i=0;i<tablesize;i++){
        arr[i].flag=0;
    }

    // Adding elements to the hashtable
    for(int i=0;i<datasize;i++){
        if(!add(arr, i)){
            failed[cnt++]=i;
        }
    }
    //Printing array positions
    print(arr);

    // deleting every third element 
    for(int i=9000;i<9900;i+=3){
        delete(arr, i);
    }
    //Printing array positions
    print(arr);


    //Printing elements that are failed to add in the hashtable
    printf("\nNo of failed adds : %d\nElements failed to add are : ",cnt);
    for(int i=0;i<cnt;i++){
        printf("%d ",failed[i]);
    }
    printf("\n\n");
    
    int check[]={5760, 9200, 9210, 12560};
    int size=sizeof(check)/sizeof(check[0]);
    // for(int i=0;i<size;i++){
    //     if(contains(check[i],arr))
    //         printf("%d is present\n",check[i]);
    //     else
    //         printf("%d is not present\n",check[i]);
    // }
    int x=0;
    for(int i=0;i<datasize;i++){
        if(contains(i, arr))
            x++;
    }
    printf("No of Elements : %d",x);
}