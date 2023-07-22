#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <bits/stdc++.h>
//using namespace std;
typedef struct Side{
    char name;
    int *arr;
    int capacity;
    int pos;
}Side;
//map<vector<int>, int> memo;
int tmparr[1]={0};
int hodnota=0;
Side north,east,west,south;

/*
int n[2]={ 1, 2 };
int e[3]={ 1,1,1 };
int w[1]={ 5 };
int s[2]={ 1, 7 };
*/

int n[3]={ 1, 2, -4 };
int e[3]={ 9, 1, 1 };
int w[3]={ 3, 5, -2 };
int s[2]={ 1, 7 };

/*
int n[2]={ 1, 2 };
int e[4]={ 9,1,1,1 };
int w[2]={ 3,5 };
int s[2]={ 1, 7 };
*/
void uf(){


    north.capacity=3;
    north.pos=-1;
    north.arr=n;
    north.name='N';

    east.capacity=3;
    east.pos=-1;
    east.arr=e;
    east.name='E';

    west.capacity=3;
    west.pos=-1;
    west.arr=w;
    west.name='W';

    south.capacity=2;
    south.pos=-1;
    south.arr=s;
    south.name='S';
}

int max(int a, int b, int c,int d){
    int max=0;
    if (a >= b && a >= c && a>=d){
        max=a;
    }
    if (b >= a && b >= c && b>=d){
        max=b;
    }
    if (c >= a && c >= b && c>=d){
        max=c;
    }
    if (d >= a && d >= b && d>=c){
        max=d;
    }
    return max;
}
int min(int a, int b, int c,int d){
    int min=0;

    if (a <= b && a <= c && a<=d){
        min=a;
    }
    if (b <=a && b <=c && b<=d){
        min=b;
    }
    if (c <= a && c <= b && c<=d){
        min=c;
    }
    if (d <= a && d <= b && d<=c){
        min=d;
    }
    return min;
}
/*
int ks=0;
int option1=0;
int option2=0;
int option3=0;
int option4=0;

int Rec(int n, int s, int w, int e){




if(n<north.capacity){
     option1  = north.arr[n] + min(Rec(n+2,s,w,e), Rec(n+1,s+1,w,e),Rec(n+1,s,w+1,e),Rec(n+1,s,w,e+1));

}


if(s<south.capacity){
     option2 = (south.arr[s] + min(Rec(n,s+2,w,e), Rec(n+1,s+1,w,e),Rec(n,s+1,w+1,e),Rec(n,s+1,w,e+1)));

}


if(w<west.capacity) {
    option3 = west.arr[w] +
              min(Rec(n, s, w + 2, e), Rec(n + 1, s, w + 1, e), Rec(n, s + 1, w + 1, e), Rec(n, s, w + 1, e + 1));
}

if(e<east.capacity){
     option4 = east.arr[e] + min(Rec(n,s,w,e+2), Rec(n+1,s,w,e+1),Rec(n,s+1,w,e+1),Rec(n,s,w+1,e+1));

}
else{
    return 0;
}

    hodnota=max(option1,option2,option3,option4);
    return hodnota;


}
 */
int lvl=-1;
int Rec(Side fop,int n){
    //printf("%d",west.arr[n]);
    lvl++;
    if(n>=fop.capacity){

        return 0;
    }


    if(fop.name==north.name){
        north.pos++;

    }
    if(fop.name==west.name){
        west.pos++;

    }
    if(fop.name==south.name){
        south.pos++;

    }
    if(fop.name==east.name){
        east.pos++;

    }
    int option1  = fop.arr[n] +  min(Rec(fop,n+2), Rec(north,lvl),Rec(west,lvl),Rec(south,lvl));

    int option2  = fop.arr[n] + min(Rec(fop,n+1), Rec(north,lvl+1),Rec(west,lvl),Rec(south,lvl));

    int option3  = fop.arr[n] + min(Rec(fop,n+1), Rec(north,lvl),Rec(west,lvl+1),Rec(south,lvl));

    int option4  = fop.arr[n] + min(Rec(fop,n+1), Rec(north,lvl),Rec(west,lvl),Rec(south,lvl+1));

    hodnota=max(option1,option2,option3,option4);
    lvl--;
    return hodnota;
   // + min(Rec(fop,n+2), Rec(north,north.pos),Rec(west,west.pos),Rec(south,south.pos));
}
int optimalStrategyOfGame()
{

   // memo.clear();
    return Rec(east,0);
    //return Rec(0,0,0,0);
}

int main() {

   // printf("%d", min(1200,1020,1201,125));
    //optimalStrategyOfGame(north,east,west,south);
   // printf("--%d\n",south.arr[2]);
    //printf("%d\n", min(2,3,5,NULL));
    uf();

    printf("%d\n",optimalStrategyOfGame());
    return 0;
}
