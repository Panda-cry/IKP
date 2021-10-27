#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct List
{
    char* arry;
    struct List* next;
}Pool;
void initList(Pool** head){
    *head=NULL;
}
void Add(Pool **head){
        if(*head==NULL){
            Pool* newEl=(Pool*)malloc(sizeof(Pool));
            newEl->arry=(char*)malloc(sizeof(char)*1024);
            newEl->next=NULL;
            *head=newEl;
            return;
        }
        Pool* newEl=(Pool*)malloc(sizeof(Pool));
        newEl->arry=(char*)malloc(sizeof(char)*1024);
        newEl->next=NULL;
        Pool*pom=*head;
        while(pom->next!=NULL){
            pom=pom->next;
        }
        pom->next=newEl;
}
char* GetNewBuffer(Pool *head){
    while(head!=NULL){
        if(head->arry==NULL){
            
            head=head->next;
        }else{
            char*pom=head->arry;
            head->arry=NULL;
            return pom;
        }
    }

}
void ReturnBuffer(char *buffer, Pool *head){
        memcpy(buffer,"Petar",6);
        while(head!=NULL){
        if(head->arry==NULL){
            head->arry=buffer;
        }else{
            head=head->next;
        }
    }

}
void PrintBuffers(Pool *head){
    int i=0;
    while(head!=NULL){
        printf("\n\t %s \n",head->arry);
        head=head->next;
        if(i>3){
            break;
        }
        i++;
    }
    
}
void main(){
    Pool* head;
    initList(&head);
    for (int i  = 0; i < 128; i++)
    {
        Add(&head);
    }
    
    char* pom=GetNewBuffer(head);
    char* pom1=GetNewBuffer(head);
     char* pom2=GetNewBuffer(head);
    ReturnBuffer(pom,head);
    ReturnBuffer(pom1,head);
     ReturnBuffer(pom2,head);
    PrintBuffers(head);


    return;
}