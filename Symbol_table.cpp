#include "stdafx.h"
#include "Symbol_table.h"
#include <fstream>


void Symbol_table::insert_global(std::string name, Type type, int size, int address, bool isArray, bool isConst, int para_num, int position, int offset)
{
	Item item;
	item.name = name;
	item.type = type;
	item.size = size;
	item.address = address;
	item.isArray = isArray;
	item.isConst = isConst;
	item.para_num = para_num;
	item.position = position;
	item.offset = offset;
	global_var.push_back(item);
	//return 0;
}

void Symbol_table::insert_func(std::string func_name, std::string name, Type type, int size, int address, bool isArray, bool isConst, int para_num, int position, int offset)
{
	Item item;
	item.name = name;
	item.type = type;
	item.size = size;
	item.address = address;
	item.isArray = isArray;
	item.isConst = isConst;
	item.para_num = para_num;
	item.position = position;
	item.offset = offset;
	std::map<std::string, std::vector<Item>>::iterator iter;
	iter = func_table.find(func_name);
	if (iter != func_table.end())
	{
		iter->second.push_back(item);
	}
	else
	{
		std::vector<Item> s;
		s.push_back(item);
		std::string name = func_name;
		func_table.insert(pair<std::string, std::vector<Item>>(name, s));
	}

	//return 0;
}

Item* Symbol_table::glo_find(std::string name)
{
	//bool isFind = false;
	for (std::vector<Item>::iterator it = global_var.begin(); it != global_var.end(); ++it)
	{
		if (it->name == name)
		{
			Item item = *it;
			return &item;
		}
	}

	return nullptr;
}

Item* Symbol_table::func_find(std::string func_name, std::string var_name)
{
	std::map<std::string, std::vector<Item> >::iterator iter = func_table.find(func_name);
	if (iter != func_table.end()) //should like this  
	{
		std::vector<Item>::iterator it = iter->second.begin();
		while (it != iter->second.end())
		{
			if (it->name == var_name)
			{
				return &*it;
			}
			//std::cout << it->name << std::endl;
			*it++;
		}
	}

	return nullptr;
}

int Symbol_table::find_offset(std::string func_name)
{
	int of = 0;
	std::map<std::string, std::vector<Item> >::iterator iter = func_table.find(func_name);
	if (iter != func_table.end()) //should like this  
	{
		std::vector<Item>::iterator it = iter->second.begin();
		while (it != iter->second.end())
		{
			if (it->offset > of)
			{
				of = it->offset;
			}
			//std::cout << it->name << std::endl;
			*it++;
		}
		it = iter->second.end() - 1;
		if (it->size > 1)
		{
			of += it->size * 4;
		}
		else
		{
			of += 4;
		}
	}
	return of;
}

void Symbol_table::printTable()
{
	ofstream fout;
	fout.open("table.txt");
	fout << "Global Table:\n";
	fout << "name\t|type\t|size\t|address\t|isArray\t|isConst\t|para_num\t|position\t|offset\t|\n";
	for (std::vector<Item>::iterator it = global_var.begin(); it != global_var.end(); ++it)
	{
		fout << "|" << it->name << "\t|" << it->type << "\t\t|" << it->size << "\t\t|" << it->address << "\t\t|" << it->isArray << "\t\t|" << it->isConst << "\t\t|" << it->para_num << "\t\t|" << it->position << "\t\t|" << it->offset << "\t\t|\n";
	}
	fout << "\n\nFunction Table:\n\n";
	for (std::map<std::string, std::vector<Item> >::iterator iter = func_table.begin(); iter != func_table.end(); ++iter)
	{
		if (iter != func_table.end()) //should like this  
		{
			fout << "Function:  " << iter->first << " \n";
			fout << "name\t|type\t|size\t|address\t|isArray\t|isConst\t|para_num\t|position\t|offset\t|\n";
			std::vector<Item>::iterator it = iter->second.begin();
			while (it != iter->second.end())
			{
				fout << "|" << it->name << "\t\t|" << it->type << "\t\t|" << it->size << "\t\t|" << it->address << "\t\t|" << it->isArray << "\t\t|" << it->isConst << "\t\t|" << it->para_num << "\t\t|" << it->position << "\t\t|" << it->offset << "\t\t|\n";				//std::cout << it->name << std::endl;
				*it++;
			}
		}
		fout << "\n\n";
	}
	fout.close();
}

Symbol_table::Symbol_table()
{
}


Symbol_table::~Symbol_table()
{
}
