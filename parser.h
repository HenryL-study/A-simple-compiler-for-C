#pragma once

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <queue>
#include "lexical_analysis.h"
#include "types_tools.h"
#include "Symbol_table.h"
#include "midcode.h"

class parser
{
private:
	std::queue<std::string> symbols; //pre-read symbol set
	std::queue<int> types;
	
	std::string  file_name;
	lexical_analysis *s;

	//symbol table
	Symbol_table *table;
	std::string ident_name;
	int data_addr;
	bool is_con = false;
	bool is_arr = false;
	bool is_glo = false;
	int var_type;
	string value;
	int num_value;
	std::string func_name;
	Type t;
	int para_num = 0;
	int addr_offset = 0;					//offset for func var & const & parameter

	//midcode
	midcode *m;
	int need;
	int label_num = 0;
	std::string qlabel;
	int t_num = 0;
	std::string qtemp;
	int exp_num = 0;
	std::string qexp;
	int term_num = 0;
	std::string qterm;
	int fac_num = 0;
	std::string qfac;
	int str_num = 0;
	std::string strings_name;
	//bool ifcon = false;
	std::string swi_ans;
	std::string swi_lab;

	std::string is_CHAR = "";
	

	//lexical
	bool getsym();

	void program();
	void con_state();		//constant statement
	void var_state();		//variable statement
	void func_re();			//function with return value
	void func_vo();			//function without return value
	void func_main();		//main fuction
	void con_def();			//constant defination
	void var_def();			//variable defination
	void def_head();		//head of a function defination
	void func_para();		//parameters of a function
	void compound_state();	//compound statement
	void para_list();		//parameters list
	void statement_col();	//statements column
	void statement();		//
	void _if();
	void _while();
	void assign();			//assign statement
	void _read();
	void _write();
	void _switch();
	void _return();	
	void condition();		//condition in if/while
	void condition_list();
	void _default();
	void condition_sub();	//substatement of a condition list
	void value_paras();		//parameters value list
	void expression();
	void term();
	void factor();
	void _integer();
	void funcre_call();
	void funcvo_call();

	void error(int type);
	int errs = 0;

	inline void gen_label()
	{
		qlabel = ".label" + numTOstrs(label_num);
		label_num++;
	}
	inline void gen_temp()
	{
		qtemp = ".t" + numTOstrs(t_num);
		t_num++;
	}
	inline void gen_exp()
	{
		qexp = ".exp" + numTOstrs(exp_num);
		exp_num++;
	}
	inline void gen_term()
	{
		qterm = ".te" + numTOstrs(term_num);
		term_num++;
	}
	inline std::string gen_fac()
	{
		qfac = ".fac" + numTOstrs(fac_num);
		fac_num++;
		return qfac;
	}

	inline std::string gen_str()
	{
		strings_name = ".String" + numTOstrs(str_num);
		str_num++;
		return strings_name;
	}

	inline void cln_temp() { t_num = 0; }
	inline void cln_exp() { exp_num = 0; }
	inline void cln_term() { term_num = 0; }
	inline void cln_fac() { fac_num = 0; }

	inline std::string get_label() { return qlabel; }
	inline std::string get_temp() { return qtemp; }
	inline std::string get_exp() { return qexp; }
	inline std::string get_term() { return qterm; }
	inline std::string get_fac() { return qfac; }


public:
	void analysis(int need);
	parser(std::string file_name);
	~parser();
};

#endif
