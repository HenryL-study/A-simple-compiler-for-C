#pragma once

#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H

#include <string>
#include <iostream>
#include <cstring>
#include <ctype.h>
#include <sstream>
#include "types_tools.h"

#define FILE_END 1
#define FILE_NO_FIND 2
#define FILE_READING 3


class lexical_analysis
{
private:
	

	unsigned int location;   //col
	unsigned int line; //line
	int symbol;
	std::string token;
	int num;
	int error_type;

	std::ifstream cfile;
	std::ofstream outfile;
	char word[1024];
	std::string word_s;
	int file_status;

	bool isLetter(char ch);
	bool isColon(char ch);
	bool isBigger(char ch);
	bool isLess(char ch);
	bool isSurprise(char ch);
	bool isEqu(char ch);
	bool isPlus(char ch);
	bool isMinus(char ch);
	bool isMul(char ch);
	bool isLpar(char ch);
	bool isRpar(char ch);
	bool isLbraket(char ch);
	bool isRbraket(char ch);
	bool isLbrace(char ch);
	bool isRbrace(char ch);
	bool isComma(char ch);
	bool isSemi(char ch);
	bool isDivi(char ch);
	bool isSquote(char ch);
	bool isDquote(char ch);
	int reserver(std::string word);
	int transNum(std::string num);
	int error(int type);

	
	



public:
	int getsym(std::string words);
	void print_sym(std::ofstream &outfile);
	int getnum();
	int getsymbol();
	int step_once();
	unsigned int getline();
	unsigned int getcol();
	std::string gettoken();
	inline int get_err() { return error_type; }
	lexical_analysis(std::string file);
	~lexical_analysis();
};

#endif