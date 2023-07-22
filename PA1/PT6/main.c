#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
typedef char *item;
typedef struct FOUND_ITEM{
    item pattern;
    item data;
    int index;
    int capacity;

}FOUND_ITEM;
typedef struct ITEMS{
    item *data;
    int capacity;
}ITEMS;
typedef struct REGAL{
    int id;
    ITEMS list;
}REGAL;
typedef struct SHOPPING_LIST{
    ITEMS list;

    int id;
}SHOPPING_LIST;
typedef struct OPT_SHOPPING_LIST{
    FOUND_ITEM *list;
    int capacity;
    int id;
}OPT_SHOPPING_LIST;
//https://quescol.com/interview-preparation/remove-character-from-string-c
void RemoveChar(char * str, char remove){

    int length = strlen(str);
    for(int i=0; i < length; i++)
    {
        if(str[i] == remove)
        {
            for(int j=i; j < length; j++)
            {
                str[j] = str[j+1];
            }
            length--;
            i--;
        }
    }

}
int CMP(const void * a, const void * b) {
    FOUND_ITEM *inA = (FOUND_ITEM *)a;
    FOUND_ITEM *inB = (FOUND_ITEM *)b;
    return (inA->index - inB->index);
}

void PrintFunc(int cntS,SHOPPING_LIST *Arr,OPT_SHOPPING_LIST *optShoppingList){



    for (int i = 0; i <= cntS; i++) {
        printf("Optimalizovany seznam:\n");
        qsort(optShoppingList[i].list, Arr[i].list.capacity, sizeof(FOUND_ITEM), CMP);
        for (int j = 0; j <Arr[i].list.capacity ; ++j) {
            if(strcmp(optShoppingList[i].list[j].data,"N/A")==0){
                printf(" %d. %s -> %s\n",j,optShoppingList[i].list[j].pattern,optShoppingList[i].list[j].data);
            }
            else{
                printf(" %d. %s -> #%d %s\n",j,optShoppingList[i].list[j].pattern,optShoppingList[i].list[j].index,optShoppingList[i].list[j].data);
            }
        }
    }
}
void Find(int cntS,int cntR,SHOPPING_LIST *Arr,REGAL *ArrREG){

    OPT_SHOPPING_LIST *optShoppingList = (OPT_SHOPPING_LIST *) malloc((cntS+3) * sizeof(OPT_SHOPPING_LIST));

    //All lists
    for (int i = 0; i <=cntS ; i++) {

        optShoppingList[i].list=( FOUND_ITEM*) malloc((Arr[i].list.capacity+3) * sizeof(FOUND_ITEM));
        int counter=0;

        //All items in list
        for (int j = 0; j < Arr[i].list.capacity ; ) {

            RemoveChar(Arr[i].list.data[j],'\n');
            int flag=0;
            int index=0;
            int secondflag=0;

            char *x1=(char *) malloc ((strlen(Arr[i].list.data[j])+5)*sizeof (char ));
            char *x2=(char *) malloc ((strlen(Arr[i].list.data[j])+5)*sizeof (char ));

            //All shelves
            for (int k = 0; k <= cntR ; k++) {

                int help=ArrREG[k].list.capacity;
                //All items in shelves
                for (int l = 0; l < help ; l++) {

                    RemoveChar(ArrREG[k].list.data[l],'\n');


                    if((strcasecmp(ArrREG[k].list.data[l],Arr[i].list.data[j]))==0){

                        optShoppingList[i].list[counter].pattern=(char*) malloc((strlen(Arr[i].list.data[j])+5)*sizeof (char ));
                        optShoppingList[i].list[counter].data=(char*) malloc((strlen(ArrREG[k].list.data[l])+5)*sizeof (char ));

                        strcpy(  optShoppingList[i].list[counter].pattern,Arr[i].list.data[j]);
                        strcpy(   optShoppingList[i].list[counter].data,ArrREG[k].list.data[l]);
                        optShoppingList[i].list[counter].index=ArrREG[k].id;
                        counter++;
                        j++;
                        flag=1;
                        goto end;
                    }


                    else if(((strcasestr(ArrREG[k].list.data[l],Arr[i].list.data[j]))!=NULL)&&flag!=1) {

                        if (ArrREG[k].id < index||secondflag==0) {

                            if((sizeof (x1)<= strlen(Arr[i].list.data[j]))||(sizeof (x2)<= strlen(ArrREG[k].list.data[l]))) {
                                x1 = (char *) realloc(x1, (strlen(Arr[i].list.data[j])+5) * sizeof(char));
                                x2 = (char *) realloc(x2, (strlen(ArrREG[k].list.data[l])+5) * sizeof(char));
                            }
                            strcpy(x1, Arr[i].list.data[j]);
                            strcpy(x2, ArrREG[k].list.data[l]);
                            index = ArrREG[k].id;
                            secondflag = 2;
                        }
                    }


                }

            }
            end:
            if(flag==0){
                if(secondflag==0){
                    optShoppingList[i].list[counter].pattern=(char*) malloc((strlen(Arr[i].list.data[j])+5)*sizeof (char ));
                    optShoppingList[i].list[counter].data=(char*) malloc(5*sizeof (char ));

                    strcpy(  optShoppingList[i].list[counter].pattern,Arr[i].list.data[j]);
                    strcpy(   optShoppingList[i].list[counter].data,"N/A");

                    optShoppingList[i].list[counter].index=Arr[i].list.capacity;
                    counter++;
                    j++;
                }
                else{

                    optShoppingList[i].list[counter].pattern=(char*) malloc((strlen(x1)+5)*sizeof (char ));
                    optShoppingList[i].list[counter].data=(char*) malloc((strlen(x2)+5)*sizeof (char ));

                    strcpy(  optShoppingList[i].list[counter].pattern,x1);
                    strcpy(   optShoppingList[i].list[counter].data,x2);

                    optShoppingList[i].list[counter].index=index;
                    counter++;
                    j++;
                }

            }
            free(x1);
            free(x2);
        }
        optShoppingList[i].capacity=counter;

    }

    //PRINTIG
    PrintFunc(cntS,Arr,optShoppingList);

    //FREE
    for (int i = 0; i <= cntS; i++) {
        for (int j = 0; j < optShoppingList[i].capacity; ++j) {
            free(optShoppingList[i].list[j].data);
            free(optShoppingList[i].list[j].pattern);
        }
        free(optShoppingList[i].list);
    }
    free(optShoppingList);


}

int main()
{
    int firstRun=1;
    int activeREG=1;

//GETLINE HELPER
    int bytes_read=0;
    size_t size = 32;
    char *string;
    string = (char *) malloc (size*sizeof (char ));

    int ERR=0;
    int flag=0;
    int maxR=10;
    int maxS=10;
    int maxI=10;
    int cntR=-1;
    int cntS=-1;
    int cntI=0;


    ITEMS items;
    items.data=NULL;
    items.capacity=0;

    REGAL *regals = (REGAL *) malloc(maxR * sizeof(REGAL));
    SHOPPING_LIST *shoppingList = (SHOPPING_LIST *) malloc(maxS * sizeof(SHOPPING_LIST));

    int num=0;
    while ((bytes_read = getline (&string, &size, stdin))!=-1){


        if((firstRun==1)&&(string[0]!='#')){
            ERR=1;
            break;
        }
        if((string[0]=='#')&&activeREG==1){



            if((firstRun==1)&&string[1]!='0')
            {
                ERR=1;
                break;
            }
            firstRun=0;
            int tmp_num;
            char tmp;
            if(items.data!=NULL){

                items.capacity=cntI;
                regals[cntR].list=items;
                items.data=NULL;
                free(items.data);
                items.capacity=0;
            }
            cntR++;

            if(cntR>=maxR){
                maxR*=1.5;
                regals = (REGAL *) realloc(regals, maxR * sizeof(REGAL));
            }
            cntI=0;
            maxI=10;
            tmp_num=num;
            items.data= ( item*) malloc(maxI * sizeof(item));

            if((sscanf(string,"%c%d",&tmp,&num)!=2)||(num<tmp_num)){
                ERR=1;
                break;
            }
            if((num>(tmp_num+1))){
                ERR=2;
                break;
            }
            regals[cntR].id=num;
        }
        else if(string[0]=='\n'){

            activeREG=0;
            if(items.data!=NULL){
                if(flag==0){
                    items.capacity=cntI;
                    regals[cntR].list=items;
                    items.data=NULL;
                    free(items.data);
                    items.capacity=0;
                    flag=100;

                }
                else{

                    maxI=10;
                    items.capacity=cntI;
                    shoppingList[cntS].list=items;
                    items.data=NULL;
                    free(items.data);
                    items.capacity=0;
                }
            }

            cntS++;

            if(cntS>=maxS) {
                maxS *= 1.5;
                shoppingList = (SHOPPING_LIST *) realloc(shoppingList, maxS * sizeof(SHOPPING_LIST));
            }

            cntI=0;
            maxI=10;
            items.data= ( item*) malloc(maxI * sizeof(item));
            shoppingList[cntS].id=cntS;
        }

        else{

            if(cntI>=maxI){
                maxI*=2;
                items.data = ( item *) realloc(items.data, maxI * sizeof(item));
            }
            items.data[cntI]=(char*) malloc((bytes_read+5)*sizeof (char ));
            strcpy(items.data[cntI], string);
            items.capacity++;
            cntI++;
        }

    }

    if(ERR==2){

        printf("Nespravny vstup.\n");
        int index=cntR;
        if ( items.data != NULL) {
            regals[cntR].list = items;
        }
        for (int i = 0; i <= index; i++) {
            for (int j = 0; j <regals[i].list.capacity ; ++j) {
                free((regals[i].list.data[j]));
            }
            free((regals[i].list.data));
        }
        free(regals);
        free(shoppingList);
        free(string);
        return 0;
    }

    if(ERR!=0||cntS<0){

        printf("Nespravny vstup.\n");

            if (cntS < 0 && items.data != NULL) {
                regals[0].list = items;
            }

        for (int i = 0; i <= cntR; i++) {
            for (int j = 0; j <regals[i].list.capacity ; ++j) {
                free((regals[i].list.data[j]));
            }
            free((regals[i].list.data));
        }
        free(regals);
        free(shoppingList);
        free(string);
        return 0;
    }

    if(items.data!=NULL){
        shoppingList[cntS].list=items;
    }


    Find(cntS,cntR,shoppingList,regals);

    /*
     * FREE
     */
    for (int i = 0; i <= cntR; i++) {
        for (int j = 0; j <regals[i].list.capacity ; ++j) {
            free((regals[i].list.data[j]));
        }
        free((regals[i].list.data));
    }
    for (int i = 0; i <= cntS; i++) {
        for (int j = 0; j <shoppingList[i].list.capacity ; ++j) {
            free((shoppingList[i].list.data[j]));
        }
        free((shoppingList[i].list.data));
    }
    free(regals);
    free(shoppingList);
    free(string);


    return 0;

}