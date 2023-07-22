#ifndef __PROGTEST__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct TItem
{
    struct TItem *m_Next;
    char *m_Name;
    char m_Secret[24];
} TITEM;

#endif /* __PROGTEST__ */

TITEM *newItem(const char *name,TITEM *next)
{
    TITEM *tmp=(TITEM*) malloc(sizeof (TITEM)+3);
    tmp->m_Next=NULL;
    tmp->m_Name= (char *) malloc((strlen(name)+3)*sizeof (char ));
    strcpy(tmp->m_Name,name);
    strcpy(tmp->m_Secret,"\0");
    if(next!=NULL){
        tmp->m_Next=next;
    }
    return tmp;
}
//Helper func
/*
void PrintList(TITEM *l){
    TITEM *tmp=l;
    while(tmp != NULL) {
        printf("%s->\n",tmp->m_Name);
        tmp = tmp->m_Next;
    }
}
 */
int Cnt(TITEM * current)
{
    TITEM *tmp=current;
    int count = 0;
    while(tmp != NULL) {
        tmp = tmp->m_Next;
        count++;
    }
    return count;
}
int Compare(TITEM* first, TITEM* second, int asc){
    int value=0;
    switch (asc) {
        case 1:
            if(strcmp(first->m_Name ,second->m_Name)>0){
                value=1;
            }
            break;
        case 0:
            if(strcmp(first->m_Name ,second->m_Name)<0){
                value=1;
            }
            break;
    }
    return value;
}
TITEM* sortList(TITEM* l,int ascending)
{
    TITEM *tmp=l;
    if(Cnt(tmp)==1||Cnt(tmp)==0){
        return tmp;
    }
    for (int i = 0; i <=Cnt(tmp) ; i++) {
        TITEM*current = tmp;
        TITEM* previous =NULL;
        while(current->m_Next!=NULL)
        {
            if(Compare(current,current->m_Next,ascending)==1){
                if(previous!=NULL)
                {
                    TITEM* next_tmp = current->m_Next ;
                    previous->m_Next = current->m_Next ;
                    current->m_Next = current->m_Next->m_Next ;

                    next_tmp->m_Next = current ;
                    previous = next_tmp ;
                }
                else
                {
                    TITEM* next_tmp = current->m_Next ;
                    current->m_Next = current->m_Next->m_Next ;

                    next_tmp->m_Next = current ;
                    previous=next_tmp ;
                    tmp = previous ;
                }
            }
            else
            {
                previous = current;
                current=current->m_Next ;
            }
        }
    }
    return tmp ;
}

void freeList(TITEM *src)
{
    TITEM *tmp=src;
    TITEM* helper;
    while(tmp != NULL) {
        helper=tmp;
        tmp = tmp->m_Next;
        free(helper->m_Name);
        free(helper);
    }
}

#ifndef __PROGTEST__

int main(int argc, char *argv[]) {
    TITEM *l;
    char tmp[50];

    assert (sizeof(TITEM) == sizeof(TITEM *) + sizeof(char *) + 24 * sizeof(char));
    l = NULL;
    l = newItem("PA1", l);
    l = newItem("PA2", l);
    l = newItem("UOS", l);
    l = newItem("LIN", l);
    l = newItem("AG1", l);
    assert (l
            && !strcmp(l->m_Name, "AG1"));
    assert (l->m_Next
            && !strcmp(l->m_Next->m_Name, "LIN"));
    assert (l->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Name, "UOS"));
    assert (l->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Name, "PA2"));

    assert (l->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "PA1"));

    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);
    l = sortList(l, 1);
    assert (l
            && !strcmp(l->m_Name, "AG1"));
    assert (l->m_Next
            && !strcmp(l->m_Next->m_Name, "LIN"));
    assert (l->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Name, "PA1"));
    assert (l->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Name, "PA2"));
    assert (l->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "UOS"));
    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);
    l = newItem("programming and algorithmic I", l);
    l = newItem("AAG", l);
    assert (l
            && !strcmp(l->m_Name, "AAG"));
    assert (l->m_Next
            && !strcmp(l->m_Next->m_Name, "programming and algorithmic I"));
    assert (l->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Name, "AG1"));
    assert (l->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Name, "LIN"));
    assert (l->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "PA1"));
    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "PA2"));
    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "UOS"));
    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);
    l = sortList(l, 0);
    assert (l
            && !strcmp(l->m_Name, "programming and algorithmic I"));
    assert (l->m_Next
            && !strcmp(l->m_Next->m_Name, "UOS"));
    assert (l->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Name, "PA2"));
    assert (l->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Name, "PA1"));
    assert (l->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "LIN"));
    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "AG1"));
    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "AAG"));
    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);
    freeList(l);
    l = NULL;
    strncpy(tmp, "PA1", sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    l = newItem(tmp, l);
    strncpy(tmp, "PA2", sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    l = newItem(tmp, l);
    strncpy(tmp, "UOS", sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    l = newItem(tmp, l);
    strncpy(tmp, "LIN", sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    l = newItem(tmp, l);
    strncpy(tmp, "AG1", sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    l = newItem(tmp, l);
    assert (l
            && !strcmp(l->m_Name, "AG1"));
    assert (l->m_Next
            && !strcmp(l->m_Next->m_Name, "LIN"));
    assert (l->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Name, "UOS"));
    assert (l->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Name, "PA2"));
    assert (l->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "PA1"));
    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);
    l = sortList(l, 1);
    assert (l
            && !strcmp(l->m_Name, "AG1"));
    assert (l->m_Next
            && !strcmp(l->m_Next->m_Name, "LIN"));
    assert (l->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Name, "PA1"));
    assert (l->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Name, "PA2"));
    assert (l->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "UOS"));
    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);
    strncpy(tmp, "programming and algorithmic I", sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    l = newItem(tmp, l);
    strncpy(tmp, "AAG", sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    l = newItem(tmp, l);
    assert (l
            && !strcmp(l->m_Name, "AAG"));
    assert (l->m_Next
            && !strcmp(l->m_Next->m_Name, "programming and algorithmic I"));
    assert (l->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Name, "AG1"));
    assert (l->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Name, "LIN"));
    assert (l->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "PA1"));
    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "PA2"));
    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "UOS"));
    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);
    l = sortList(l, 0);
    assert (l
            && !strcmp(l->m_Name, "programming and algorithmic I"));
    assert (l->m_Next
            && !strcmp(l->m_Next->m_Name, "UOS"));
    assert (l->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Name, "PA2"));
    assert (l->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Name, "PA1"));
    assert (l->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "LIN"));
    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "AG1"));
    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next
            && !strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "AAG"));
    assert (l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);

    freeList(l);


    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
