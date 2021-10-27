#include<stdlib.h>
#include<stdio.h>
#define MAX 100000
/*
Algoritam se sastoji od toga da se slaze od nazad, tokom niza ce se ici i na kraj ce isplivati najveci element
Ovo sa pokazivacima tj sa listom moze malko drugacije... ne mora **  moze i jedna jer svakako saljemo adresu head
a posto imamo ** sa *head dobijamo isto i kada bi bilo *head i samo head ako me kapirate :D


Po mome misljenju brzi je niz jer mu se lakse pristupa tjj index based pristup dok kod liste nije tako mora se iterirati kroz celu ,
takodje niz je data structure, dok je lista abstract data structure drugacije alociranje memorije, pokazivaci ma guglaj na netu necu ja sve da ti kazem  :D
*/
typedef struct Sort{
    int number;
    struct  Sort * next;
    
}List;
void initList(List **head){
    *head=NULL;
}
void addToList(List** head, int number){
    List* newElement=(List*)malloc(sizeof(List));
    newElement->number=number;
    newElement->next=NULL;
    if(*head==NULL){
        *head=newElement;
        return;
    }
    List* pom=*head;
    while(pom->next!=NULL){
        pom=pom->next;
    }
    pom->next=newElement;
}
void swapItems(int* a, int* b){
    int temp;
    temp=*a;
    *a=*b;
    *b=temp;
}
void SortirajNiz(int* niz){
    int n=MAX;
    for (int i = 0; i < n-1; i++)
    {
        for (int j = 0; j < n-i-1; j++)
        {
            if(niz[j]>niz[j+1])
            swapItems(&niz[j],&niz[j+1]);
        }
        
    }
    
}
void swapDataNotNodes(List* first, List* second){
    int temp=first->number;
    first->number=second->number;
    second->number=temp;
}
void SortirajListu(List **head){
    //sortiranje liste nije zavrseno
    int n=MAX;
    List* temp=NULL;
    List* pom= *head;
    int swaped;
    do{
        swaped=0;
        pom=*head;
        while(pom->next!=temp){
            if(pom->number>pom->next->number){
                swaped=1;
                swapDataNotNodes(pom,pom->next);
            }
            pom=pom->next;

        }
        temp=pom;
    }while(swaped);

}
void pirntList(List* head){
    while(head!=NULL){
        printf("\n\t Element je : %d ",head->number);
        head=head->next;
    }
}
int main(){
    List* head;
    initList(&head);
    int niz[MAX];
    for (int i = 0; i < 100; i++)
    {
        addToList(&head,100-i);
        niz[i]=MAX-i;
    }
    //SortirajNiz(niz);
    SortirajListu(&head);
    pirntList(head);
    return 0;
}