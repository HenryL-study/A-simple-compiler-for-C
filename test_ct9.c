const int a=1, BB=-1, b1=0;
const char x='p', y='0',z='*';
int m,n[20];
int w[10],yy;
int u[5];
int v;
int vv,vvv,vvvv,vvvvv;
char s1,s2[6];
int f1(int p){
    return (p*2+p/2);
}
char f2(int q1,char q2){
    int i;
    i=q1+BB;
    while(i<=6){
        s2[i]=q2;
        i=i+1;
    }
    return (s2[5]);
}
void g1(){
    int i,tmp;
    i=BB;
    while(a-i>0){
        if(1){
            printf("hello");
        }
        i=i+1;
    }
}
int g2(char q3){
    s1=q3;
    return (0);
}
int h1(int n){
    if(n==0){
        return (0);
    }
    printf(n);
    n=n-1;
    h1(n);
}
void main(){
    const int c1=1, c2=+20;
    int c3;
    int i,j,k,num[10],e1,e2,key,x_1;
    char str[20];
    if(c1==1){
        i=0;
        j=0;
        k=0;
    }
    while(i!=10){
        num[i]=c1;
        i=i+1;
    }
    while(9>=j){
        str[j]='o';
        j=j+1;
    }
    g2(str[k]);
    g1();
    printf(f2(1,'*'));
    c3=f1(c2);
    ;
    key=c3+c3;
    switch(key){
        case 100 : x_1=a+b1;
        case 2 : x_1=a-b1;
        default : x_1=0;
    }
    printf("key=c3+c3:x_1=",x_1);
    key=c3-c3;
    switch(key){
        case 100 : x_1=a+b1;
        case 2 : x_1=a-b1;
        default : x_1=0;
    }
    printf("key=c3-c3:x_1=",x_1);
    key=c3/c3+c3/c3;
    switch(key){
        case 100 : x_1=a+b1;
        case 2 : x_1=a-b1;
        default : x_1=0;
    }
    printf("key=c3/c3+c3/c3:x_1=",x_1);
    h1(2);
    printf("please input 2 numbers");
    scanf(e1,e2);
    printf(e1+e2);
}