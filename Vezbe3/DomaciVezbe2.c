#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#define RING_SIZE 13

#pragma region Struct,Methods

typedef struct Box{
    unsigned int head;
    unsigned int tail;
    char data[RING_SIZE];
}Ring;
char getRingElement(Ring *item){
        int index = 0;
        index = item->head;
        item->head = (item->head + 1) %RING_SIZE;
        return item->data[index];
}
void addElementToRing(Ring *item,char data){
        item->data[item->tail] =data;
        item->tail = (item->tail + 1)%RING_SIZE;    
}
char converElement(char item){
    return (item - 0x20);
}

#pragma endregion

#pragma region GlobalVariables
Ring ringIn;
Ring ringOut;
HANDLE semaphoreFull;
HANDLE semaphoreEmpty;
HANDLE semaphoreFinish;
HANDLE semaphoreFullforOut;
HANDLE semaphoreEmptyforOut;
CRITICAL_SECTION cs;
#pragma endregion
#pragma region FunkcijaUnutrasnjeNiti
DWORD WINAPI InnerThreadWork(LPVOID params){

    char c ;
    //Dodavanje semafora koji su nam potrebni 
    HANDLE semaphores[2] = {semaphoreFinish,semaphoreEmpty};
    //E sada  posto wait bilo koji vraca semafor u stanje manje tj smanjuje brojac jer racuna da se izvrsilo nesto
    //sa relase ga uvecavamo sto mu vraca stanje tj vrti se nekako, izlazimo iz While samo ako se gasi program tj q
    //jer waitobject ako se desilo za semaphore finish vraca WAIT_OBJECT_0  a ako je 2 semafor u pitanju isto samo +1
    //WAIT_OBJECT COUNTsEMAPHORES - 1 znaci ako je 1 onda je 0, ako je drugi onda 1  :D moje zapazanje 
    while(WaitForMultipleObjects(2,semaphores,FALSE,INFINITE) == WAIT_OBJECT_0 + 1){
            printf("\n Unesite neki karakter da se upise u ulazni buffer \n");

            c = getch();
            if( c == 'q' || c == 'Q')
                ReleaseSemaphore(semaphoreFinish,3,NULL);
                //ovde oslobadjamo semafor da mozemo da izadjemo iz svih niti jer one cekaju beskonacno da se program odigra

                //ulazimo u kriticnu sekciju pozivamo funkciju  u kojoj se proverava da li moze da se doda element, dodajemo i povecavamo semafor za drugu nit koja 
                //ceka da se obradi :D
            EnterCriticalSection(&cs);
            addElementToRing(&ringIn,c);
            LeaveCriticalSection(&cs);

            ReleaseSemaphore(semaphoreFull,1,NULL);
    }
}
#pragma endregion
#pragma region WOrkerThread
DWORD WINAPI workingThreadFunction(LPVOID params){
    char c;
    //promenljiva samo da ubacujem karakter
    //init semafora potrebnih za ovu nit
    HANDLE semaphores[2] = {semaphoreFinish,semaphoreFull};
    //ista radnja kao i u prosloj prici tj funkciji za ulaznu nit pogledati tamo 
    while(WaitForMultipleObjects(2,semaphores,FALSE,INFINITE) == WAIT_OBJECT_0 +1){
            //upadamo u kriicnu sekciju jer moramo pristupiti deljenoj promenljivoj :D
            //uzimamo karakter i menjamo ga u veliko slovo
            EnterCriticalSection(&cs);
            c = getRingElement(&ringIn);
     
            c = converElement(c);
      
            addElementToRing(&ringOut,c);
            LeaveCriticalSection(&cs);
            //obavestavamo ulaznu nit da smo obradili jedan elemnt i ubacili promenjeni element u izlaznu
            ReleaseSemaphore(semaphoreEmpty,1,NULL);
            ReleaseSemaphore(semaphoreFullforOut,1,NULL);
    }   




}
#pragma endregion
#pragma region outThreadFunction
DWORD WINAPI outThreadFunction(LPVOID pharams){

    //deklaracija promenljive
    char c;
    //Init semafora za ovu fju
    HANDLE semaphores[2] = {semaphoreFinish , semaphoreFullforOut};
    //ista logika kao za prethodne 2 funkcije
    while(WaitForMultipleObjects(2,semaphores,FALSE,INFINITE) == WAIT_OBJECT_0 +1 ){
        EnterCriticalSection(&cs);
        c = getRingElement(&ringOut);
        printf("\tElement incrised size :  %c \n",c);
        LeaveCriticalSection(&cs);
    }
}
#pragma endregion



void main(){
    InitializeCriticalSection(&cs);
    semaphoreEmpty = CreateSemaphore(0,RING_SIZE,RING_SIZE,NULL);
    semaphoreFinish = CreateSemaphore(0,0,3,NULL);
    semaphoreFull =CreateSemaphore(0,0,RING_SIZE,NULL);
    semaphoreFullforOut = CreateSemaphore(0,0,RING_SIZE,NULL);
    semaphoreEmptyforOut = CreateSemaphore(0,RING_SIZE,RING_SIZE,NULL);

    HANDLE inThread,workerThread,outThread;
    DWORD innThread,wThread,oThread;


    //Ako su semafori dobro napravljeni nastavljamo dalje sa kreiranjem;
    if(semaphoreEmptyforOut && semaphoreFinish && semaphoreFull && semaphoreFullforOut && semaphoreEmpty){

        inThread = CreateThread(NULL,0,&InnerThreadWork,(LPVOID)0,0,&innThread);
        workerThread = CreateThread(NULL,0,&workingThreadFunction,(LPVOID)0,0,&wThread);
        outThread = CreateThread(NULL,0,&outThreadFunction,(LPVOID)0,0,&oThread);
        if(inThread && workerThread && outThread){

            WaitForSingleObject(inThread,INFINITE);
            WaitForSingleObject(workerThread,INFINITE);
            WaitForSingleObject(outThread,INFINITE);
        }

    }
    DeleteCriticalSection(&cs);
    CloseHandle(inThread);
    CloseHandle(semaphoreEmpty);
    CloseHandle(workerThread);
    CloseHandle(semaphoreFull);
    CloseHandle(outThread);
    CloseHandle(semaphoreFullforOut);
    return;
}