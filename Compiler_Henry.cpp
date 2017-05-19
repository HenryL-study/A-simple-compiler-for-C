// Compiler_Henry.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
/*
0: normal
1: test lexical and parser.
2: test Symbol table.
3: test Symbol table & parser.
*/
int test_code = 3;

int main()
{
	using namespace std;
	//lexical_analysis s("test_ct.c");  

	//ofstream outfile;
	//outfile.open("lexical_result.txt");

	//while (s.step_once() == 0)
	//{
	//	s.print_sym(outfile);
	//}

	string cfile, needOp;
	int need;
	cout << "Please input your file directory:\n";
	cin >> cfile;
	cout << "OP?\n";
	cin >> needOp;
	if (needOp == "Y" || needOp == "y")
	{
		need = 1;
	}
	else
	{
		need = 0;
	}

	parser p(cfile);
	streambuf *backup;
	ofstream fout;
	Symbol_table s;
	Item* item;
	Type type = VAR_int;
	switch (test_code)
	{
	case 0:
		break;
	case 1:
		
		fout.open("data.out");
		backup = cout.rdbuf();
		cout.rdbuf(fout.rdbuf());
		p.analysis(need);
		cout.rdbuf(backup);
		break;
	case 2:
		s.insert_global("name", type, 1, 1234, false, false, 0, 21, 10);
		s.insert_global("name1", type, 1, 1234, false, false, 0, 22, 10);
		s.insert_global("name2", type, 1, 1234, false, false, 0, 23, 10);

		s.insert_func("func1", "name", type, 1, 1234, false, false, 0, 20, 10);
		s.insert_func("func1", "name1", type, 1, 1234, false, false, 0, 21, 10);
		s.insert_func("func1", "name2", type, 1, 1234, false, false, 0, 22, 10);
		item = s.glo_find("name1");
		cout << item->position << "\n";
		item = s.func_find("func1", "name1");
		cout << item->position;
		break;
	case 3:
		p.analysis(need);
		break;
	default:
		break;
	}
    return 0;
}

