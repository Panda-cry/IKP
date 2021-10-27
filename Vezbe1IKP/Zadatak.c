#include <stdio.h>
#include <stdlib.h>
#include<string.h>


float* Calculate(char* buffer){
    char operation=buffer[0];
    float* resenje=(float*)malloc(sizeof(float));

    int first = *((int*)(buffer+1));
    int second = *((int*)(buffer+5));

 
    switch(operation){
    case 0:
        *resenje=first+second;
        break;
    case 1:
    *resenje=first-second;
        break;
    case 2 :
    *resenje=first * (second);
        break;
    case  3 :
    *resenje=first*1.0/(second);
        break;
    default:
        break;
    }
    printf("%f",*resenje);
    return resenje;
}
int main()
{
    char arry[9];
    char operation=0;
    int num1=33;
    int num2=55;
    memcpy(arry,&operation,1);
    memcpy(arry+1,&num1,4);
    memcpy(arry+5,&num2,4);
    float* rezultat=Calculate(arry);
    printf("Nase resenje zadatka je  %f",*rezultat);
    return 0;
}
