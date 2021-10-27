#include<stdlib.h>
#include<stdio.h>

typedef struct Node{
    int number;
    int index;
    struct Node* next;
}List;
void InitList(List **head){
    *head=NULL;
}
void PrintList(List *head){
    if(head==NULL){
        printf("\n\tTrenutno nema elemenata dodajte neke !!\n\n");
        return;
    }
    printf("\n\t-------------Trenutno sto se nalazi u listi-------------\n");
    
    while(head!=NULL){
        printf("\n\tIndex elementa %d , i njegova vrednost : %d \n",head->index,head->number);
        head=head->next;
    }
}
void AddToList(int element,List **head){
    List* newElement=(List*)malloc(sizeof(List));
    if(*head==NULL){
        newElement->index=0;
        newElement->number=element;
        newElement->next=NULL;
        *head=newElement;
        return;
    }
    newElement->number=element;
    List* pom=*head;
    int i=1;
    while(pom->next!=NULL){
        pom=pom->next;
        i++;
    }
    newElement->index=i;
    newElement->next=NULL;
    pom->next=newElement;
}
void InsetAtIndex(int index,int number,List** head){
    List* newEelemnt=(List*)malloc(sizeof(List));
    List* pom=*head;
    List*temp=*head;
    int id;
    
    while(pom!=NULL){
        if(pom->index==index){
            
            newEelemnt->index=index;
            newEelemnt->number=number;
            newEelemnt->next=pom;
            temp->next=newEelemnt;
            List*tt=pom;
            while(tt!=NULL){
                tt->index=tt->index+1;
                tt=tt->next;
            }
            return;
        }else{
            temp=pom;
        }
        pom=pom->next;
    }
        

}
int ElementAt(int index,List*head){
    while(head!=NULL){
        if(head->index==index){
            return head->number;
        }
    }

}
void RemoveAt(int index,List** head){
    List*pom=*head;
    List*temp=*head;
    while(pom!=NULL){
        if(pom->index==index){
            temp->next=pom->next;
            free(pom);
            pom=NULL;
            while((temp=temp->next)!=NULL){
                temp->index-=1;
               
            }
            return;
        }else{
            temp=pom;
        }
        pom=pom->next;
    }




}
void Clear(List** head){
    List*pom=*head;
    while((*head)!=NULL){
        *head=(*head)->next;
        free(pom);
        pom=*head;
    }
}
int* Array(List *head){
    int counter;
    List*pom=head;
    while(head!=NULL){
        counter++;
        head=head->next;
    }
    int* array=malloc(sizeof(int)*counter);
    int i=0;
    while(pom!=NULL){
        *(array+i)=pom->number;
        i++;
        
        pom=pom->next;
    }
    printf("%d\n\n\n",array[1]);
    return array;
}
void main(){
    List* head;
    InitList(&head);
    PrintList(head);
    AddToList(5,&head);
    AddToList(6,&head);
    AddToList(7,&head);
    AddToList(9,&head);
    AddToList(11,&head);
   
   /* InsetAtIndex(4,13,&head);
    PrintList(head);
    RemoveAt(4,&head);
    PrintList(head);
    Clear(&head);
    PrintList(head);*/
    int* array=Array(head);
    for (int i = 0; i < 5; i++)
    {
        printf("%d",array[i]);
    }
    
    return;
}