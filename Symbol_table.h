#pragma once
#define MAX_ITEM 100
#define MAX_TABLE 20
#include <set>
#include <map>
#include <vector>

enum Type
{
	VAR_int,				//variable
	VAR_char,
	FUNC_V,					//header of void function
	FUNC_R_int,				//header of function with return value
	FUNC_R_char,
	PAR_int,				//parameter of a function
	PAR_char,
	STR_p
};

struct Item
{
	std::string name;
	Type type;
	int size;						//Array size,if it's not array,set 1.
	int address;					//record the address of global var and const, -1 if it is not global, -1000 means should be set
	bool isArray;
	bool isConst;
	int para_num;                   //number of a parameter in a function
	int	position;					//position in origin code
	int offset;
};

inline bool operator<(const Item& lhs, const Item& rhs)
{
	return lhs.name < rhs.name;
}

class Symbol_table
{
private:
	int item_number;
	std::vector<Item> global_var;
	std::map< std::string, std::vector<Item> > func_table;
public:
	void insert_global(std::string name, Type type, int size, int address, bool isArray, bool isConst, int para_num, int position, int offset);
	void insert_func(std::string func_name, std::string name, Type type, int size, int address, bool isArray, bool isConst, int para_num, int position, int offset);

	Item* glo_find(std::string name);
	Item* func_find(std::string func_name, std::string var_name);
	int find_offset(std::string func_name);

	void printTable();

	Symbol_table();
	~Symbol_table();
};

