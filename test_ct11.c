const int a=1;
const int a0=1,a1=1;
const char b0='a',b1='b';
int aa0,aa1,aa2; 
char ab0,ab1,ab2,ab3;
int add(int a){
    if(a>0) return (a+add(a-1));
    else return (0);
}
int add_a_b_c(int a , int b,char c){ 
    int ab1,ab2;
    ab1=a+b;
    ab2=ab1+c;
    return (ab2);
}
char get_c(char a,int b){
    a=a+b;
    return (a);
}
void new_print(int a,int b){
    printf("2333333");
    printf((a+b));
    a=a+b;
    printf("2333333",(a+b));
}
void main(){
    int arr[10];
    int k,m;
    char az,ba;
    scanf(k,m);
    scanf(az,ba);
    arr[2]=3;
    arr[1]=add(arr[2]);
    printf(arr[1]);
    m=k;
    while(m>0){
        aa0=m+aa0;
        switch(m){
            case 2:{
                aa1=3;
                printf(aa1);
            } 
            case 1:{
                aa1=2;
                printf(aa1);
            }
            default :{
                aa1=0;
                printf(aa1);
            }
        }
        if(m==2){
            aa1=3;
            printf(aa1);
        }else {
            aa1=2;
            printf(aa1);
        }
        m=m-1;
    }
    printf(aa0);
    k=add(k);
    printf(k);
    k=1;
    m=k+(m+k*m);
    printf(m);
    k=add_a_b_c(k,m,az);
    printf(k);
    az=get_c(az,a);
    printf(az);
    new_print(a0,a1);

}