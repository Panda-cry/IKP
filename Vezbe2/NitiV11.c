#include<stdio.h>
#include<windows.h>


DWORD WINAPI print1(LPVOID parametar){

        for(int i = 0 ; i < 100 ; i++){
            printf("1");
        }
    return 0;
}

DWORD WINAPI print2(LPVOID parametar){

        for(int i = 0 ; i < 100 ; i++){
            printf("2");
        }
    return 0;
}

DWORD WINAPI print3(LPVOID parametar){

        for(int i = 0 ; i < 100 ; i++){
            printf("3");
        }
    return 0;
}



int main(){

    DWORD print11 , print22 ,print33;
    HANDLE h1,h2,h3;

    h1 = CreateThread(NULL,0,&print1,NULL,0,&print11);
    h2 = CreateThread(NULL,0,&print2,NULL,0,&print22);
    h3 = CreateThread(NULL,0,&print3,NULL,0,&print33);

    int i = getchar();

    CloseHandle(h1);
    CloseHandle(h2);
    CloseHandle(h3);

    return 0;
}