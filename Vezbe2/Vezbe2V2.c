#include<stdio.h>
#include<windows.h>
CRITICAL_SECTION cs;
DWORD WINAPI print(LPVOID lpharams){

        char* c = (char*)lpharams;
        EnterCriticalSection(&cs);
        for(int i = 0; i < 100 ; i++){
            printf("%c",*c);
        }
        printf("\n");
        LeaveCriticalSection(&cs);
        return 0;


}



void main(){

    InitializeCriticalSection(&cs);
    DWORD p1,p2,p3;
    HANDLE h1,h2,h3;
    char c1 = '1';
    char c2 = '2';
    char c3 = '3';
    h1 = CreateThread(NULL,0,&print,&c1,0,&p1);
    h2 = CreateThread(NULL,0,&print,&c2,0,&p2);
    h3 = CreateThread(NULL,0,&print,&c3,0,&p3);

    int i = getchar();

    CloseHandle(h1);
    CloseHandle(h2);
    CloseHandle(h3);
    DeleteCriticalSection(&cs);
    return;
}