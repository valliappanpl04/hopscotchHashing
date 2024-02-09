#include<stdio.h>
#include<stdlib.h>

#define tablesize 10000
#define inputdatasize 10000

struct node{
    int data;
    struct node *next;
};

int hash(int num){
    return num%tablesize;
}

void add(struct node* arr[], int i){
    if(search(arr,i)){
        printf("%d already present\n", i);
        return;
    }
    int h=hash(i);
    struct node* ptr=(struct node*)malloc(sizeof(struct node));
    ptr->data=i;
    ptr->next=NULL;
    if(arr[h]==NULL){
        arr[h]=ptr;
    }
    else{
        struct node* temp=arr[h];
        while(temp->next!=NULL){
            temp=temp->next;
        }
        temp->next=ptr;
    }
}

int search(struct node *arr[], int i){
    int h=hash(i);
    struct node* temp = arr[h];
    if(temp==NULL)
        return 0;
    while(temp->next!=NULL){
        if(temp->data==i){
            // printf("Found\n");
            return 1;
        }
        temp=temp->next;
    }
    // printf("Not found\n");
    return 0;
}

int main(){
    struct node* arr[tablesize];
    printf("Hi");
    memset(arr, NULL, sizeof(arr));
    for(int i=0;i<inputdatasize;i++){
        add(arr, i);
    }
    // add(arr, 1);
    for(int i=0;i<inputdatasize;i++){
        search(arr,i);
    }
    // if(search(arr, 1076561))
    //     printf("Found");
    // else    
    //     printf("Not found");
}