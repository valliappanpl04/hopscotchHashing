#include<stdio.h>
#define tableno 2
#define tablesize 10000
#define datasize 10000
int hash1(int key){
    return (key*4)%13572;
}
int hash2(int key){
    return (key*3)%12647;
}

void add(int arr[tableno][tablesize], int flag[tableno][tablesize], int table, int cnt,int key){
    int h;
    if(cnt==datasize){
        printf("Cannot insert\n");
        return;
    }
    if(table==0)
        h=hash1(key)%tablesize;
    else    
        h=hash2(key)%tablesize;
    if((flag[0][h]==1 && arr[0][h]==key)||(flag[1][h]==1 && arr[1][h]==key)){
        printf("Already present \n");
        return;
    }
    if(flag[table][h]==0){
        arr[table][h]=key;
        flag[table][h]=1;
        // printtable(arr, flag);
    }
    else{
        int val=arr[table][h];
        arr[table][h]=key;
        // printtable(arr, flag);
        add(arr, flag, (table+1)%tableno, cnt+1, val);
    }
}
void printtable(int arr[tableno][tablesize], int flag[tableno][tablesize]){
    for(int i=0;i<tableno;i++){
        for(int j=0;j<tablesize;j++){
            if(flag[i][j]==0)
                printf(" - ");
            else    
                printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
int main(){
    int arr[tableno][tablesize],flag[tableno][tablesize];
    memset(flag, 0, sizeof(flag));
    for(int i=0;i<datasize;i++){
        add(arr, flag, 0, 0, i);
    }
}