#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct node{
    int data;
    char *name;
    struct node *next;
};

void printlist(struct node *a){
    while(a!=NULL){
        printf("%d %s\n",a->data,a->name);
        a=a->next;
    }
    printf("\n");
}

struct node* createnode(int val, char *name){
    struct node *ptr=(struct node*)malloc(sizeof(struct node));
    ptr->name=(char*)malloc(sizeof(char)*50);
    ptr->next=NULL;
    ptr->data=val;
    strcpy(ptr->name, name);
    return ptr;
}

void append(struct node **head, struct node **tail, int val, char *name){
    struct node *temp=createnode(val, name);
    if(*head==NULL){
        *head=*tail=temp;
    }
    else{
        struct node *t=*tail;
        t->next=temp;
        *tail=temp;
    }
}

void prepend(struct node **head, struct node **tail, int val, char *name){
    struct node *temp=createnode(val, name);
    if(*head==NULL)
        *head=*tail=temp;
    else{
        temp->next=*head;
        *head=temp;
    }
}

void deletefront(struct node **head){
    struct node *t=*head;
    if(t==NULL){
        printf("No Element to delete\n");
        return;
    }
    else{
        *head=t->next;
    }
    free(t);
}

void deletelast(struct node **head, struct node **tail){
    struct node *t=*tail;
    if(*head==NULL){
        printf("No element to delete\n");
        return;
    }
    else if(*head==*tail){
        *head=NULL;
        *tail=NULL;
    }
    else{
        struct node *temp=*head;
        while(temp->next!=*tail){
            temp=temp->next;
        }
        temp->next=NULL;
        *tail=temp;   
    }
    free(t);
}

void deleteAtPos(struct node **head, struct node **tail, int pos){
    struct node *h=*head;
    struct node *t=*tail;
    if(pos==1){
        struct node *temp=h;
        h=h->next;
        *head=h;
        free(temp);
    }
    else{
        int cnt=2;
        while(cnt<pos){
            h=h->next;
            cnt++;
        }
        struct node *ptr=h->next;
        h->next=h->next->next;
        if(ptr->next==NULL){
            *tail=h;
        }
        struct node *h=*tail;
        free(ptr);
    }
}
// struct node* joinlist(struct node **h1, struct node **t1, struct node **h2, struct node **t2){
//     struct node *temp=*t1;
//     struct node *temp2=*h2;
//     temp->next=temp2;
//     return *h1;
// }
struct x{
    int a,b,c;
    int arr[20];
    int *x;
};
int main(){
    struct node *h1=NULL,*t1=NULL;
    // struct node *h2=NULL, *t2=NULL;
    append(&h1,&t1,1,"Hello");
    append(&h1,&t1,2,"World");
    append(&h1,&t1,3,"Hi");
    append(&h1,&t1,4,"Welcome");
    deleteAtPos(&h1,&t1,2);
    prepend(&h1,&t1,5,"morning");
    prepend(&h1,&t1,6,"Good");
    deleteAtPos(&h1,&t1,4);
    printlist(h1);
    // printf("Sizeof structure is : %d",sizeof(struct node));
    printf("Size of the structure is : %d",sizeof(*h1));
    printf("\nSize of int is : %d", sizeof(h1->data));
    printf("\nSize of char pointer is : %d", sizeof(h1->name));
    printf("\nSize of structure pointer is : %d\n\n", sizeof(h1->next));
    printf("size of : %d\n\n",sizeof(struct x));
}