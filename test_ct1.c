/*测试常量声明*/
const int _const_1 = +101;
const int _const_2 = -101, _const_3 = 0, _const_4 = 5;
const char _const_c_1 = '+', _const_c_2 = '_', _const_c_3 = 'a';
/*测试变量声明*/
int _var_1;
int _var_2,_int_array[10];
char _var_4,_char_array[10];
/*测试函数定义*/
int _return_0(){return (0);}

void main()
{
    const int _const_1 = 101;
    _var_4 = 10;
    printf("should not happend!!");
    printf(_const_1);
    printf("43:",_var_4);
    _var_2 = _return_0();
    printf("0:",_var_2);
    printf("0:",_return_0());
    return;
}