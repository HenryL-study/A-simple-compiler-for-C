void main()
{
    int num,temp1,temp2;
    scanf(num);
     switch(num){
        case 0:printf("result:0\n");
        case 1:{printf("result:1\n");}
        case 2:{scanf(temp1,temp2);printf("result:\n",temp1+temp2);}
        default:
            {
            temp2=0;
            printf("result:\n",temp2);
        }
    }
}