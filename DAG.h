#pragma once


struct Dnode
{
	vector<int> fnode;
	bool isleaf;
	string op;
	int lchild;
	int rchild;
};

class DAG
{
private:
	vector<Dnode> DAGragh;
	map<string, int> DAGtable;

	int CreateNode(bool isleaf);
	int CreateNode(bool isleaf, int lchild, int rchild, string op);

	void addFather(int fa, int node);

public:
	DAG(vector<Quaternary> basic_block);
	~DAG();
};

