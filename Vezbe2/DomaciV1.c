#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>

typedef struct Box{
    char* c;
    int index;
}Dom;

CRITICAL_SECTION cs;

DWORD WINAPI print(LPVOID lpparam){

    Dom* c= (Dom*)lpparam;
    for(int i = 0 ; i<100 ; i++){

        while(*c->c != c->index-1)
            Sleep(1);

        EnterCriticalSection(&cs);
        *c->c = (c->index) % 3;
        printf("%d",c->index);

        LeaveCriticalSection(&cs);
    }



    return 0;
}









void main(){

    DWORD p1,p2,p3;
    HANDLE h1,h2,h3;

    InitializeCriticalSection(&cs);

    Dom d1,d2,d3;
    char c = 0;

    d1.c = &c;
    d1.index=1;

    d2.c = &c;
    d2.index = 2;

    d3.c = &c;
    d3.index = 3;

    h1 =  CreateThread(NULL,0,&print,&d1,0,&p1);
    h2 = CreateThread(NULL,0,&print,&d2,0,&p2);
    h3 = CreateThread(NULL,0,&print,&d3,0,&p3);

    int i = getchar();

    CloseHandle(h1);
    CloseHandle(h2);
    CloseHandle(h3);

    DeleteCriticalSection(&cs);





    return;
}