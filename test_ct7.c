int mod(int a,int b){
	int divs,_a;
	divs=a/b;
	_a=b*divs;
	return (a-_a);
}

void mod_test(){
	int cnt;
	int a, b;
	cnt = 0;
	printf("---------mod_test---------");
	while(cnt<3){
		printf("input a: ");
		scanf(a);
		printf("input b: ");
		scanf(b);
		printf("a mod b: ",mod(a,b));
		cnt=cnt+1;
	}
	printf("---------mod_test---------");
}

void main()
{
    int a;
    mod_test(7);
    return;
}