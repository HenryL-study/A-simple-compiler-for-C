void echo_test(){	
	int echo_int;
	char echo_char;
	printf("---------echo_test---------");
	printf("input an int and a char:");
	scanf(echo_int,echo_char);
	printf("echo_int: ",echo_int);
	printf(" echo_char: ",echo_char);
	printf("---------echo_test---------");
	return;
}

void main()
{
    echo_test();
    return;
}