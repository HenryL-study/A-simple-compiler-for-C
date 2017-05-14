#pragma once
#include <vector>

using namespace std;

struct Quaternary
{
	string op;
	string res;
	string a;
	string b;
};

class Basic_Block
{
private:
	vector<Quaternary> origin_mid;

public:
	vector<int> pre;
	vector<int> next;

	//live-variable analysis
	vector<string> in;
	vector<string> out;
	vector<string> use;
	vector<string> def;
	void count_usedef();
	void peephole();

	void DAGit();
	vector<Quaternary> getOPmid();
	Basic_Block(vector<Quaternary> origin_mid);

	vector<Quaternary> get_origin_mid();

	inline Quaternary get_first_oq()
	{
		return origin_mid[0];
	}
	inline Quaternary get_last_oq()
	{
		return origin_mid.back();
	}
	inline Quaternary get_ith_oq(int i)
	{
		if (i >= origin_mid.size())
		{
			return origin_mid[0];
		}
		return origin_mid[i];
	}

	~Basic_Block();
};

