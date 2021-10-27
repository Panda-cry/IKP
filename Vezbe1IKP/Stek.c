#include<stdio.h>
#include<stdlib.h>

typedef struct Stek
{
   int top;
   unsigned capacity;
   int* array;
}Stek;
Stek* initStack(unsigned capacity){
    Stek* init=(Stek*)malloc(sizeof(Stek));
    init->capacity=capacity;
    init->array=(int*)malloc(sizeof(int)*capacity);
    init->top=-1;
    return init;
}
int IsFull(Stek *stek){
    return stek->capacity-stek->top;
}
void PushOnStack(int data,Stek *stek){
    if(IsFull(stek)==1)
    return;
    stek->top+=1;
    stek->array[stek->top]=data;
}
void PrintStack(Stek *stek){
    for (int i = 0; i <=stek->top; i++)
    {
       printf("\n\tElement sa dole : %d je : %d \n\n",i,stek->array[i]);
    }
    
}
int IsEmpty(Stek *stek){
    return stek->capacity-stek->top;
}
void popFromStack(Stek *stek){
    if(IsEmpty(stek)==stek->capacity)
    return;
    stek->top-=1;
}

void main(){
    Stek* stek=initStack(100);
    PushOnStack(13,stek);
    PushOnStack(10,stek);
    PushOnStack(11,stek);
    PrintStack(stek);
    popFromStack(stek);
    PrintStack(stek);
}
