#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include<conio.h>


HANDLE Semaphore;
int counter = 0  ;

DWORD WINAPI count(LPVOID pharam){

    while(1){

        WaitForSingleObject(Semaphore,INFINITE);

        counter++;
        printf("Counter : %d",counter);

    }



}





void main(){
    DWORD c;
    HANDLE h1;

    Semaphore =  CreateSemaphore(0,0,1,NULL);
    h1 = CreateThread(NULL,0,&count,NULL,0,&c);

    while(1){

        if(kbhit()){
            if(getch()=='q')
                break;

            ReleaseSemaphore(Semaphore,1,NULL);
        }
        Sleep(100);
    }

    return;
}