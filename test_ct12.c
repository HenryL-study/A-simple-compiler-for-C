/*bad*/
const int _a=+1;
const int b=-1;
const char ch1='-'; 
const char ch2='n';
int REsult[3000];

void pr(int i,int j){
    if(i==-1)
    return;
    switch(j){
        case 0:;
        default:{
        if((j-(j/50)*50)==0)
            printf("\\",ch2);
        }
    }
    j=j+1;
    i=i-1;
    printf(" ",result[i]);
    pr(i,j);    
} 
int getNum(){
    const int x=1;
    int y;
    y=0;
    while(y<1000){
        if(y<100)
            y=y+1;
        else
            y=y+2;
    }
    return (y);
}

int getZero(){
    int x;
    int re;
    x=0;
    re=1;
    while(x<=10)
        x=re+x;
    return (x-11);
}

char getChar(){
    char ch;
    ch='m';
    return (ch+1);
}

void main(){
    int n,i,pce,j,length,t;
    char ch3;
    ch3 = getChar();
    result[0]=2;
    scanf(n);
    i=3;
    while(i<=n){
    pce=getZero();        
    j=getZero();
    while(j<=1000){
    t=reSUlt[j]*i;
    resulT[j]=(result[j]*i+pce)-((result[j]*i+pce)/10)*10;
    pce=(t+pce)/10;
    j=j+_a;
    }
    i=i+_a;
    }
    length=getNum();
    while(result[length]==0)
        length=length+b;
    i=length;
    j=0;
    printf("start");
    printf(ch1);
    printf(ch2);
    pr(i,j);
    printf("\\",ch3);
}