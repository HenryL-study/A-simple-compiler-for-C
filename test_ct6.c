int pow(int bottom, int exp){
	int ans,i;
	ans=1;
	i=1;
	while(i<=exp){
		ans=ans*bottom;
		i=i+1;
	}
	return (ans);
}

void pow_test(){
	int cnt;
	int bottom, exp;
	cnt = 0;
	printf("---------pow_test---------");
	while(cnt<3){
		printf("input bottom: ");
		scanf(bottom);
		printf("input exponent: ");
		scanf(exp);
		printf("pow: ",pow(bottom,exp));
		cnt=cnt+1;
	}
	printf("---------pow_test---------");
}


void main()
{
    int a;
    pow_test();
    printf(a);
    return;
}