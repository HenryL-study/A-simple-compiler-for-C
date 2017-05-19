#include "stdafx.h"
#include "DAG.h"


int DAG::CreateNode(bool isleaf)
{
	Dnode d;
	d.isleaf = isleaf;
	DAGragh.push_back(d);

	return DAGragh.size()-1;
}

int DAG::CreateNode(bool isleaf, int lchild, int rchild, string op)
{
	Dnode d;
	d.isleaf = isleaf;
	d.lchild = lchild;
	d.rchild = rchild;
	d.op = op;
	DAGragh.push_back(d);

	return DAGragh.size() - 1;
}

void DAG::addFather(int fa, int node)
{
	DAGragh[node].fnode.push_back(fa);
}

/*
The last quaternary of the basic block should be one of the items list below
1.	PUSH
2.	branch OP
3.	CALL
*/
DAG::DAG(vector<Quaternary> basic_block)
{
	for (std::vector<Quaternary>::iterator it = basic_block.begin(); it != basic_block.end(); it++)
	{
		if (it->op == "ASSIGN")
		{
			std::map<std::string, int>::iterator iter = DAGtable.find(it->a);
			if (iter != DAGtable.end()) //find in table
			{
				DAGtable[it->res] = DAGtable[it->a];
			}
			else
			{

			}
		}
		else if (it->op == "ASSFA")
		{

		}
		else if (it->op == "ASSTA")
		{

		}
		else if (it->op == "ADD")
		{

		}
		else if (it->op == "SUB")
		{

		}
		else if (it->op == "MUL")
		{

		}
		else if (it->op == "DIV")
		{

		}
		else if (it->op == "PUSH")
		{

		}
	}
}

DAG::~DAG()
{
}
