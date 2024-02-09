#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

// #if INTERFACE

struct vector{
    void **elemet;
    int size;
};

struct vector* createvector(){
    struct vector *v=(struct vector*)malloc(sizeof(struct vector));
    v->size=0;
    v->elemet=malloc(sizeof(void*)*5);
}

int main(){
    vect *v=createvector();
}