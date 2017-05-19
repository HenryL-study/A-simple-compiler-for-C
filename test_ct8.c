void alphabet(char ELSE){
	char ch;
	ch=ELSE;
	while(ch<=ELSE+25){
		printf(ch);
		ch=ch+1;
	}
	printf("\n");
}

void alphabet_test(){
	printf("---------alphabet_test---------");
	printf("Uppercase: ");
	alphabet('A');
	printf("Lowercase: ");
	alphabet('a');
	printf("---------alphabet_test---------");
}


void main(){

	alphabet_test();

}