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

//Good
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
void PrintList(TITEM *l){
    TITEM *tmp=l;
    while(tmp != NULL) {
        printf("%s->\n",tmp->m_Name);
        tmp = tmp->m_Next;
    }
}

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
void Merge(TITEM** start1,TITEM** end1,TITEM** start2, TITEM** end2,int asc)
{
    TITEM* tmp = NULL;
    TITEM* tmp1=NULL;
    if (((strcmp((*start1)->m_Name,(*start2)->m_Name))>=0)&&asc==1)
    {
        //SWAPING
        tmp1=*start2;
        *start2=*start1;
        *start1=tmp1;

        tmp1=*end2;
        *end2=*end1;
        *end1=tmp1;

    }
    else if (((strcmp((*start1)->m_Name,(*start2)->m_Name))<=0)&&asc==0)
    {
        //SWAPING
        tmp1=*start2;
        *start2=*start1;
        *start1=tmp1;

        tmp1=*end2;
        *end2=*end1;
        *end1=tmp1;
    }

    TITEM* astart = *start1;
    TITEM* aend = *end1;
    TITEM* bstart = *start2;
    TITEM *bend = *end2;
    TITEM* bendnext = (*end2)->m_Next;


    while (astart != aend && bstart != bendnext) {

        if ((strcmp( astart->m_Next->m_Name,bstart->m_Name)>=0)&&asc==1) {
            tmp = bstart->m_Next;
            bstart->m_Next = astart->m_Next;
            astart->m_Next = bstart;
            bstart = tmp;
        }

        else if ((strcmp( astart->m_Next->m_Name,bstart->m_Name)<=0)&&asc==0) {
            tmp = bstart->m_Next;
            bstart->m_Next = astart->m_Next;
            astart->m_Next = bstart;
            bstart = tmp;
        }

        astart = astart->m_Next;
    }
    if (astart == aend)
        astart->m_Next = bstart;
    else
        *end2 = *end1;

}
TITEM *sortList(TITEM * l,int ascending)
{
    if (l == NULL||l->m_Next==NULL){
        return l;
    }
    int lenght = Cnt(l);
    int counter=0;
    TITEM *tmp = l;

    TITEM *start1 = NULL;
    TITEM *end1 = NULL;
    TITEM *start2 = NULL;
    TITEM *end2 = NULL;

    TITEM *prevend = NULL;

    printf("%d\n", Cnt(tmp));

    for (int i = 1; i < lenght; i *= 2) {
        //printf("BRUH\n");
        start1 = tmp;
        while (start1!=NULL) {

            int isFirstIter = 0;
            if (start1 == tmp){
                isFirstIter = 1;
            }

            end1 = start1;
            counter = i;
            while ((counter-1>0) && (end1->m_Next!=NULL)){
                counter--;
                end1 = end1->m_Next;
            }

            start2 = end1->m_Next;
            if (start2==NULL){
                break;
            }

            end2 = start2;
            counter = i;
            while ((counter-1>0) && (end2->m_Next)!=NULL){
                counter--;
                end2 = end2->m_Next;
            }

            TITEM *temp = end2->m_Next;
            //printf("Start1:%s, End1:%s, Start2:%s, End2:%s\n",start1->m_Name, end1->m_Name, start2->m_Name, end2->m_Name);
            Merge(&start1, &end1, &start2, &end2,ascending);

            //NECHAPU
            if (isFirstIter==1){
                tmp = start1;
            }
            else{
                prevend->m_Next = start1;

            }
            start1 = temp;
            prevend = end2;

        }
        prevend->m_Next = start1;
    }
    return tmp;
}

//Good
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
    PrintList(l);
    l = sortList(l, 1);
    printf("\n");
    PrintList(l);
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
