/*测试常量声明*/
const int _const_1 = +108;
const int _const_2 = -101, _const_4 = 5;
const char _const_c_1 = '+', _const_c_2 = '_', _const_c_3 = 'a';
/*测试变量声明*/
int _var_1;
int _var_2,_int_array[10];
char _var_4,_char_array[10];
/*测试函数定义*/
int _return_0(){return (0);}
/*return a*b*/
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
void _print_na(char a, int b)
{
	while(b>0)
	{
		b = b - 1;
		printf(a);
	}
}
/*echo a*/
char _echo_4(char a)
{
	return (a);
}
void main()
{
	const int _const_1 = 101;
	int _varm_1, _;
	/*test if*/
	if (_return_0() != 0)
		_print_na(_const_c_2,_const_4);
	else
		_varm_1 = _return_0();
	/*test while*/
	while(_varm_1 < 10)
	{
		_varm_1 = _varm_1 + 1;
		_int_array[_varm_1-1] = _varm_1 - 1;
		_varm_1 = _int_array[_varm_1-1] + 1;
		_print_na(_const_c_3,_const_4);
	}
	printf("\n");
	printf("_varm_1:",_varm_1);
	printf("\n");
	/*test sentence lines*/
	{
		_varm_1 = ++1-+2*_const_1-(_echo_4(_const_c_1)+1)/4*2+'a'-'s'*_int_array[_varm_1-2]; /*1046*/
	}
	printf("_varm_1:",_varm_1);
	printf("\n");
	printf("1:",_int_array[1]); /* =1*/
	printf("\n");
	_var_4 = _echo_4(_const_c_1);
	printf("+:",_var_4);
	printf("\n");
	_int_array[0] = _return_ab(_var_4,_int_array[3]); /*=30*/
	printf("132:",_int_array[0]);
	printf("\n");
	/*test switch*/
	switch (_varm_1)
	{
		case -1046:
		{
			printf("Right!",_int_array[2]);
		}
		default:
		{
			printf("Wrong!");
		}
	}

	/*test read*/
	printf("\n");
	scanf(_var_1,_var_2,_);
	printf(_var_1);
}