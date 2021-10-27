#include<stdlib.h>
#include<stdio.h>

typedef struct Queue{
    int start;
    int current;
    int* array;
}Red;
Red* Create(int size){
    Red* init=(Red*)malloc(sizeof(Red));
    init->start=0;
    init->current=0;
    init->array=(int*)malloc(sizeof(int)*size);
    return init;
}
void Enqueue(int number,Red* red){
    red->array[red->current++]=number;
}
int Dequeue(Red* red){
        return red->array[red->start++];
}
void printQueue(Red *red){
    for (int i = red->start; i <red->current; i++)
    {
       printf("\n\t%d is element : %d \n\n ",i,red->array[i]);
    }
    
}
void main(){
    Red* red=Create(100);
    Enqueue(13,red);
    Enqueue(10,red);
    Enqueue(7,red);
    Enqueue(19,red);
    Dequeue(red);
    printQueue(red);


    return;
}