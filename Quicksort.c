int a[10];

void QuickSort(int min,int max)
{
    int i,j; 
    int val,ttt;
    val = a[min];
    i=min;
    j=max-1;
    if(max-min>1)
    {
        ttt = i;
        while(i<j)
        {
            while(j > i)
            {
                if(a[j]<val)
                {
                    a[i]=a[j];
                    i = i +1;
                    ttt = j;
                    j = j - 20;
                }
                j = j -1;
                if(j >= 0)
                	ttt = j;
            }
            j = ttt;
            while(i < j)
            {
                if(a[i]>val)
                {
                    a[j]=a[i];
                    ttt = i;
                    i = i +20;
                }
                i = i+1;
                if(i < 10)
                	ttt = i;
            }
            i = ttt;
        }
        a[i]=val;
        QuickSort(min,i);
        QuickSort(i+1,max);
    }
    return;
}
void main()
{
    int i,mm, tnum;
    
    i=0;
    while(i<10)
    {
        scanf(tnum);
        a[i] = tnum;
        i = i+1;
    }

    mm=0;
    QuickSort(mm,10);
    i=0;

    while(i<10)
    {
        printf(a[i]);
         printf(" ");
        i = i+1;
    }
    return;
}
