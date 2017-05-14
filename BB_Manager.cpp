#include "stdafx.h"
#include "BB_Manager.h"

/*
basic block divid standerd:
1.	after branch
2.	begin with SET/BEGIN
3.	after GO/END/CALL/RET
*/
BB_Manager::BB_Manager(vector<Quaternary> origin_mid)
{
	vector<Quaternary> Block_mid;
	for (std::vector<Quaternary>::iterator it = origin_mid.begin(); it != origin_mid.end(); ++it)
	{
		if (it->op == "SET" || it->op == "BEGIN")
		{
			if (Block_mid.size() > 0)
			{
				Basic_Block b(Block_mid);
				BBs.push_back(b);
			}
			Block_mid.clear();
			Block_mid.push_back(*it);
		}
		else if (it->op == "GO" || it->op == "END" || it->op == "BEQ" || it->op == "BNE" || it->op == "BL" || it->op == "BS" || it->op == "BLE" || it->op == "BSE" || it->op == "RET")
		{ 
			Block_mid.push_back(*it);
			Basic_Block b(Block_mid);
			BBs.push_back(b);
			Block_mid.clear();
		}
		else if (it->op == "CALL" || it->op == "PUSH" || it->op == "RD" || it->op == "PD")
		{
			if (Block_mid.size() > 0)
			{
				Basic_Block b(Block_mid);
				BBs.push_back(b);
			}
			Block_mid.clear();
			//single block
			Block_mid.push_back(*it);
			Basic_Block b(Block_mid);
			BBs.push_back(b);
			Block_mid.clear();
		}
		else
		{
			Block_mid.push_back(*it);
		}
	}
	if (Block_mid.size() > 0)
	{
		Basic_Block b(Block_mid);
		BBs.push_back(b);
	}

	//connect blocks
	for (unsigned int i = 0; i < BBs.size(); i++)
	{
		BBs[i].count_usedef();
		BBs[i].peephole();
		BBs[i].peephole();
		BBs[i].peephole();
		BBs[i].peephole();
		if (BBs[i].get_first_oq().op == "SET")
		{
			for (unsigned int j = 0; j < BBs.size(); j++)
			{
				if (j == i)
				{
					continue;
				}
				if (BBs[j].get_last_oq().op == "GO" || BBs[j].get_last_oq().op == "BEQ" || BBs[j].get_last_oq().op == "BNE" || BBs[j].get_last_oq().op == "BL" 
					|| BBs[j].get_last_oq().op == "BS" || BBs[j].get_last_oq().op == "BLE" || BBs[j].get_last_oq().op == "BSE")
				{
					if (BBs[j].get_last_oq().res == BBs[i].get_first_oq().res)
					{
						if (find(BBs[i].pre.begin(), BBs[i].pre.end(), j) == BBs[i].pre.end())
						{
							BBs[i].pre.push_back(j);
						}
						if (find(BBs[j].next.begin(), BBs[j].next.end(), i) == BBs[j].next.end())
						{
							BBs[j].next.push_back(i);
						}
						
					}
				}
			}
		}
		if (BBs[i].get_last_oq().op == "GO")
		{
			for (unsigned int j = 0; j < BBs.size(); j++)
			{
				if (BBs[j].get_first_oq().op == "SET" && BBs[i].get_last_oq().res == BBs[j].get_first_oq().res)
				{
					if (find(BBs[i].next.begin(), BBs[i].next.end(), j) == BBs[i].next.end())
					{
						BBs[i].next.push_back(j);
					}
					if (find(BBs[j].pre.begin(), BBs[j].pre.end(), i) == BBs[j].pre.end())
					{
						BBs[j].pre.push_back(i);
					}
				}
			}
		}
		else if (BBs[i].get_last_oq().op != "END" && BBs[i].get_last_oq().op != "RET")
		{
			if (i + 1 < BBs.size() && find(BBs[i].next.begin(), BBs[i].next.end(), i+1) == BBs[i].next.end())
			{
				BBs[i].next.push_back(i+1);
			}
			if (i + 1 < BBs.size() && find(BBs[i+1].pre.begin(), BBs[i+1].pre.end(), i) == BBs[i+1].pre.end())
			{
				BBs[i+1].pre.push_back(i);
			};
		}
	}

	count_inout();


	for (unsigned int i = 0; i < BBs.size(); i++)
	{
		if (BBs[i].get_first_oq().op == "BEGIN")
		{
			i = _process(i);
		}
	}
}

vector<Quaternary> BB_Manager::get_op()
{
	vector<Quaternary> temp;
	for (unsigned int i = 0; i < BBs.size(); i++)
	{
		vector<Quaternary> q = BBs[i].get_origin_mid();
		for (std::vector<Quaternary>::iterator it = q.begin(); it != q.end(); ++it)
		{
			temp.push_back(*it);
		}
	}
	return temp;
}

int BB_Manager::find_reg(string funcname, string var)
{
	return func_reg_alloc[funcname][var];
}

void BB_Manager::count_inout()
{
	bool ischange = true;
	while (ischange)
	{
		ischange = false;
		for (unsigned int i = BBs.size() - 1; i > 0; i--)
		{
			//count out
			vector<string> temp(BBs[i].out);
			BBs[i].out.clear();
			for (unsigned j = 0; j < BBs[i].next.size(); j++)
			{
				for (unsigned k = 0; k < BBs[BBs[i].next[j]].in.size(); k++)
				{
					if (find(BBs[i].out.begin(), BBs[i].out.end(), BBs[BBs[i].next[j]].in[k]) == BBs[i].out.end())
					{
						BBs[i].out.push_back(BBs[BBs[i].next[j]].in[k]);
					}
				}
			}
			//change?
			if (BBs[i].out.size() != temp.size())
			{
				ischange = true;
			}
			else
			{
				for (unsigned int j = 0; j < BBs[i].out.size(); j++)
				{
					if (find(temp.begin(), temp.end(), BBs[i].out[j]) == temp.end())
					{
						ischange = true;
					}
				}
			}

			//count in
			vector<string> temp1(BBs[i].in);
			BBs[i].in.clear();
			for (unsigned int j = 0; j < BBs[i].out.size(); j++)
			{
				if (find(BBs[i].def.begin(), BBs[i].def.end(), BBs[i].out[j]) == BBs[i].def.end())
				{
					BBs[i].in.push_back(BBs[i].out[j]);
				}
			}
			for (unsigned int j = 0; j < BBs[i].use.size(); j++)
			{
				if (find(BBs[i].in.begin(), BBs[i].in.end(), BBs[i].use[j]) == BBs[i].in.end())
				{
					BBs[i].in.push_back(BBs[i].use[j]);
				}
			}
			//change?
			if (BBs[i].in.size() != temp1.size())
			{
				ischange = true;
			}
			else
			{
				for (unsigned int j = 0; j < BBs[i].in.size(); j++)
				{
					if (find(temp1.begin(), temp1.end(), BBs[i].in[j]) == temp1.end())
					{
						ischange = true;
					}
				}
			}
		}
	}
}

int BB_Manager::_process(int i)
{
	int index = 0;
	string funcname = BBs[i].get_first_oq().res;
	if (funcname == "main")
	{
		index = BBs.size() - 1;
	}
	else
	{
		for (unsigned int j = i; j < BBs.size(); j++)
		{
			if (BBs[j].get_last_oq().op == "END" && BBs[j].get_last_oq().res == funcname)
			{
				index = j;
				break;
			}
		}
	}

	map<string, int> reg_list;
	map<int, string> reg_list2;
	map<string, int> reg_alloc;
	int graph[100][100];
	int num = 0;
	memset(graph, 0, sizeof(graph));

	for (int j = i; j <= index; j++)
	{
		for (unsigned k = 0; k < BBs[j].in.size(); k++)
		{
			if (reg_list.find(BBs[j].in[k]) == reg_list.end())
			{
				reg_list.insert(pair<string, int>(BBs[j].in[k], num));
				reg_list2.insert(pair<int, string>(num, BBs[j].in[k]));
				num++;
			}
		}
		if (BBs[j].in.size() > 1)
		{
			for (unsigned k = 0; k < BBs[j].in.size(); k++)
			{
				for (unsigned l = 0; l < BBs[j].in.size(); l++)
				{
					graph[reg_list[BBs[j].in[k]]][reg_list[BBs[j].in[l]]] = 1;
					graph[reg_list[BBs[j].in[l]]][reg_list[BBs[j].in[k]]] = 1;
				}
			}
		}

		for (unsigned k = 0; k < BBs[j].out.size(); k++)
		{
			if (reg_list.find(BBs[j].out[k]) == reg_list.end())
			{
				reg_list.insert(pair<string, int>(BBs[j].out[k], num));
				reg_list2.insert(pair<int, string>(num, BBs[j].out[k]));
				num++;
			}
		}
		if (BBs[j].out.size() > 1)
		{
			for (unsigned k = 0; k < BBs[j].out.size(); k++)
			{
				for (unsigned l = 0; l < BBs[j].out.size(); l++)
				{
					graph[reg_list[BBs[j].out[k]]][reg_list[BBs[j].out[l]]] = 1;
					graph[reg_list[BBs[j].out[l]]][reg_list[BBs[j].out[k]]] = 1;
				}
			}
		}
	}

	int temp_num = num - 1;
	int temp_graph[100][100];
	memcpy(&temp_graph, &graph, sizeof(graph));
	vector<int> quene;
	vector<int> quene_in;
	while (temp_num > 0)
	{
		int edges[100];

		//count edges
		for (int i = 0; i < num; i++)
		{
			if (find(quene.begin(), quene.end(), i) == quene.end())
			{
				edges[i] = 0;
				for (int j = 0; j < num; j++)
				{
					if (j == i)
						continue;
					else
					{
						if (temp_graph[i][j] == 1)
						{
							edges[i]++;
						}
					}
				}
			}
			else
			{
				edges[i] = -1;
			}
		}
		bool has = false;
		int next = -1;
		for (int i = 0; i < num; i++)
		{
			if (edges[i] < regs_num && (find(quene.begin(), quene.end(), i) == quene.end() && find(quene_in.begin(), quene_in.end(), i) == quene_in.end()))
			{
				has = true;
				next = i;
				break;
			}
			else
			{
				if (find(quene_in.begin(), quene_in.end(), i) == quene_in.end())
				{
					next = i;
				}
			}
		}
		if (has)
		{
			quene.push_back(next);
		}
		else
		{
			quene_in.push_back(next);
			reg_alloc.insert(pair<string, int>(reg_list2[next], -1));
		}
		for (int i = 0; i < num; i++)
		{
			temp_graph[next][i] = 0;
			temp_graph[i][next] = 0;
		}
		temp_num--;
	}
	vector<int> al_in;
	if (num > 0)
	{
		bool last[100];
		for (int i = 0; i < num; i++)
		{
			last[i] = false;
		}
		for (unsigned int i = 0; i < quene.size(); i++)
		{
			last[quene[i]] = true;
		}
		for (int i = 0; i < num; i++)
		{
			if (!last[i])
			{
				reg_alloc.insert(pair<string, int>(reg_list2[i], 0));
				al_in.push_back(i);
			}
		}
	}
	for (int i = (int)quene.size()-1; i >= 0; i--)
	{
		bool flag = true;
		int reg = 0;
		while (flag)
		{
			bool can = true;
			for (unsigned int j = 0; j < al_in.size(); j++)
			{
				if (graph[al_in[j]][quene[i]] == 1 && reg_alloc[reg_list2[al_in[j]]] == reg)
				{
					can = false;
				}
			}
			if (can)
			{
				flag = false;
				reg_alloc.insert(pair<string, int>(reg_list2[quene[i]], reg));
				al_in.push_back(quene[i]);
			}
			reg++;
		}
	}

	func_reg_alloc.insert(pair<string, map<string, int>>(funcname, reg_alloc));
	return index;
}

void BB_Manager::Print_Block()
{
	ofstream fout;
	fout.open("Blocks.txt");
	int i = 0;
	for (std::vector<Basic_Block>::iterator it = BBs.begin(); it != BBs.end(); ++it)
	{
		fout << "----------------------------------------\n";
		fout << "Block " << i << "\npre: ";
		for (unsigned int j = 0; j < it->pre.size(); j++)
		{
			fout << it->pre[j] << " ";
		}
		fout << "\nnext: ";
		for (unsigned int j = 0; j < it->next.size(); j++)
		{
			fout << it->next[j] << " ";
		}
		fout << "\nin: ";
		for (unsigned int j = 0; j < it->in.size(); j++)
		{
			fout << it->in[j] << " ";
		}
		fout << "\nout: ";
		for (unsigned int j = 0; j < it->out.size(); j++)
		{
			fout << it->out[j] << " ";
		}
		fout << "\nuse: ";
		for (unsigned int j = 0; j < it->use.size(); j++)
		{
			fout << it->use[j] << " ";
		}
		fout << "\ndef: ";
		for (unsigned int j = 0; j < it->def.size(); j++)
		{
			fout << it->def[j] << " ";
		}
		fout << "\n";
		vector<Quaternary> q = it->get_origin_mid();
		for (std::vector<Quaternary>::iterator it = q.begin(); it != q.end(); ++it)
		{
			fout << it->op << "\t" << it->a << "\t" << it->b << "\t" << it->res << "\n";
		}
		i++;
		fout << "----------------------------------------\n";
	}
	fout.close();

	fout.open("reg_alloc.txt");
	for (unsigned int i = 0; i < BBs.size(); i++)
	{
		if (BBs[i].get_first_oq().op == "BEGIN")
		{
			string funcname = BBs[i].get_first_oq().res;
			fout << funcname << ":\n";
			map<string, map<string, int>>::iterator iter = func_reg_alloc.find(funcname);
			if (iter != func_reg_alloc.end()) //should like this  
			{
				map<string, int>::iterator it = iter->second.begin();
				while (it != iter->second.end())
				{
					fout << it->first << ": " << it->second << "\n";
					*it++;
				}
			}
		}
	}
	fout.close();
}

BB_Manager::~BB_Manager()
{
}
