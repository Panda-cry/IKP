#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#define MAX 1000
//koliko sam skontao imamo niz gde popunjavamo sve i niz gde se popunjavaju samo float vrednosti  !!!
//racuna dobro ali nesto sa satovima nije kako treba
typedef struct BigStructre{
    char data1[500];
    float data2;
    char data3[500];
}Data;


int main(){
    srand(time(0));

    Data first[MAX];
    Data second[MAX];
  
    for (int i = 0; i < MAX; i++)
    {
        
        first[i].data2=rand()%10000+1;
        second[i].data2=rand()%10000+1;
        for (int j = 0; j < 500; j++)
         {
        first[i].data1[j]=rand()%27+65;
        first[i].data3[j]=rand()%27+65;
         }
    }
 
   
    
    clock_t time;
    time=clock();
    float sum=0;
    for (int i = 0; i < MAX; i++)
    {
       sum+=first[i].data2;
    }  sleep(1);
    time=clock()-time;
  
    float seconds = (float)(time) / CLOCKS_PER_SEC;
    printf("\n\tIzracunato za  : %f \n",sum);

    time=clock();
    float sum1=0;
    for (int i = 0; i < MAX; i++)
    {
       sum1+=second[i].data2;
    }
    time=clock()-time;
    seconds = (float)(time) / CLOCKS_PER_SEC;
    printf("\n\tIzracunato za  : %f \n",sum1);


    return 0;
}