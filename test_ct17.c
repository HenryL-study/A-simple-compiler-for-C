const int cona = 1, conb = 2;       
const int conc = 3, cong = 4;        
const int conh = 5, coni = 6, conj = 7;       
int a[10];      
int c;
void emptyfunc(){}    
void emptyfunc2(){;}   
char testchar(){     
    return ('0');
}
int fibonacci (int n){   
    const int testcon = 1;        
    char testc;
    if(n==1) return (1);    
    if(n!=2) return (+fibonacci(n+-1)+fibonacci(n-2)+0); 
    return (1);
}
void move(char a, char b){        
    const int testcon1 = 1;            
    const char testcon2 = '1';
    printf("MOVE");    
    printf(" ");
    printf(a);
    printf(" TO ",b);
    printf(" ");
}
void hanoi(int n0, char l, char m, char r){    
    if(2>n0) {
        move(l,r);
        return;
    }
    hanoi(n0-1, l, r, m);    
    move(l,r);       
    hanoi(n0-1, m, l, r);
}
int getpower (int n, int le){
    int tempans;       
    if(le>=2){    
        tempans = getpower(n,le-1);
        a[le-1] = tempans;    
        return (n*tempans);
    }
    if(1<=le){
        a[le] = n;
        return (n);
    }
}
int getsqr(int n){
    int _i;
    _i = 1;
    while(_i < n / _i) _i = +1 + _i;
    if(-n+_i*_i < n-(-1+_i)*(_i-1)) return (_i);   
    return (_i-1);
}
void main(){
    const char cond = 'A', cone = 'B', conf = 'C'; 
    const char con1 = '+', con2 = '-';   
    const char con3 = '*';    
    const int testcon = 1;    
    
    int c;   
    int num,i;    
    int fib,testaj[10],sqr;  
    int testai[10],a,a_,a_a,a0,a00;
    int testai1,testai2,testai3,testai4,testai5,testai6;

    char c1;  
    char c2,c3;   
    char c4,c5,c6;      

    testai[0] = 5;
    scanf(c,num);    
    switch(c){    
        case 1:{
            getpower(num, 10); 
            i = 0;
            while(i<10){    
                printf("NO",i+1);
                printf(":", a[i]);
                printf(" ");
                printf("");
                i=i+1;
            }
        }
        default: ;
    }
    switch(c){    
        case 2: hanoi(num, cond, cone, conf); 
        case 3: {
            fib = fibonacci(num);    
            printf(fib);
        }
        default:;
    }
    switch(c){    
        case 4: {
            sqr = getsqr(num);   
            printf(sqr);
        }
        case 5:{ 
            printf(testchar());
            printf(3*2/6*testai[0]); 
        }
        case 6:{
            if(testai[0]){         
                printf("IFCORRECT");
            }
            while(testai[0]){    
                printf(testai[0]);
                testai[0] = testai[0] - 1;
            }
        }
        case 7: {
            scanf(testai1);        
            scanf(testai2,testai3);
            scanf(testai4,testai5,testai6);
            printf(testai1);
            printf(testai2);
            printf(testai3);
            printf(testai4);
            printf(testai5);
            printf(testai6);
        }
        default: printf("ERROR");
    }
    return;
}