int _return_ab(int a, int b)
{
	int temp;
	if (a == 0)
		return (b);
	else
	{
		a = a - 1;
		temp = _return_ab(a,b);
	}
	return (b + temp);
}

void main()
{
    int a;
    a = _return_ab(2,4);
    printf(a);
    return;
}