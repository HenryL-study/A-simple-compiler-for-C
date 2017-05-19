const int cInt1=+10,cInt2=-1;
const int cInt3=0;
const int cMaxInt=2147483644,cMinInt=-2147483644;
const char cChar0='_',cChar1='a',cChar2='A',cChar3='+',cChar4='*';
const char cChar5='0';
int _,_9,__,_a;
int a1;
int a2,a3[10],a4[1],a5;
void PrintHelloWorld(){
    printf(" !#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~\n");
    printf("");
    printf("helloworld!\n");
    return;
}
int SquareSum(int _a,int b){
    const int unuse=0;
    int temp;
    temp=-_a*-1;
    temp=+_a*0;
    temp=_a*unuse;
    temp=_a*_a;
    temp=-(-_a*_a)+(b*b);
    return (temp);
}
void _myStop(){
    char temp;
    printf("input a char and enter:\n");
    scanf(temp);
}
int NsquareSum(int temp,int times){
    if(times==1)
        return (temp);
    Temp=Temp*NsquareSum(temp,times-1);
    return (((Temp)));
}
void main(){
    int num;
    int temp1,temp2;
    int i,j,k;
    PrintHelloWorld();
    printf("please input number counts:\n");
    scanf(num);
    i=0;
    switch(num){
        case 0:printf("result:0\n");
        case 1:{scanf(temp1);printf("result:\n",temp1+10-10);}
        case 2:{scanf(temp1,temp2);printf("result:\n",SquareSum(temp1+0,temp2-2+2));}
        default:
            {if(num>0){
            temp2=0;
            while(i<num){
                scanf(temp1);
                temp2=temp2+NsquareSum(temp1,num);
                i=i+1;
            }
            a3[0]=temp2;
            printf("result:\n",a3[0]);}
        }
    }
    _myStop();
    k=temp1+temp2;
    i=temp1+temp2;
    j=temp1;
    j=j+temp2;
    return;
}