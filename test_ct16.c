const int s_c_i1 = 1;            
const int s_c_i2 = +2  ,  s_c_i3 = -3;    
const char s_c_c1 = 'a'; 
const char s_c_c2 = '_',s_c_c3 = 'c'; 
int S_v_i;                         
char s_V_c;
int s_v_iarr[2];

int test1(){}         
int test2(){
    return (1);
}
int test3(int arg){} 
int test4(int arg){
    const int i = 0;
    int arr[10];
    ;                 
    if(arg < 0)
        return (-1);
    else{
        while(arg < 10)
            arg = arg+1;
    }
    return;         
}

void test5(int i, char c){
    return;
}
void test6(int i, char c){
    int j;
    j = i;
    j = +i -i;
    j = i +i;
    j = -i + -3;
    j = -i - 0;
    j = +-3 - -2;
    j = -0 + i;
}

int add(int arg){
    const int i = 1; 
    return (arg+i);
}
void print(int arg){
    switch(arg){
        case 0:
            printf("n is too low");
        case 1:
            printf("n is too high");
        case 2:
            printf("Fibonaccis are:");
        case 3:
            printf(" ");
        default:
            printf("wrong arg");
    }
    printf(arg);
}
void main(){
    const int f_0 = 1;
    int f[1000];
    int num;
    int i;
    int n;

    f[0] = f_0;
    f[1] = f_0;

    scanf(n);

    if(n<=2){
        print(0);
        return;
    }
    if(n>1000){
        print(1);
        return;
    }

    i = 0;
    print(2);
    while(i<n){
        if(i>=2){
            f[i]=f[i-1]+f[i-2];
        }
        printf(f[i]);
        print(3);
        i=add(i);
    }

    return;

}