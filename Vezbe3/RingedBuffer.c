#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include<conio.h>

#define RING_SIZE 6
#define SLEEPING_TIME 300


HANDLE Empty;
HANDLE Full;
HANDLE FinishSignal;
CRITICAL_SECTION cs;

typedef struct Box{
    unsigned int tail;
    unsigned int head;
    char data[RING_SIZE];
}RingBuffer;

RingBuffer ring ;
char ringBufferGetChar(RingBuffer *select){
        int index;
        index = select->head;
        select->head = (select->head + 1) % RING_SIZE;
        return select->data[index];
}

void ringBuffedSetChar(RingBuffer *select,const char data){
    select->data[select->tail]=data;
    select->tail = (select->tail + 1)%RING_SIZE;
}

DWORD WINAPI producer(LPVOID param){

    char c;
    const int semaphore = 2;
    HANDLE ss[2] = {FinishSignal,Empty};
    
    while(WaitForMultipleObjects(2,ss,FALSE,INFINITE)==WAIT_OBJECT_0 + 1){
        printf("\n\t Unesite neki karakter \t\n\n");
            c = getch();
            if(c == 'q' || c == 'Q' ){
                ReleaseSemaphore(FinishSignal,2,NULL);
            }

            EnterCriticalSection(&cs);

            ringBuffedSetChar(&ring,c);

            LeaveCriticalSection(&cs);

            ReleaseSemaphore(Full,1,NULL);
    }
    printf("Izasao sam iz while");
    return 0;
}

DWORD WINAPI costumer(LPVOID param){
        HANDLE semaphores[2] = {FinishSignal,Full};

        while(WaitForMultipleObjects(2,semaphores,FALSE,INFINITE) == WAIT_OBJECT_0 + 1){
        
            EnterCriticalSection(&cs);

            char c= ringBufferGetChar(&ring);
            printf("\n\t Slovo preuzeto sa kruznog bafera  %c \t\n",c);

            LeaveCriticalSection(&cs);

            ReleaseSemaphore(Empty,1,NULL);
            Sleep(SLEEPING_TIME);

        }

return 0;

}



void main(){

    HANDLE hpro,hcos;
    DWORD pro,cos;
    Empty = CreateSemaphore(0,RING_SIZE,RING_SIZE,NULL);
    Full = CreateSemaphore(0,0,RING_SIZE,NULL);
    FinishSignal = CreateSemaphore(0,0,2,NULL);

    if(Empty  && Full && FinishSignal){
        InitializeCriticalSection(&cs);

        hpro = CreateThread(NULL,0,&producer,(LPVOID)0,0,&pro);
        hcos = CreateThread(NULL,0,&costumer,(LPVOID)0,0,&cos);


        if(!hpro || !hcos)
            ReleaseSemaphore(FinishSignal,2,NULL);

         if(hpro)
            WaitForSingleObject(hpro,INFINITE);
        if(hcos)
            WaitForSingleObject(hcos,INFINITE);



    }

        CloseHandle(hpro);
        CloseHandle(hcos);
        CloseHandle(Empty);
        CloseHandle(Full);
        CloseHandle(FinishSignal);

        DeleteCriticalSection(&cs);
    return;
}
