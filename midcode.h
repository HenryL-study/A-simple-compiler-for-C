#pragma once
#include <vector>
#include "BB_Manager.h"

using namespace std;



class midcode
{
private:
	vector<Quaternary> code_mid;
	ofstream CODE;
	void search_string();
	
	std::string temp_reg[10];
	std::string save_reg[8];
	std::map<std::string, std::string> temp_var;
	int var_num = 0;
	int get_temp(std::string var_name);
	int offset_of_func = 0;
	int save_num = 0;
	int get_save(std::string var_name);
	int get_save_op(std::string var_name);
	void s_wri_del();
	void t_wri_del();
	void s_wri();
	void s_del();

	int para_num = 0;
	vector<std::string> para_list;
	vector<std::string> call_stack;
	//vector<int> offset_stack;
	void pop_stack();
	void push_stack();
	void load_para(int paras);

	std::map<std::string, int> var_off;

	int MaxOff = 0;

	void clear_varTable();
	Symbol_table *table;
	std::string func_name;

	void error(int type, std::string var_name);

	//optimize
	BB_Manager* BBm = nullptr;
	bool needop = false;

public:
	void insert(string op, string a, string b, string res);
	void gen_code(Symbol_table *table);
	void gen_globaldata();
	void printMidcode();

	void init_BB();
	void setOP();

	midcode();
	~midcode();
};

