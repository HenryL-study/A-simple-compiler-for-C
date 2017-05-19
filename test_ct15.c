const int small=+49,big=-17;
const char z='a';
int i,j;
char op;
void fun(int num,char useless2)
{
    int f;
    if(num>=26)
    {
        return;
    }
    switch(op)
    {
        case '+':f=big;
        case '-':f=small;
        case '*':return;
    }
    printf('0'+f+num);
    if(num>1)
    {
        fun(num-1,useless2);
    }
    return;
}
void inputchar()
{
    char a;
    printf("please input op");
    scanf(a);
    if(a=='+')
    {
        op=a;
    }
    else
    {
        switch(a)
        {
            case '-':op=a;
            default:op='*';
        }
    }
}
int input()
{
    const int useless3=1;
    int questiontimes,num;
    num=0;
    printf("please input a number");
    scanf(questiontimes);
    printf("please input questiontimes");
    scanf(questiontimes);
    printf("please input num");
    scanf(num);
    if(num<0)
    {
        printf("num<0");
    }
    else if(num==0)
    {
        printf("num=0");
    }
    else
    {
        printf("num>0");
    }
    printf(questiontimes);
    while(questiontimes>0)
    {
        inputchar();
        if(op!='*')
        {
            return (num);
        }
        questiontimes=questiontimes-1;
    }
    return (num);
}
void main()
{
    const char useless2='u';
    int a;
    a=input();
    fun(a,useless2);
    return;
}