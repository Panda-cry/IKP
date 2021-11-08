#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

HANDLE semaphores[3];


DWORD WINAPI print(LPVOID param){
    int n = (int) param;
    for(int i = 0; i < 100; i++){

        WaitForSingleObject(semaphores[n],INFINITE);

        printf("Thread : %d ", n );
        ReleaseSemaphore(semaphores[(n+1)%3],1,NULL);
    }



    return 0;
}





void main(){
    DWORD p1,p2,p3;
    HANDLE h1,h2,h3;
    semaphores[0] =CreateSemaphore(0,1,1,NULL);
     semaphores[1] =CreateSemaphore(0,0,1,NULL);
      semaphores[2] =CreateSemaphore(0,0,1,NULL);


    h1 = CreateThread(NULL,0,&print,(LPVOID)0,0,&p1);
        h2 = CreateThread(NULL,0,&print,(LPVOID)1,0,&p2);
            h3 = CreateThread(NULL,0,&print,(LPVOID)2,0,&p3);
    int i = getch();


    CloseHandle(h1);
        CloseHandle(h2);
            CloseHandle(h3);
                CloseHandle(semaphores[0]);
                    CloseHandle(semaphores[1]);
                        CloseHandle(semaphores[2]);

    return;
}
