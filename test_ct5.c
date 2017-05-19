int fibo1(int n){
	if(n<=0)
		return (-1);
	else if(n<=2){
		return (1);
	}else{
		return (fibo1(n-1)+fibo1(n-2));
	}
	return (-2);
}

int fibo2(int n){
	const int MAX=10;
	int ary[11];
	int i;
	if(n>MAX)
		return (-1);
	i=1;
	while(i<=n){
		switch(i){
			case 1:ary[i]=1;
			case 2:ary[i]=1;
			default: ary[i]=ary[i-1]+ary[i-2];
		}
        printf(ary[i]);
        printf("\n");
		i = i+1;
	}
	return (ary[n]);
}

int fibo3(int n){
	const int MAX=10;
	int ary[15];
	int i;
	if(n>MAX)
		return (-1);
	i=1;
	while(i<=n){
		if(i==1)
			ary[i]=1;
		else if(i==2)
			ary[i]=1;
		else
			ary[i]=ary[i-1]+ary[i-2];
		i=i+1;
	}
	
	return (ary[n]);
}

void main()
{
    int a;
    a = fibo3(7);
    printf(a);
    return;
}