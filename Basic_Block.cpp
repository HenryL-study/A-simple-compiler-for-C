#include "stdafx.h"
#include "Basic_Block.h"


void Basic_Block::count_usedef()
{
	for (vector<Quaternary>::iterator it = origin_mid.begin(); it != origin_mid.end(); it++)
	{
		if (it->op == "ADD" || it->op == "SUB" || it->op == "MUL" || it->op == "DIV" || it->op == "ASSFA" || it->op == "ASSTA" || 
			it->op == "BEQ" || it->op == "BNE" || it->op == "BL" || it->op == "BS" || it->op == "BLE" || it->op == "BSE")
		{
			if (it->a[0] != '.' && it->a[0] != '-' && !isdigit(it->a[0]) && find(use.begin(), use.end(), it->a) == use.end())
			{
				use.push_back(it->a);
			}
			if (it->b[0] != '.' && it->b[0] != '-' && !isdigit(it->b[0]) && find(use.begin(), use.end(), it->b) == use.end())
			{
				use.push_back(it->b);
			}
			if (find(def.begin(), def.end(), it->res) == def.end() && find(use.begin(), use.end(), it->res) == use.end())
			{
				def.push_back(it->res);
			}
		}
		else if (it->op == "ASSIGN")
		{
			if (it->a[0] != '.' && it->a[0] != '-' && !isdigit(it->a[0]) && find(use.begin(), use.end(), it->a) == use.end())
			{
				use.push_back(it->a);
			}
			if (it->res[0] != '.' && it->res[0] != '-' && find(def.begin(), def.end(), it->res) == def.end() && find(use.begin(), use.end(), it->res) == use.end())
			{
				def.push_back(it->res);
			}
		}
		else if (it->op == "CALL" && it->b != "" && it->b[0] != '-' && it->b[0] != '.')
		{
			def.push_back(it->b);
		}
		else if (it->op == "RET" && it->res != "" && it->res[0] != '-' && it->res[0] != '.')
		{
			if (find(use.begin(), use.end(), it->res) == use.end())
			{
				use.push_back(it->res);
			}
		}
		else if (it->op == "PUSH" && it->res != "" && it->res[0] != '-'  && it->res[0] != '.')
		{
			if (find(use.begin(), use.end(), it->res) == use.end())
			{
				use.push_back(it->res);
			}
		}
		else if (it->op == "const")
		{
			if (find(def.begin(), def.end(), it->res) == def.end())
			{
				def.push_back(it->res);
			}
		}

	}
}

void Basic_Block::peephole()
{ 
	for (vector<Quaternary>::iterator it = origin_mid.begin(); it != origin_mid.end(); it++)
	{
		if (it->op == "ASSIGN")
		{
			if ((it + 1) != origin_mid.end() && (it + 1)->op == "ASSIGN" && it->res == (it + 1)->a  && it->res.c_str()[0] == '.')
			{
				if ((it + 2) != origin_mid.end() && (it + 2)->op == "ASSIGN" && (it + 1)->res == (it + 2)->a && (it + 1)->res.c_str()[0] == '.')
				{
					it->res = (it + 2)->res;
					it = origin_mid.erase(it + 1);
					it = origin_mid.erase(it);
				}
				else
				{
					it->res = (it + 1)->res;
					it = origin_mid.erase(it + 1);
				}
				it--;
			}
		}
	}
}

void Basic_Block::DAGit()
{

}

vector<Quaternary> Basic_Block::getOPmid()
{
	return vector<Quaternary>();
}

Basic_Block::Basic_Block(vector<Quaternary> origin_mid)
{
	this->origin_mid = origin_mid;
}

vector<Quaternary> Basic_Block::get_origin_mid()
{
	return origin_mid;
}


Basic_Block::~Basic_Block()
{
}
