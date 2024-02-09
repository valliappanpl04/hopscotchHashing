#include<stdio.h>
#define H 3
#define noOfSegments 3
#define segmentSize 5
typedef struct hashtable{
    int key;
    int flag;
}table;

int hashfunction(int key){
    // return key%segmentSize;
    return key;
}

int add(table arr[noOfSegments][segmentSize], int val){
    int hash=hashfunction(val);
    int segment = hash%noOfSegments;
    int bucket = hash%segmentSize;
    int hop=0,i,j=segment;
    for(i=0;i<segmentSize*noOfSegments;i++){
        if(i==segmentSize){
            j++;
        }
        int index=(bucket+i)%segmentSize;
        if(arr[j%noOfSegments][index].flag==0)
            break;
        hop++;
    }
    if(i==segmentSize*noOfSegments)
        return 0;
    int index=(segment*segmentSize)+(hop+bucket);
    while(1){
        int seg=index/segmentSize;
        if((segment*segmentSize)+(bucket+H-1)-index>=0){
            arr[seg][index%segmentSize].key=val;
            arr[seg][index%segmentSize].flag=1;
            return 1;
        }
        int step=1;
        while(step<H){
            int xindex=index-H+step;
            int xhash=hashfunction(arr[xindex/segmentSize][xindex%segmentSize].key);
            int xseg=xhash%noOfSegments;
            int xbuck=xhash%segmentSize;
            int xpos=(xseg*segmentSize)+xbuck;
            if(xpos+H-1-xindex>=0){
                int b=
                arr[segment][index%segmentSize].key=arr[segment][xindex].key;
                arr[segment][index%segmentSize].flag=1;
                arr[segment][xindex].flag=0;
                index=xindex;
                break;
            }
            step++;
        }
        if(step==H)
            return 0;
    }
}
void print(table arr[noOfSegments][segmentSize]){
    for(int i=0;i<noOfSegments;i++){
        for(int j=0;j<segmentSize;j++){
            if(arr[i][j].flag==1)
            printf("%d ",arr[i][j].key);
            else    
            printf(" - ");
        }
        printf("\n");
    }
    printf("\n\n");
}
int main(){
    table arr[noOfSegments][segmentSize];
    for(int i=0;i<noOfSegments;i++)
        for(int j=0;j<segmentSize;j++)
            arr[i][j].flag=0;
    for(int i=1;i<100;i++){
        add(arr, i);
        print(arr);
    }
}