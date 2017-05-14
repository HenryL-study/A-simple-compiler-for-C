#pragma once
#include "Basic_Block.h"
#include <vector>
#include <map>
#include <algorithm>

class BB_Manager
{
private:
	vector<Basic_Block> BBs;
	map<string, map<string, int>> func_reg_alloc;
	int regs_num = 7;

	void count_inout();
	int _process(int i);

public:
	BB_Manager(vector<Quaternary> origin_mid);
	vector<Quaternary> get_op();
	int find_reg(string funcname, string var);

	void Print_Block();
	~BB_Manager();
};

