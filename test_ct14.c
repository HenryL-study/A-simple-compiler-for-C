const int a=10;
const int b=12;
const int p1=+1,p2=-1,p3=2,p4=-1,p5=0,p6=0;
const int p7=+1,p8=+1,p9=-1,p10=-1;
const char l1='a';
const char l2='b';
const char l3='c';
const char l4='d';
const char l5='e';
const char l6='f';
char c[10];
int num[10];
int tmp1;
int tmp2;
int tmp3;
int c1,c2,c3;
char tmp4;
int f1;
int f2(){
    ;
}
void input(){
    scanf(tmp1);
    scanf(tmp2);
}
void output(){
    int i;
    i=0;
    printf(c);
    printf("result: ",tmp1+tmp2+tmp3);
    printf(tmp1);
    printf("ENd");
    while(i<10){
        printf(c[i]);
        i = i+1;
    }
}
int add(int x,int y){
    return (x+y);
}
int sub(int x,int y){
    return (x-y);
}
int mul(int x,int y){
    return (x*y);
}
int div(int x,int y){
    return (x/y);
}
void fun(int x){
    while(x<100){
        if(x>50){
            x=x*x;
        }
        if(x<100){
            x=51;
        }
    }
}
int foo(int n){
    if(n==1){
        return (1);
    }
    return (n*foo(n-1));
}
void main(){
    const int x1=1;
    const int x2=1;
    const char x3='1';
    int i;
    i=0;
    tmp1=0;
    tmp2=2;
    tmp3=0;
    tmp4='1';
    scanf(c1,c2,c3);    
    input();    
    switch(tmp4){
        case '1':  tmp1=1;
        case '2':  tmp1=2;
        case '3':  tmp1=3;
        case '4':  tmp1=4;
        case '5':  tmp1=5;
        case '6':  tmp1=6;
        case '7':  tmp1=7;
        case '8':  tmp1=8;
        case '9':  tmp1=9;
        default:   tmp1=10;        
    }
    switch(tmp4){
        case 1: ;
        default: ;
    }
    while(i<10){
        c[i]='a';
        num[i]=num[i]*tmp1;
        i = i+1;
    }
    if(tmp1>tmp2){
                num[1]=1;
        tmp3=1;
    }
    if(tmp1>=tmp2){
                num[2]=2;
        tmp3=2;
    }
    if(tmp1<=tmp2){
                num[3]=3;
        tmp3=3;
    }
    if(tmp1<tmp2){
                num[4]=4;
        tmp3=4;
    }
    if(tmp1!=tmp2){
                num[5]=5;
        tmp3=5;
    }
    if(tmp1==tmp2){
                num[6]=6;
        tmp3=6;
    }
    ;
    tmp1=1;
    tmp2=1;
    tmp4=tmp1*tmp2*10-8;
    fun(tmp4);
    tmp3=-foo(3)+mul(tmp1,tmp2)+div(tmp1,tmp2)+sub(tmp1,tmp2)+add(tmp1,tmp2)+tmp3;
    output();
    return;
}