#include "stdafx.h"
#include "midcode.h"


void midcode::search_string()
{
	for (std::vector<Quaternary>::iterator it = code_mid.begin(); it != code_mid.end(); ++it)
	{
		if (it->op == "PD")
		{
			std::string s = it->a;
			s = s.substr(0, 7);
			if (s == ".String")
			{
				CODE << it->a << ": .asciiz  \"" << it->b << "\"\n";
			}
		}
	}
}

int midcode::get_temp(std::string var_name)
{
	std::map<std::string, std::string >::iterator iter = temp_var.find(var_name);
	int t = var_num % 10;
	//int func_offset = offset_of_func;
	if (iter != temp_var.end()) //find it
	{
		if (iter->second.c_str()[0] == 't')
		{
			return iter->second.c_str()[1] - '0';
		}
		else
		{
			if (temp_reg[t] == "")
			{
				CODE << "lw $t" << t << " -" << iter->second << "($sp)\n";
				temp_reg[t] = var_name;
				temp_var[var_name] = "t" + numTOstrs(t);
				var_num++;
				return t;
			}
			else
			{
				map<std::string, int>::iterator i_off = var_off.find(temp_reg[t]);
				if (i_off == var_off.end())
				{
					CODE << "sw $t" << t << " -" << offset_of_func << "($sp)\n";
					var_off[temp_reg[t]] = offset_of_func;
					temp_var[temp_reg[t]] = numTOstrs(offset_of_func);
					offset_of_func += 4;
				}
				else
				{
					CODE << "sw $t" << t << " -" << var_off[temp_reg[t]] << "($sp)\n";
					temp_var[temp_reg[t]] = numTOstrs(var_off[temp_reg[t]]);
				}
				//CODE << "sw $t" << t << " -" << func_offset << "($sp)\n";
				//temp_var[temp_reg[t]] = numTOstrs(func_offset);
				//func_offset += 4;
				//offset_stack.pop_back();
				//offset_stack.push_back(func_offset);
				CODE << "lw $t" << t << " -" << iter->second << "($sp)\n";
				temp_reg[t] = var_name;
				temp_var[var_name] = "t" + numTOstrs(t);
				var_num++;
				return t;
			}
		}
	}
	else
	{
		if (temp_reg[t] == "")
		{
			temp_reg[t].assign(var_name);
			temp_var[var_name] = "t" + numTOstrs(t);
			var_num++;
			return t;
		}
		else
		{
			map<std::string, int>::iterator i_off = var_off.find(temp_reg[t]);
			if (i_off == var_off.end())
			{
				CODE << "sw $t" << t << " -" << offset_of_func << "($sp)\n";
				var_off[temp_reg[t]] = offset_of_func;
				temp_var[temp_reg[t]] = numTOstrs(offset_of_func);
				offset_of_func += 4;
			}
			else
			{
				CODE << "sw $t" << t << " -" << var_off[temp_reg[t]] << "($sp)\n";
				temp_var[temp_reg[t]] = numTOstrs(var_off[temp_reg[t]]);
			}
			//CODE << "sw $t" << t << " -" << var_off[temp_reg[t]] << "($sp)\n";
			//var_off[var_name] = offset_of_func;
			//offset_of_func += 4;
			//offset_stack.pop_back();
			//offset_stack.push_back(func_offset);
			//CODE << "lw $t" << var_num % 10 << " " << iter->second << "($sp)\n";
			temp_reg[t] = var_name;
			temp_var[var_name] = "t" + numTOstrs(t);
			var_num++;
			return t;
		}
	}
	return -1;
}

int midcode::get_save(std::string var_name)
{
	std::map<std::string, std::string >::iterator iter = temp_var.find(var_name);
	int t = save_num % 8;
	if (table->func_find(func_name, var_name) == nullptr && table->glo_find(var_name) == nullptr)
	{
		error(2, var_name);
	}
	if (iter != temp_var.end()) //find it
	{
		if (iter->second.c_str()[0] == 's')
		{
			return iter->second.c_str()[1] - '0';
		}
		else
		{
			if (save_reg[t] == "")
			{
				if (table->func_find(func_name, var_name) != nullptr)
				{
					if (table->func_find(func_name, var_name)->offset == 0 && (table->func_find(func_name, var_name)->type == PAR_int || table->func_find(func_name, var_name)->type == PAR_char))
					{
						CODE << "move $s" << t << " $a" << table->func_find(func_name, var_name)->size << "\n";
					}
					else
						CODE << "lw $s" << t << " -" << table->func_find(func_name, var_name)->offset << "($sp)\n";
				}
				else
				{
					CODE << "lw $s" << t << " " << iter->second << "($gp)\n";
				}
				save_reg[t] = var_name;
				temp_var[var_name] = "s" + numTOstrs(t);
				save_num++;
				return t;
			}
			else
			{
				if (table->func_find(func_name, save_reg[t]) != nullptr)
				{
					if (table->func_find(func_name, save_reg[t])->offset == 0 && (table->func_find(func_name, save_reg[t])->type == PAR_int || table->func_find(func_name, save_reg[t])->type == PAR_char))
					{
						CODE << "move $a" << table->func_find(func_name, var_name)->size << " $t" << t << "\n";
					}
					else
					{
						int offset = table->func_find(func_name, save_reg[t])->offset;
						CODE << "sw $s" << t << " -" << offset << "($sp)\n";
					}
					temp_var[save_reg[t]] = numTOstrs(table->func_find(func_name, save_reg[t])->offset);
					if (table->func_find(func_name, var_name) != nullptr)
					{
						if (table->func_find(func_name, var_name)->offset == 0 && (table->func_find(func_name, var_name)->type == PAR_int || table->func_find(func_name, var_name)->type == PAR_char))
						{
							CODE << "move $s" << t << " $a" << table->func_find(func_name, var_name)->size << "\n";
						}
						else
							CODE << "lw $s" << t << " -" << table->func_find(func_name, var_name)->offset << "($sp)\n";
					}
					else
					{
						CODE << "lw $s" << t << " " << iter->second << "($gp)\n";
					}
					save_reg[t] = var_name;
					temp_var[var_name] = "s" + numTOstrs(t);
					save_num++;
					return t;
				}
				else
				{
					int offset = table->glo_find(save_reg[t])->address;
					CODE << "sw $s" << t << " " << offset << "($gp)\n";
					temp_var[save_reg[t]] = numTOstrs(offset);
					if (table->func_find(func_name, var_name) != nullptr)
					{
						CODE << "lw $s" << t << " -" << iter->second << "($sp)\n";
					}
					else
					{
						CODE << "lw $s" << t << " " << iter->second << "($gp)\n";
					}
					save_reg[t] = var_name;
					temp_var[var_name] = "s" + numTOstrs(t);
					save_num++;
					return t;
				}

			}
		}
	}
	else
	{
		if (save_reg[t] == "")
		{
			save_reg[t] = var_name;
			temp_var[var_name] = "s" + numTOstrs(t);
			save_num++;
			if (table->func_find(func_name, var_name) != nullptr)
			{
				if (table->func_find(func_name, var_name)->offset == 0 && (table->func_find(func_name, var_name)->type == PAR_int || table->func_find(func_name, var_name)->type == PAR_char))
				{
					CODE << "move $s" << t << " $a" << table->func_find(func_name, var_name)->size << "\n";
				}
				else
					CODE << "lw $s" << t << " -" << table->func_find(func_name, var_name)->offset << "($sp)\n";
			}
			else
			{
				CODE << "lw $s" << t << " " << table->glo_find(var_name)->address << "($gp)\n";
			}
			return t;
		}
		else
		{
			if (table->func_find(func_name, save_reg[t]) != nullptr)
			{
				if (table->func_find(func_name, save_reg[t])->offset == 0 && (table->func_find(func_name, save_reg[t])->type == PAR_int || table->func_find(func_name, save_reg[t])->type == PAR_char))
				{
					CODE << "move $a" << table->func_find(func_name, var_name)->size << " $t" << t << "\n";
				}
				else
				{
					int offset = table->func_find(func_name, save_reg[t])->offset;
					CODE << "sw $s" << t << " -" << offset << "($sp)\n";
				}
				temp_var[save_reg[t]] = numTOstrs(table->func_find(func_name, save_reg[t])->offset);
				if (table->func_find(func_name, var_name) != nullptr)
				{
					if (table->func_find(func_name, var_name)->offset == 0 && (table->func_find(func_name, var_name)->type == PAR_int || table->func_find(func_name, var_name)->type == PAR_char))
					{
						CODE << "move $s" << t << " $a" << table->func_find(func_name, var_name)->size << "\n";
					}
					else
						CODE << "lw $s" << t << " -" << table->func_find(func_name, var_name)->offset << "($sp)\n";
				}
				else
				{
					CODE << "lw $s" << t << " " << table->glo_find(var_name)->address << "($gp)\n";
				}
				save_reg[t] = var_name;
				temp_var[var_name] = "s" + numTOstrs(t);
				save_num++;
				return t;
			}
			else
			{
				int offset = table->glo_find(save_reg[t])->address;
				CODE << "sw $s" << t << " " << offset << "($gp)\n";
				temp_var[save_reg[t]] = numTOstrs(offset);
				if (table->func_find(func_name, var_name) != nullptr)
				{
					if (table->func_find(func_name, var_name)->offset == 0 && (table->func_find(func_name, var_name)->type == PAR_int || table->func_find(func_name, var_name)->type == PAR_char))
					{
						CODE << "move $s" << t << " $a" << table->func_find(func_name, var_name)->size << "\n";
					}
					else
						CODE << "lw $s" << t << " -" << table->func_find(func_name, var_name)->offset << "($sp)\n";
				}
				else
				{
					CODE << "lw $s" << t << " " << table->glo_find(var_name)->address << "($gp)\n";
				}
				save_reg[t] = var_name;
				temp_var[var_name] = "s" + numTOstrs(t);
				save_num++;
				return t;
			}
		}
	}
	return -1;
}

int midcode::get_save_op(std::string var_name)
{
	std::map<std::string, std::string >::iterator iter = temp_var.find(var_name);
	if (table->func_find(func_name, var_name) == nullptr && table->glo_find(var_name) == nullptr)
	{
		error(2, var_name);
	}
	int t = BBm->find_reg(func_name, var_name);
	if (t == -1)
	{
		t = 7;
	}
	if (iter != temp_var.end()) //find it
	{
		if (iter->second.c_str()[0] == 's')
		{
			return iter->second.c_str()[1] - '0';
		}
		else
		{
			if (save_reg[t] == "")
			{
				if (table->func_find(func_name, var_name) != nullptr)
				{
					if (table->func_find(func_name, var_name)->offset == 0 && (table->func_find(func_name, var_name)->type == PAR_int || table->func_find(func_name, var_name)->type == PAR_char))
					{
						CODE << "move $s" << t << " $a" << table->func_find(func_name, var_name)->size << "\n";
					}
					else
						CODE << "lw $s" << t << " -" << table->func_find(func_name, var_name)->offset << "($sp)\n";
				}
				else
				{
					CODE << "lw $s" << t << " " << iter->second << "($gp)\n";
				}
				save_reg[t] = var_name;
				temp_var[var_name] = "s" + numTOstrs(t);
				save_num++;
				return t;
			}
			else
			{
				if (table->func_find(func_name, save_reg[t]) != nullptr)
				{
					if (table->func_find(func_name, save_reg[t])->offset == 0 && (table->func_find(func_name, save_reg[t])->type == PAR_int || table->func_find(func_name, save_reg[t])->type == PAR_char))
					{
						CODE << "move $a" << table->func_find(func_name, var_name)->size << " $t" << t << "\n";
					}
					else
					{
						int offset = table->func_find(func_name, save_reg[t])->offset;
						CODE << "sw $s" << t << " -" << offset << "($sp)\n";
					}
					temp_var[save_reg[t]] = numTOstrs(table->func_find(func_name, save_reg[t])->offset);
					if (table->func_find(func_name, var_name) != nullptr)
					{
						if (table->func_find(func_name, var_name)->offset == 0 && (table->func_find(func_name, var_name)->type == PAR_int || table->func_find(func_name, var_name)->type == PAR_char))
						{
							CODE << "move $s" << t << " $a" << table->func_find(func_name, var_name)->size << "\n";
						}
						else
							CODE << "lw $s" << t << " -" << table->func_find(func_name, var_name)->offset << "($sp)\n";
					}
					else
					{
						CODE << "lw $s" << t << " " << iter->second << "($gp)\n";
					}
					save_reg[t] = var_name;
					temp_var[var_name] = "s" + numTOstrs(t);
					save_num++;
					return t;
				}
				else
				{
					int offset = table->glo_find(save_reg[t])->address;
					CODE << "sw $s" << t << " " << offset << "($gp)\n";
					temp_var[save_reg[t]] = numTOstrs(offset);
					if (table->func_find(func_name, var_name) != nullptr)
					{
						CODE << "lw $s" << t << " -" << iter->second << "($sp)\n";
					}
					else
					{
						CODE << "lw $s" << t << " " << iter->second << "($gp)\n";
					}
					save_reg[t] = var_name;
					temp_var[var_name] = "s" + numTOstrs(t);
					save_num++;
					return t;
				}

			}
		}
	}
	else
	{
		if (save_reg[t] == "")
		{
			save_reg[t] = var_name;
			temp_var[var_name] = "s" + numTOstrs(t);
			save_num++;
			if (table->func_find(func_name, var_name) != nullptr)
			{
				if (table->func_find(func_name, var_name)->offset == 0 && (table->func_find(func_name, var_name)->type == PAR_int || table->func_find(func_name, var_name)->type == PAR_char))
				{
					CODE << "move $s" << t << " $a" << table->func_find(func_name, var_name)->size << "\n";
				}
				else
					CODE << "lw $s" << t << " -" << table->func_find(func_name, var_name)->offset << "($sp)\n";
			}
			else
			{
				CODE << "lw $s" << t << " " << table->glo_find(var_name)->address << "($gp)\n";
			}
			return t;
		}
		else
		{
			if (table->func_find(func_name, save_reg[t]) != nullptr)
			{
				if (table->func_find(func_name, save_reg[t])->offset == 0 && (table->func_find(func_name, save_reg[t])->type == PAR_int || table->func_find(func_name, save_reg[t])->type == PAR_char))
				{
					CODE << "move $a" << table->func_find(func_name, var_name)->size << " $t" << t << "\n";
				}
				else
				{
					int offset = table->func_find(func_name, save_reg[t])->offset;
					CODE << "sw $s" << t << " -" << offset << "($sp)\n";
				}
				temp_var[save_reg[t]] = numTOstrs(table->func_find(func_name, save_reg[t])->offset);
				if (table->func_find(func_name, var_name) != nullptr)
				{
					if (table->func_find(func_name, var_name)->offset == 0 && (table->func_find(func_name, var_name)->type == PAR_int || table->func_find(func_name, var_name)->type == PAR_char))
					{
						CODE << "move $s" << t << " $a" << table->func_find(func_name, var_name)->size << "\n";
					}
					else
						CODE << "lw $s" << t << " -" << table->func_find(func_name, var_name)->offset << "($sp)\n";
				}
				else
				{
					CODE << "lw $s" << t << " " << table->glo_find(var_name)->address << "($gp)\n";
				}
				save_reg[t] = var_name;
				temp_var[var_name] = "s" + numTOstrs(t);
				save_num++;
				return t;
			}
			else
			{
				int offset = table->glo_find(save_reg[t])->address;
				CODE << "sw $s" << t << " " << offset << "($gp)\n";
				temp_var[save_reg[t]] = numTOstrs(offset);
				if (table->func_find(func_name, var_name) != nullptr)
				{
					if (table->func_find(func_name, var_name)->offset == 0 && (table->func_find(func_name, var_name)->type == PAR_int || table->func_find(func_name, var_name)->type == PAR_char))
					{
						CODE << "move $s" << t << " $a" << table->func_find(func_name, var_name)->size << "\n";
					}
					else
						CODE << "lw $s" << t << " -" << table->func_find(func_name, var_name)->offset << "($sp)\n";
				}
				else
				{
					CODE << "lw $s" << t << " " << table->glo_find(var_name)->address << "($gp)\n";
				}
				save_reg[t] = var_name;
				temp_var[var_name] = "s" + numTOstrs(t);
				save_num++;
				return t;
			}
		}
	}
	return -1;
}

void midcode::s_wri_del()
{
	for (std::map<std::string, std::string>::iterator iter = temp_var.begin(); iter != temp_var.end(); iter++)
	{
		std::string var_name = iter->first;
		if (var_name.c_str()[0] != '.')
		{
			if (iter->second.c_str()[0] == 's')
			{
				int t = iter->second.c_str()[1] - '0';
				if (table->func_find(func_name, var_name) != nullptr)
				{
					if (table->func_find(func_name, var_name)->offset == 0 && (table->func_find(func_name, var_name)->type == PAR_int || table->func_find(func_name, var_name)->type == PAR_char))
					{
						CODE << "move  $a" << table->func_find(func_name, var_name)->size << " $s" << t << "\n";
					}
					else
						CODE << "sw $s" << t << " -" << table->func_find(func_name, var_name)->offset << "($sp)\n";
				}
				else
				{
					CODE << "sw $s" << t << " " << table->glo_find(var_name)->address << "($gp)\n";
				}
				save_reg[t] = "";
				iter = temp_var.erase(iter);
				if (iter == temp_var.end())
				{
					break;
				}
				iter--;
			}
			else
			{
				continue;
			}
		}
	}
}

void midcode::t_wri_del()
{
	for (std::map<std::string, std::string>::iterator iter = temp_var.begin(); iter != temp_var.end(); iter++)
	{
		std::string var_name = iter->first;
		if (var_name.c_str()[0] == '.')
		{
			if (iter->second.c_str()[0] == 't')
			{
				/*int t = iter->second.c_str()[1] - '0';
				if (table->func_find(func_name, var_name) != nullptr)
				{
					if (table->func_find(func_name, var_name)->offset == 0 && (table->func_find(func_name, var_name)->type == PAR_int || table->func_find(func_name, var_name)->type == PAR_char))
					{
						CODE << "move  $a" << table->func_find(func_name, var_name)->size << " $s" << t << "\n";
					}
					else
						CODE << "sw $s" << t << " -" << table->func_find(func_name, var_name)->offset << "($sp)\n";
				}
				else
				{
					CODE << "sw $s" << t << " " << table->glo_find(var_name)->address << "($gp)\n";
				}
				save_reg[t] = "";*/
				int t = iter->second.c_str()[1] - '0';
				map<std::string, int>::iterator i_off = var_off.find(temp_reg[t]);
				if (i_off == var_off.end())
				{
					CODE << "sw $t" << t << " -" << offset_of_func << "($sp)\n";
					var_off[temp_reg[t]] = offset_of_func;
					temp_var[temp_reg[t]] = numTOstrs(offset_of_func);
					offset_of_func += 4;
				}
				else
				{
					CODE << "sw $t" << t << " -" << var_off[temp_reg[t]] << "($sp)\n";
					temp_var[temp_reg[t]] = numTOstrs(var_off[temp_reg[t]]);
				}
				temp_reg[t] = "";
			}
			else
			{
				continue;
			}
		}
	}
}

void midcode::s_wri()
{
	for (std::map<std::string, std::string>::iterator iter = temp_var.begin(); iter != temp_var.end(); iter++)
	{
		std::string var_name = iter->first;
		if (var_name.c_str()[0] != '.')
		{
			if (iter->second.c_str()[0] == 's')
			{
				int t = iter->second.c_str()[1] - '0';
				if (table->func_find(func_name, var_name) != nullptr)
				{
					continue;
				}
				else
				{
					CODE << "sw $s" << t << " " << table->glo_find(var_name)->address << "($gp)\n";
				}
			}
			else
			{
				continue;
			}
		}
	}
}

void midcode::s_del()
{
	for (std::map<std::string, std::string>::iterator iter = temp_var.begin(); iter != temp_var.end(); iter++)
	{
		std::string var_name = iter->first;
		if (var_name.c_str()[0] != '.')
		{
			if (iter->second.c_str()[0] == 's')
			{
				int t = iter->second.c_str()[1] - '0';
				if (table->func_find(func_name, var_name) != nullptr)
				{
					continue;
				}
				else
				{
					save_reg[t] = "";
					iter = temp_var.erase(iter);
					iter--;
				}
			}
			else
			{
				continue;
			}
		}
	}
}

void midcode::pop_stack()
{
	int regList[] = { 8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25 };

	if (table->glo_find(call_stack.back())->para_num > 0) {
		CODE << "addi $sp $sp 20\n";
		CODE << "lw $a0 0($sp)\n";
		CODE << "lw $a1 -4($sp)\n";
		CODE << "lw $a2 -8($sp)\n";
		CODE << "lw $a3 -12($sp)\n";
		CODE << "lw $ra -16($sp)\n";
	}
	else {
		CODE << "addi $sp $sp 4\n";
		CODE << "lw $ra 0($sp)\n";
	}
	CODE << "addi $sp $sp 72\n";
	for (int i = 0; i < 18; i++) {

		CODE << "lw " << " $" << regList[i] << " -" << i * 4 << "($sp)\n";
	}

}

void midcode::push_stack()
{
	int regList[] = { 8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25 };
	int i;
	CODE << "addi $sp $sp -72\n";
	MaxOff += 76;
	for (i = 0; i < 18; i++) {
		CODE << "sw " << " $" << regList[i] << " " << (17-i) * 4 << "($sp)\n";
	}
	CODE << "addi $sp $sp -4\n";
	if (table->glo_find(call_stack.back())->para_num > 0) {
		CODE << "sw $a0 0($sp)\n";
		CODE << "sw $a1 -4($sp)\n";
		CODE << "sw $a2 -8($sp)\n";
		CODE << "sw $a3 -12($sp)\n";
		CODE << "sw $ra -16($sp)\n";
		CODE << "addi $sp $sp -20\n";
		MaxOff += 20;
	}
	else {
		CODE << "sw $ra 0($sp)\n";
		CODE << "addi $sp $sp -4\n";
		MaxOff += 4;
	}
}

void midcode::load_para(int paras)
{
	int be = para_list.size() - paras;
	for (unsigned int i = be; i < para_list.size(); i++)
	{
		if (i - be > 3)
		{
			CODE << "sw $t" << get_temp(para_list[i]) << " -" << (i - 3) * 4 + MaxOff << "($sp)\n";
		}
		else
		{
			CODE << "move $a" << i - be << " $t" << get_temp(para_list[i]) << "\n";
		}
	}
	for (int i = 0; i < paras; i++)
	{
		para_list.pop_back();
	}
}

void midcode::clear_varTable()
{
	for (int i = 0; i < 10; i++)
	{
		temp_reg[i] = "";
	}
	for (int i = 0; i < 8; i++)
	{
		save_reg[i] = "";
	}
	temp_var.clear();
	var_off.clear();
	var_num = 0;
	save_num = 0;
}

void midcode::error(int type, std::string var_name)
{
	string reason = "unknown reason.";
	switch (type)
	{
	case 1:
		reason = "Number begin with 0";
		break;
	case 2:
		reason = "Unknown Symbol";
		break;
	case 3:
		reason = "Missing '";
		break;
	case 4:
		reason = "Missing \"";
		break;
	case 5:
		reason = "Unknown char";
		break;
	case 6:
		reason = "Unknown string";
		break;
	case 7:
		reason = "Missing =";
		break;
	case 11:
		reason = "Missing ;";
		break;
	case 12:
		reason = "Should be char or int";
		break;
	case 13:
		reason = "Should be ¡®const¡¯(not useful)";
		break;
	case 14:
		reason = "Missing }";
		break;
	case 15:
		reason = "Missing {";
		break;
	case 16:
		reason = "Missing (";
		break;
	case 17:
		reason = "Missing )";
		break;
	case 18:
		reason = "Should be identifier";
		break;
	case 19:
		reason = "Missing void";
		break;
	case 20:
		reason = "Missing main";
		break;
	case 21:
		reason = "Should be a assign op";
		break;
	case 22:
		reason = "This type should have a char";
		break;
	case 23:
		reason = "Missing ]";
		break;
	case 24:
		reason = "Missing [";
		break;
	case 25:
		reason = "Should be integer";
		break;
	case 26:
		reason = "Unmatch op";
		break;
	case 27:
		reason = "Unknown Symbol";
		break;
	case 28:
		reason = "Write format wrong";
		break;
	case 29:
		reason = "Missing :";
		break;
	case 30:
		reason = "Unknown factor";
		break;
	case 31:
		reason = "duplicate definition";
		break;
	case 32:
		reason = "duplicate parameter";
		break;
	case 33:
		reason = "should not assign to a const";
		break;
	case 34:
		reason = "unmatch number of parameters";
		break;
	case 35:
		reason = "void function should not return a value";
		break;
	default:
		break;
	}
	std::cout << "-------------------------------\n";
	std::cout << "Find error! E:\n" << type << ":" << reason << "\n";
	std::cout << "-------------------------------\n";
	system("pause");
}

void midcode::insert(string op, string a, string b, string res)
{
	Quaternary q;
	q.a = a;
	q.b = b;
	q.res = res;
	q.op = op;
	code_mid.push_back(q);
}

void midcode::gen_code(Symbol_table *table)
{
	//offset_stack.push_back(table->find_offset("main"));
	if (needop)
	{
		code_mid = vector<Quaternary>(BBm->get_op());
		cout << "Optimizing...\n";
		offset_of_func = table->find_offset("main");

		call_stack.push_back("main");
		this->table = table;
		CODE.open("aimcode.txt");
		gen_globaldata();
		CODE << ".text\n";
		for (std::vector<Quaternary>::iterator it = code_mid.begin(); it != code_mid.end(); it++)
		{
			if (it->op == "global" || it->op == "text")
			{
				if (it->op == "text")
				{
					CODE << "lui $gp 0x1001\n";
					CODE << "j main\n";
				}
				continue;
			}
			else if (it->op == "ASSIGN")
			{
				std::string from = it->a;
				std::string dst = it->res;
				std::string from_reg;
				std::string dst_reg;

				if ((table->func_find(func_name, dst) != nullptr && table->func_find(func_name, dst)->isConst) ||
					(table->func_find(func_name, dst) == nullptr && table->glo_find(dst) != nullptr &&
						(table->glo_find(dst)->isConst || table->glo_find(dst)->isArray || table->glo_find(dst)->type == FUNC_R_char || table->glo_find(dst)->type == FUNC_R_int || table->glo_find(dst)->type == FUNC_V)))
				{
					error(33, dst);
					continue;
				}

				int fromnum = false;
				int needsave = false;
				if (from.c_str()[0] == '.')
				{
					from_reg = "t" + numTOstrs(get_temp(from));
				}
				else
				{
					if (isdigit(from.c_str()[0]) || from.c_str()[0] == '-')
					{
						from_reg = from;
						fromnum = true;
					}
					else
					{
						int n = get_save_op(from);
						from_reg = "s" + numTOstrs(n);

					}
				}
				if (dst.c_str()[0] == '.')
				{
					dst_reg = "t" + numTOstrs(get_temp(dst));
				}
				else
				{
					int n = get_save_op(dst);
					dst_reg = "s" + numTOstrs(n);
					needsave = true;
				}
				if (fromnum)
					CODE << "li $" << dst_reg << " " << from_reg << "\n";
				else
					CODE << "move $" << dst_reg << " $" << from_reg << "\n";
				if (needsave)
				{
					if (table->func_find(func_name, dst) != nullptr)
					{
						if (table->func_find(func_name, dst)->offset == 0 && (table->func_find(func_name, dst)->type == PAR_int || table->func_find(func_name, dst)->type == PAR_char))
						{
							CODE << "move  $a" << table->func_find(func_name, dst)->size << " $" << dst_reg << "\n";
						}
						else
							CODE << "sw $" << dst_reg << " -" << table->func_find(func_name, dst)->offset << "($sp)\n";

					}
					else if (table->glo_find(dst) != nullptr)
					{
						CODE << "sw $" << dst_reg << " " << table->glo_find(dst)->address << "($gp)\n";
					}
					else
					{
						system("pause");
					}

				}
			}
			else if (it->op == "ASSFA")
			{
				std::string arr_name = it->a;
				std::string arr_idx = it->b;
				std::string dst = it->res;
				//int reg = get_save_op(arr_name);
				bool needsave = false;
				if ((table->func_find(func_name, dst) != nullptr && table->func_find(func_name, dst)->isConst) ||
					(table->func_find(func_name, dst) == nullptr && table->glo_find(dst) != nullptr &&
						(table->glo_find(dst)->isConst || table->glo_find(dst)->isArray || table->glo_find(dst)->type == FUNC_R_char || table->glo_find(dst)->type == FUNC_R_int || table->glo_find(dst)->type == FUNC_V)))
				{
					error(33, dst);
					continue;
				}
				if (table->func_find(func_name, arr_name) != nullptr && table->func_find(func_name, arr_name)->isArray)
				{
					CODE << "addi $k1 $sp " << table->func_find(func_name, arr_name)->offset << "\n";
					if (dst.c_str()[0] == '.')
					{
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							CODE << "lw $t" << get_temp(dst) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "lw $t" << get_temp(dst) << " 0($k1)\n";
						}
						else
						{
							int n = get_save_op(arr_idx);
							if (n == -1)
							{
								CODE << "mul $k0 $k0 4\n";
								CODE << "add $k1 $k1 $k0\n";
								CODE << "lw $t" << get_temp(dst) << " 0($k1)\n";
							}
							else
							{
								CODE << "mul $s" << n << " $s" << n << " 4\n";
								CODE << "add $k1 $k1 $s" << n << "\n";
								CODE << "lw $t" << get_temp(dst) << " 0($k1)\n";
							}

						}

					}
					else
					{
						needsave = true;
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							int n = get_save_op(dst);
							CODE << "lw $s" << n << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
							//CODE << "lw $s" << get_save_op(dst) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "lw $s" << get_save_op(dst) << " 0($k1)\n";
						}
						else
						{
							CODE << "mul $s" << get_save_op(arr_idx) << " $s" << get_save_op(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $s" << get_save_op(arr_idx) << "\n";
							CODE << "lw $s" << get_save_op(dst) << " 0($k1)\n";
						}
					}

				}
				else if (table->glo_find(arr_name) != nullptr && table->glo_find(arr_name)->isArray)
				{
					CODE << "addi $k1 $gp " << table->glo_find(arr_name)->address << "\n";
					if (dst.c_str()[0] == '.')
					{
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							CODE << "lw $t" << get_temp(dst) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "lw $t" << get_temp(dst) << " 0($k1)\n";
						}
						else
						{
							CODE << "mul $s" << get_save_op(arr_idx) << " $s" << get_save_op(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $s" << get_save_op(arr_idx) << "\n";
							CODE << "lw $t" << get_temp(dst) << " 0($k1)\n";
						}

					}
					else
					{
						needsave = true;
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							CODE << "lw $s" << get_save_op(dst) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "lw $s" << get_save_op(dst) << " 0($k1)\n";
						}
						else
						{
							CODE << "mul $s" << get_save_op(arr_idx) << " $s" << get_save_op(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $s" << get_save_op(arr_idx) << "\n";
							CODE << "lw $s" << get_save_op(dst) << " 0($k1)\n";
						}
					}
				}
				else
				{
					error(2, arr_name);
					continue;
				}
				if (needsave)
				{
					if (table->func_find(func_name, dst) != nullptr)
					{
						if (table->func_find(func_name, dst)->offset == 0 && (table->func_find(func_name, dst)->type == PAR_int || table->func_find(func_name, dst)->type == PAR_char))
						{
							CODE << "move  $a" << table->func_find(func_name, dst)->size << " $s" << get_save_op(dst) << "\n";
						}
						else
							CODE << "sw $s" << get_save_op(dst) << " -" << table->func_find(func_name, dst)->offset << "($sp)\n";

					}
					else
					{
						CODE << "sw $s" << get_save_op(dst) << " " << table->glo_find(dst)->address << "($gp)\n";
					}
				}
			}
			else if (it->op == "ASSTA")
			{
				std::string arr_name = it->res;
				std::string arr_idx = it->b;
				std::string res = it->a;
				//int reg = get_save_op(arr_name);
				if (table->func_find(func_name, arr_name) != nullptr && table->func_find(func_name, arr_name)->isArray)
				{
					CODE << "addi $k1 $sp " << table->func_find(func_name, arr_name)->offset << "\n";
					if (res.c_str()[0] == '.')
					{
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							CODE << "sw $t" << get_temp(res) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "sw $t" << get_temp(res) << " 0($k1)\n";
						}
						else
						{
							CODE << "mul $s" << get_save_op(arr_idx) << " $s" << get_save_op(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $s" << get_save_op(arr_idx) << "\n";
							CODE << "sw $t" << get_temp(res) << " 0($k1)\n";
						}

					}
					else
					{
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							CODE << "sw $s" << get_save_op(res) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "sw $s" << get_save_op(res) << " 0($k1)\n";
						}
						else
						{
							CODE << "mul $s" << get_save_op(arr_idx) << " $s" << get_save_op(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $s" << get_save_op(arr_idx) << "\n";
							CODE << "sw $s" << get_save_op(res) << " 0($k1)\n";
						}
					}

				}
				else if (table->glo_find(arr_name) != nullptr && table->glo_find(arr_name)->isArray)
				{
					CODE << "addi $k1 $gp " << table->glo_find(arr_name)->address << "\n";
					if (res.c_str()[0] == '.')
					{
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							CODE << "sw $t" << get_temp(res) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "sw $t" << get_temp(res) << " 0($k1)\n";
						}
						else
						{
							CODE << "mul $s" << get_save_op(arr_idx) << " $s" << get_save_op(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $s" << get_save_op(arr_idx) << "\n";
							CODE << "sw $t" << get_temp(res) << " 0($k1)\n";
						}

					}
					else
					{
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							CODE << "sw $s" << get_save_op(res) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "sw $s" << get_save_op(res) << " 0($k1)\n";
						}
						else
						{
							CODE << "mul $s" << get_save_op(arr_idx) << " $s" << get_save_op(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $s" << get_save_op(arr_idx) << "\n";
							CODE << "sw $s" << get_save_op(res) << " 0($k1)\n";
						}
					}
				}
				else
				{
					error(2, arr_name);
					continue;
				}
			}
			else if (it->op == "ADD")
			{
				std::string op1 = it->a;
				std::string op2 = it->b;
				std::string rst = it->res;
				if (op1.c_str()[0] == '.')
				{
					op1 = "t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "s" + numTOstrs(get_save_op(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "s" + numTOstrs(get_save_op(op2));
				}
				if (rst.c_str()[0] == '.')
				{
					rst = "t" + numTOstrs(get_temp(rst));
				}
				else
				{
					rst = "s" + numTOstrs(get_save_op(rst));
				}
				CODE << "add $" << rst << " $" << op1 << " $" << op2 << "\n";
			}
			else if (it->op == "SUB")
			{
				std::string op1 = it->a;
				std::string op2 = it->b;
				std::string rst = it->res;
				bool hasnum = false;
				//int num;
				if (op1.c_str()[0] == '.')
				{
					op1 = "t" + numTOstrs(get_temp(op1));
				}
				else if (isdigit(op1.c_str()[0]))
				{
					//num = strTOnum(op1);
					hasnum = true;
				}
				else
				{
					op1 = "s" + numTOstrs(get_save_op(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "s" + numTOstrs(get_save_op(op2));
				}
				if (rst.c_str()[0] == '.')
				{
					rst = "t" + numTOstrs(get_temp(rst));
				}
				else
				{
					rst = "s" + numTOstrs(get_save_op(rst));
				}
				if (hasnum)
					CODE << "sub $" << rst << " $0 $" << op2 << "\n";
				else
					CODE << "sub $" << rst << " $" << op1 << " $" << op2 << "\n";
			}
			else if (it->op == "MUL")
			{
				std::string op1 = it->a;
				std::string op2 = it->b;
				std::string rst = it->res;
				if (op1.c_str()[0] == '.')
				{
					op1 = "t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "s" + numTOstrs(get_save_op(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "s" + numTOstrs(get_save_op(op2));
				}
				if (rst.c_str()[0] == '.')
				{
					rst = "t" + numTOstrs(get_temp(rst));
				}
				else
				{
					rst = "s" + numTOstrs(get_save_op(rst));
				}
				CODE << "mul $" << rst << " $" << op1 << " $" << op2 << "\n";
			}
			else if (it->op == "DIV")
			{
				std::string op1 = it->a;
				std::string op2 = it->b;
				std::string rst = it->res;
				if (op1.c_str()[0] == '.')
				{
					op1 = "t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "s" + numTOstrs(get_save_op(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "s" + numTOstrs(get_save_op(op2));
				}
				if (rst.c_str()[0] == '.')
				{
					rst = "t" + numTOstrs(get_temp(rst));
				}
				else
				{
					rst = "s" + numTOstrs(get_save_op(rst));
				}
				CODE << "div $" << rst << " $" << op1 << " $" << op2 << "\n";
			}
			else if (it->op == "BEQ")
			{
				s_wri_del();
				t_wri_del();
				std::string op1 = it->a;
				std::string op2 = it->b;
				if (op1.c_str()[0] == '.')
				{
					op1 = "$t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "$s" + numTOstrs(get_save_op(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "$t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "$s" + numTOstrs(get_save_op(op2));
				}
				CODE << "beq " << op1 << " " << op2 << " " << it->res << "\n";
			}
			else if (it->op == "BNE")
			{
				s_wri_del();
				t_wri_del();
				std::string op1 = it->a;
				std::string op2 = it->b;
				if (op1.c_str()[0] == '.')
				{
					op1 = "$t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "$s" + numTOstrs(get_save_op(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "$t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "$s" + numTOstrs(get_save_op(op2));
				}
				CODE << "bne " << op1 << " " << op2 << " " << it->res << "\n";
			}
			else if (it->op == "BL")
			{
				s_wri_del();
				t_wri_del();
				std::string op1 = it->a;
				std::string op2 = it->b;
				if (op1.c_str()[0] == '.')
				{
					op1 = "$t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "$s" + numTOstrs(get_save_op(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "$t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "$s" + numTOstrs(get_save_op(op2));
				}
				CODE << "bgt " << op1 << " " << op2 << " " << it->res << "\n";
			}
			else if (it->op == "BS")
			{
				s_wri_del();
				t_wri_del();
				std::string op1 = it->a;
				std::string op2 = it->b;
				if (op1.c_str()[0] == '.')
				{
					op1 = "$t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "$s" + numTOstrs(get_save_op(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "$t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "$s" + numTOstrs(get_save_op(op2));
				}
				CODE << "blt " << op1 << " " << op2 << " " << it->res << "\n";
			}
			else if (it->op == "BLE")
			{
				s_wri_del();
				t_wri_del();
				std::string op1 = it->a;
				std::string op2 = it->b;
				if (op1.c_str()[0] == '.')
				{
					op1 = "$t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "$s" + numTOstrs(get_save_op(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "$t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "$s" + numTOstrs(get_save_op(op2));
				}
				CODE << "bge " << op1 << " " << op2 << " " << it->res << "\n";
			}
			else if (it->op == "BSE")
			{
				s_wri_del();
				t_wri_del();
				std::string op1 = it->a;
				std::string op2 = it->b;
				if (op1.c_str()[0] == '.')
				{
					op1 = "$t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "$s" + numTOstrs(get_save_op(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "$t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "$s" + numTOstrs(get_save_op(op2));
				}
				CODE << "ble " << op1 << " " << op2 << " " << it->res << "\n";
			}
			else if (it->op == "GO")
			{
				s_wri_del();
				t_wri_del();
				CODE << "j " << it->res << "\n";
			}
			else if (it->op == "SET")
			{
				s_wri_del();
				t_wri_del();
				CODE << it->res << ": \n";
			}
			else if (it->op == "BEGIN")
			{
				offset_of_func = table->find_offset(it->res);
				clear_varTable();
				call_stack.push_back(it->res);
				CODE << it->res << ": \n";
				func_name = it->res;
			}
			else if (it->op == "RET")
			{
				if (call_stack.back() == "main")
				{
					CODE << "j main_end\n";
					continue;
				}
				if (it->res == "")
				{
					s_wri();
					CODE << "jr $ra\n";
					//call_stack.pop_back();
				}
				else
				{
					std::string from = it->res;
					std::string from_reg = it->res;
					int fromnum = false;
					if (from.c_str()[0] == '.')
					{
						from_reg = "t" + numTOstrs(get_temp(from));
					}
					else
					{
						if (isdigit(from.c_str()[0]))
						{
							from_reg = from;
							fromnum = true;
						}
						else
						{
							from_reg = "s" + numTOstrs(get_save_op(from));
						}
					}
					if (fromnum)
						CODE << "li $v0 " << from_reg << "\n";
					else
						CODE << "move $v0 $" << from_reg << "\n";
					s_wri();
					CODE << "jr $ra\n";

				}
			}
			else if (it->op == "END")
			{
				s_wri();
				CODE << "jr $ra\n";
				call_stack.pop_back();
				clear_varTable();
				func_name = "main";
			}
			else if (it->op == "CALL")
			{
				if (table->glo_find(it->res) == nullptr)
				{
					error(27, it->res);
					continue;
				}
				CODE << "addi $sp $sp " << -(offset_of_func - 4) << "\n";
				MaxOff += (offset_of_func - 4);
				call_stack.push_back(it->res);
				push_stack();
				int paranum = table->glo_find(it->res)->para_num;
				if (paranum > para_list.size())
				{
					error(34, it->res);
					continue;
				}
				CODE << "addi $sp $sp " << MaxOff << "\n";
				load_para(paranum);
				CODE << "addi $sp $sp " << -MaxOff << "\n";
				MaxOff = 0;
				CODE << "jal " << it->res << "\n";
				s_del();
				//offset_stack.push_back(table->find_offset(it->res));

				pop_stack();
				//offset_stack.pop_back();
				call_stack.pop_back();
				CODE << "addi $sp $sp " << offset_of_func << "\n";
				if (it->b != "")
				{
					if (table->glo_find(it->res)->type == FUNC_V)
					{
						error(35, it->res);
						continue;
					}
					CODE << "move $t" << get_temp(it->b) << " $v0\n";
				}

			}
			else if (it->op == "PUSH")
			{
				para_list.push_back(it->res);
				para_num++;
			}
			else if (it->op == "PD")
			{
				if (it->a != "")
				{
					CODE << "move $k0 $a0\n";
					CODE << "la $a0 " << it->a << "\n";
					CODE << "li $v0 4 \n";
					CODE << "syscall\n";
					CODE << "move $a0 $k0\n";
				}
				else if (it->b == "c")
				{
					CODE << "move $k0 $a0\n";
					CODE << "li $v0 11 \n";
					CODE << "move $a0 $t" << get_temp(it->res) << " \n";
					CODE << "syscall\n";
					CODE << "move $a0 $k0\n";
				}
				else
				{
					CODE << "move $k0 $a0\n";
					//CODE << "la $a0 " << it->a << "\n";
					//Item* item1 = table->func_find(func_name, it->res);
					//Item* item2 = table->glo_find(it->res);
					//if ((item1 != nullptr && (item1->type == VAR_char|| item1->type == PAR_char)) || (item2 != nullptr && (item2->type == VAR_char || item2->type == PAR_char)))
					//{
					//	CODE << "li $v0 4 \n";
					//}
					//else
					//{
					//	CODE << "li $v0 1 \n";
					//	move << "$a0 $t"
					//}

					CODE << "li $v0 1 \n";
					CODE << "move $a0 $t" << get_temp(it->res) << " \n";
					CODE << "syscall\n";
					CODE << "move $a0 $k0\n";
				}
			}
			else if (it->op == "RD")
			{
				if (table->func_find(func_name, it->res) != nullptr)
				{
					if (table->func_find(func_name, it->res)->type == VAR_int || table->func_find(func_name, it->res)->type == PAR_int)
					{
						CODE << "li $v0 5 \n";
						CODE << "syscall\n";
						CODE << "move $s" << get_save_op(it->res) << " $v0\n";
					}
					else
					{
						CODE << "li $v0 12 \n";
						CODE << "syscall\n";
						CODE << "move $s" << get_save_op(it->res) << " $v0\n";
					}
				}
				else if (table->glo_find(it->res) != nullptr)
				{
					if (table->glo_find(it->res)->type == VAR_int || table->glo_find(it->res)->type == PAR_int)
					{
						CODE << "li $v0 5 \n";
						CODE << "syscall\n";
						CODE << "move $s" << get_save_op(it->res) << " $v0\n";
					}
					else
					{
						CODE << "li $v0 12 \n";
						CODE << "syscall\n";
						CODE << "move $s" << get_save_op(it->res) << " $v0\n";
					}
				}
				else
				{
					error(2, it->res);
					continue;
				}
			}
			else if (it->op == "const")
			{
				//int value;
				if (it->a == "int")
				{
					int reg = get_save_op(it->res);
					CODE << "li $s" << reg << " " << it->b << "\n";
					CODE << "sw $s" << reg << " -" << table->func_find(call_stack.back(), it->res)->offset << "($sp)\n";

				}
				else if (it->a == "char")
				{
					int reg = get_save_op(it->res);
					int num = it->b.c_str()[0];
					CODE << "li $s" << reg << " " << num << "\n";
					CODE << "sw $s" << reg << " -" << table->func_find(call_stack.back(), it->res)->offset << "($sp)\n";
				}
			}
		}


		CODE << "main_end:\n";
		CODE.close();
	}
	else
	{
		//code_mid.swap(BBm->get_op());

		offset_of_func = table->find_offset("main");

		call_stack.push_back("main");
		this->table = table;
		CODE.open("aimcode.txt");
		gen_globaldata();
		CODE << ".text\n";
		for (std::vector<Quaternary>::iterator it = code_mid.begin(); it != code_mid.end(); it++)
		{
			if (it->op == "global" || it->op == "text")
			{
				if (it->op == "text")
				{
					CODE << "lui $gp 0x1001\n";
					CODE << "j main\n";
				}
				continue;
			}
			else if (it->op == "ASSIGN")
			{
				std::string from = it->a;
				std::string dst = it->res;
				std::string from_reg;
				std::string dst_reg;

				if ((table->func_find(func_name, dst) != nullptr && table->func_find(func_name, dst)->isConst) ||
					(table->func_find(func_name, dst) == nullptr && table->glo_find(dst) != nullptr &&
						(table->glo_find(dst)->isConst || table->glo_find(dst)->isArray || table->glo_find(dst)->type == FUNC_R_char || table->glo_find(dst)->type == FUNC_R_int || table->glo_find(dst)->type == FUNC_V)))
				{
					error(33, dst);
					continue;
				}

				int fromnum = false;
				int needsave = false;
				if (from.c_str()[0] == '.')
				{
					from_reg = "t" + numTOstrs(get_temp(from));
				}
				else
				{
					if (isdigit(from.c_str()[0]) || from.c_str()[0] == '-')
					{
						from_reg = from;
						fromnum = true;
					}
					else
					{
						from_reg = "s" + numTOstrs(get_save(from));
					}
				}
				if (dst.c_str()[0] == '.')
				{
					dst_reg = "t" + numTOstrs(get_temp(dst));
				}
				else
				{
					dst_reg = "s" + numTOstrs(get_save(dst));
					needsave = true;
				}
				if (fromnum)
					CODE << "li $" << dst_reg << " " << from_reg << "\n";
				else
					CODE << "move $" << dst_reg << " $" << from_reg << "\n";
				if (needsave)
				{
					if (table->func_find(func_name, dst) != nullptr)
					{
						if (table->func_find(func_name, dst)->offset == 0 && (table->func_find(func_name, dst)->type == PAR_int || table->func_find(func_name, dst)->type == PAR_char))
						{
							CODE << "move  $a" << table->func_find(func_name, dst)->size << " $" << dst_reg << "\n";
						}
						else
							CODE << "sw $" << dst_reg << " -" << table->func_find(func_name, dst)->offset << "($sp)\n";

					}
					else if (table->glo_find(dst) != nullptr)
					{
						CODE << "sw $" << dst_reg << " " << table->glo_find(dst)->address << "($gp)\n";
					}
					else
					{
						system("pause");
					}

				}
			}
			else if (it->op == "ASSFA")
			{
				std::string arr_name = it->a;
				std::string arr_idx = it->b;
				std::string dst = it->res;
				//int reg = get_save(arr_name);
				bool needsave = false;
				if ((table->func_find(func_name, dst) != nullptr && table->func_find(func_name, dst)->isConst) ||
					(table->func_find(func_name, dst) == nullptr && table->glo_find(dst) != nullptr &&
						(table->glo_find(dst)->isConst || table->glo_find(dst)->isArray || table->glo_find(dst)->type == FUNC_R_char || table->glo_find(dst)->type == FUNC_R_int || table->glo_find(dst)->type == FUNC_V)))
				{
					error(33, dst);
					continue;
				}
				if (table->func_find(func_name, arr_name) != nullptr && table->func_find(func_name, arr_name)->isArray)
				{
					CODE << "addi $k1 $sp " << table->func_find(func_name, arr_name)->offset << "\n";
					if (dst.c_str()[0] == '.')
					{
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							CODE << "lw $t" << get_temp(dst) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "lw $t" << get_temp(dst) << " 0($k1)\n";
						}
						else
						{
							CODE << "mul $s" << get_save(arr_idx) << " $s" << get_save(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $s" << get_save(arr_idx) << "\n";
							CODE << "lw $t" << get_temp(dst) << " 0($k1)\n";
						}

					}
					else
					{
						needsave = true;
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							CODE << "lw $s" << get_save(dst) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "lw $s" << get_save(dst) << " 0($k1)\n";
						}
						else
						{
							CODE << "mul $s" << get_save(arr_idx) << " $s" << get_save(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $s" << get_save(arr_idx) << "\n";
							CODE << "lw $s" << get_save(dst) << " 0($k1)\n";
						}
					}

				}
				else if (table->glo_find(arr_name) != nullptr && table->glo_find(arr_name)->isArray)
				{
					CODE << "addi $k1 $gp " << table->glo_find(arr_name)->address << "\n";
					if (dst.c_str()[0] == '.')
					{
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							CODE << "lw $t" << get_temp(dst) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "lw $t" << get_temp(dst) << " 0($k1)\n";
						}
						else
						{
							CODE << "mul $s" << get_save(arr_idx) << " $s" << get_save(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $s" << get_save(arr_idx) << "\n";
							CODE << "lw $t" << get_temp(dst) << " 0($k1)\n";
						}

					}
					else
					{
						needsave = true;
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							CODE << "lw $s" << get_save(dst) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "lw $s" << get_save(dst) << " 0($k1)\n";
						}
						else
						{
							CODE << "mul $s" << get_save(arr_idx) << " $s" << get_save(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $s" << get_save(arr_idx) << "\n";
							CODE << "lw $s" << get_save(dst) << " 0($k1)\n";
						}
					}
				}
				else
				{
					error(2, arr_name);
					continue;
				}
				if (needsave)
				{
					if (table->func_find(func_name, dst) != nullptr)
					{
						if (table->func_find(func_name, dst)->offset == 0 && (table->func_find(func_name, dst)->type == PAR_int || table->func_find(func_name, dst)->type == PAR_char))
						{
							CODE << "move  $a" << table->func_find(func_name, dst)->size << " $s" << get_save(dst) << "\n";
						}
						else
							CODE << "sw $s" << get_save(dst) << " -" << table->func_find(func_name, dst)->offset << "($sp)\n";

					}
					else
					{
						CODE << "sw $s" << get_save(dst) << " " << table->glo_find(dst)->address << "($gp)\n";
					}
				}
			}
			else if (it->op == "ASSTA")
			{
				std::string arr_name = it->res;
				std::string arr_idx = it->b;
				std::string res = it->a;
				//int reg = get_save(arr_name);
				if (table->func_find(func_name, arr_name) != nullptr && table->func_find(func_name, arr_name)->isArray)
				{
					CODE << "addi $k1 $sp " << table->func_find(func_name, arr_name)->offset << "\n";
					if (res.c_str()[0] == '.')
					{
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							CODE << "sw $t" << get_temp(res) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "sw $t" << get_temp(res) << " 0($k1)\n";
						}
						else
						{
							CODE << "mul $s" << get_save(arr_idx) << " $s" << get_save(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $s" << get_save(arr_idx) << "\n";
							CODE << "sw $t" << get_temp(res) << " 0($k1)\n";
						}

					}
					else
					{
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							CODE << "sw $s" << get_save(res) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "sw $s" << get_save(res) << " 0($k1)\n";
						}
						else
						{
							CODE << "mul $s" << get_save(arr_idx) << " $s" << get_save(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $s" << get_save(arr_idx) << "\n";
							CODE << "sw $s" << get_save(res) << " 0($k1)\n";
						}
					}

				}
				else if (table->glo_find(arr_name) != nullptr && table->glo_find(arr_name)->isArray)
				{
					CODE << "addi $k1 $gp " << table->glo_find(arr_name)->address << "\n";
					if (res.c_str()[0] == '.')
					{
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							CODE << "sw $t" << get_temp(res) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "sw $t" << get_temp(res) << " 0($k1)\n";
						}
						else
						{
							CODE << "mul $s" << get_save(arr_idx) << " $s" << get_save(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $s" << get_save(arr_idx) << "\n";
							CODE << "sw $t" << get_temp(res) << " 0($k1)\n";
						}

					}
					else
					{
						if (isdigit(arr_idx.c_str()[0]) || arr_idx.c_str()[0] == '-')
						{
							CODE << "sw $s" << get_save(res) << " " << strTOnum(arr_idx) * 4 << "($k1)\n";
						}
						else if (arr_idx.c_str()[0] == '.')
						{
							CODE << "mul $t" << get_temp(arr_idx) << " $t" << get_temp(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $t" << get_temp(arr_idx) << "\n";
							CODE << "sw $s" << get_save(res) << " 0($k1)\n";
						}
						else
						{
							CODE << "mul $s" << get_save(arr_idx) << " $s" << get_save(arr_idx) << " 4\n";
							CODE << "add $k1 $k1 $s" << get_save(arr_idx) << "\n";
							CODE << "sw $s" << get_save(res) << " 0($k1)\n";
						}
					}
				}
				else
				{
					error(2, arr_name);
					continue;
				}
			}
			else if (it->op == "ADD")
			{
				std::string op1 = it->a;
				std::string op2 = it->b;
				std::string rst = it->res;
				if (op1.c_str()[0] == '.')
				{
					op1 = "t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "s" + numTOstrs(get_save(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "s" + numTOstrs(get_save(op2));
				}
				if (rst.c_str()[0] == '.')
				{
					rst = "t" + numTOstrs(get_temp(rst));
				}
				else
				{
					rst = "s" + numTOstrs(get_save(rst));
				}
				CODE << "add $" << rst << " $" << op1 << " $" << op2 << "\n";
			}
			else if (it->op == "SUB")
			{
				std::string op1 = it->a;
				std::string op2 = it->b;
				std::string rst = it->res;
				bool hasnum = false;
				//int num;
				if (op1.c_str()[0] == '.')
				{
					op1 = "t" + numTOstrs(get_temp(op1));
				}
				else if (isdigit(op1.c_str()[0]))
				{
					//num = strTOnum(op1);
					hasnum = true;
				}
				else
				{
					op1 = "s" + numTOstrs(get_save(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "s" + numTOstrs(get_save(op2));
				}
				if (rst.c_str()[0] == '.')
				{
					rst = "t" + numTOstrs(get_temp(rst));
				}
				else
				{
					rst = "s" + numTOstrs(get_save(rst));
				}
				if (hasnum)
					CODE << "sub $" << rst << " $0 $" << op2 << "\n";
				else
					CODE << "sub $" << rst << " $" << op1 << " $" << op2 << "\n";
			}
			else if (it->op == "MUL")
			{
				std::string op1 = it->a;
				std::string op2 = it->b;
				std::string rst = it->res;
				if (op1.c_str()[0] == '.')
				{
					op1 = "t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "s" + numTOstrs(get_save(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "s" + numTOstrs(get_save(op2));
				}
				if (rst.c_str()[0] == '.')
				{
					rst = "t" + numTOstrs(get_temp(rst));
				}
				else
				{
					rst = "s" + numTOstrs(get_save(rst));
				}
				CODE << "mul $" << rst << " $" << op1 << " $" << op2 << "\n";
			}
			else if (it->op == "DIV")
			{
				std::string op1 = it->a;
				std::string op2 = it->b;
				std::string rst = it->res;
				if (op1.c_str()[0] == '.')
				{
					op1 = "t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "s" + numTOstrs(get_save(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "s" + numTOstrs(get_save(op2));
				}
				if (rst.c_str()[0] == '.')
				{
					rst = "t" + numTOstrs(get_temp(rst));
				}
				else
				{
					rst = "s" + numTOstrs(get_save(rst));
				}
				CODE << "div $" << rst << " $" << op1 << " $" << op2 << "\n";
			}
			else if (it->op == "BEQ")
			{
				s_wri_del();
				t_wri_del();
				std::string op1 = it->a;
				std::string op2 = it->b;
				if (op1.c_str()[0] == '.')
				{
					op1 = "$t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "$s" + numTOstrs(get_save(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "$t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "$s" + numTOstrs(get_save(op2));
				}
				CODE << "beq " << op1 << " " << op2 << " " << it->res << "\n";
			}
			else if (it->op == "BNE")
			{
				s_wri_del();
				t_wri_del();
				std::string op1 = it->a;
				std::string op2 = it->b;
				if (op1.c_str()[0] == '.')
				{
					op1 = "$t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "$s" + numTOstrs(get_save(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "$t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "$s" + numTOstrs(get_save(op2));
				}
				CODE << "bne " << op1 << " " << op2 << " " << it->res << "\n";
			}
			else if (it->op == "BL")
			{
				s_wri_del();
				t_wri_del();
				std::string op1 = it->a;
				std::string op2 = it->b;
				if (op1.c_str()[0] == '.')
				{
					op1 = "$t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "$s" + numTOstrs(get_save(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "$t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "$s" + numTOstrs(get_save(op2));
				}
				CODE << "bgt " << op1 << " " << op2 << " " << it->res << "\n";
			}
			else if (it->op == "BS")
			{
				s_wri_del();
				t_wri_del();
				std::string op1 = it->a;
				std::string op2 = it->b;
				if (op1.c_str()[0] == '.')
				{
					op1 = "$t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "$s" + numTOstrs(get_save(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "$t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "$s" + numTOstrs(get_save(op2));
				}
				CODE << "blt " << op1 << " " << op2 << " " << it->res << "\n";
			}
			else if (it->op == "BLE")
			{
				s_wri_del();
				t_wri_del();
				std::string op1 = it->a;
				std::string op2 = it->b;
				if (op1.c_str()[0] == '.')
				{
					op1 = "$t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "$s" + numTOstrs(get_save(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "$t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "$s" + numTOstrs(get_save(op2));
				}
				CODE << "bge " << op1 << " " << op2 << " " << it->res << "\n";
			}
			else if (it->op == "BSE")
			{
				s_wri_del();
				t_wri_del();
				std::string op1 = it->a;
				std::string op2 = it->b;
				if (op1.c_str()[0] == '.')
				{
					op1 = "$t" + numTOstrs(get_temp(op1));
				}
				else
				{
					op1 = "$s" + numTOstrs(get_save(op1));
				}
				if (op2.c_str()[0] == '.')
				{
					op2 = "$t" + numTOstrs(get_temp(op2));
				}
				else
				{
					op2 = "$s" + numTOstrs(get_save(op2));
				}
				CODE << "ble " << op1 << " " << op2 << " " << it->res << "\n";
			}
			else if (it->op == "GO")
			{
				s_wri_del();
				t_wri_del();
				CODE << "j " << it->res << "\n";
			}
			else if (it->op == "SET")
			{
				s_wri_del();
				t_wri_del();
				CODE << it->res << ": \n";
			}
			else if (it->op == "BEGIN")
			{
				offset_of_func = table->find_offset(it->res);
				clear_varTable();
				call_stack.push_back(it->res);
				CODE << it->res << ": \n";
				func_name = it->res;
			}
			else if (it->op == "RET")
			{
				if (call_stack.back() == "main")
				{
					CODE << "j main_end\n";
					continue;
				}
				if (it->res == "")
				{
					s_wri();
					CODE << "jr $ra\n";
					//call_stack.pop_back();
				}
				else
				{
					std::string from = it->res;
					std::string from_reg = it->res;
					int fromnum = false;
					if (from.c_str()[0] == '.')
					{
						from_reg = "t" + numTOstrs(get_temp(from));
					}
					else
					{
						if (isdigit(from.c_str()[0]))
						{
							from_reg = from;
							fromnum = true;
						}
						else
						{
							from_reg = "s" + numTOstrs(get_save(from));
						}
					}
					if (fromnum)
						CODE << "li $v0 " << from_reg << "\n";
					else
						CODE << "move $v0 $" << from_reg << "\n";
					s_wri();
					CODE << "jr $ra\n";

				}
			}
			else if (it->op == "END")
			{
				s_wri();
				CODE << "jr $ra\n";
				call_stack.pop_back();
				clear_varTable();
				func_name = "main";
			}
			else if (it->op == "CALL")
			{
				if (table->glo_find(it->res) == nullptr)
				{
					error(27, it->res);
					continue;
				}
				CODE << "addi $sp $sp " << -(offset_of_func - 4) << "\n";
				MaxOff += (offset_of_func - 4);
				call_stack.push_back(it->res);
				push_stack();
				int paranum = table->glo_find(it->res)->para_num;
				if (paranum > para_list.size())
				{
					error(34, it->res);
					continue;
				}
				CODE << "addi $sp $sp " << MaxOff << "\n";
				load_para(paranum);
				CODE << "addi $sp $sp " << -MaxOff << "\n";
				MaxOff = 0;
				CODE << "jal " << it->res << "\n";
				s_del();
				//offset_stack.push_back(table->find_offset(it->res));

				pop_stack();
				//offset_stack.pop_back();
				call_stack.pop_back();
				CODE << "addi $sp $sp " << offset_of_func << "\n";
				if (it->b != "")
				{
					if (table->glo_find(it->res)->type == FUNC_V)
					{
						error(35, it->res);
						continue;
					}
					CODE << "move $t" << get_temp(it->b) << " $v0\n";
				}

			}
			else if (it->op == "PUSH")
			{
				para_list.push_back(it->res);
				para_num++;
			}
			else if (it->op == "PD")
			{
				if (it->a != "")
				{
					CODE << "move $k0 $a0\n";
					CODE << "la $a0 " << it->a << "\n";
					CODE << "li $v0 4 \n";
					CODE << "syscall\n";
					CODE << "move $a0 $k0\n";
				}
				else if (it->b == "c")
				{
					CODE << "move $k0 $a0\n";
					CODE << "li $v0 11 \n";
					CODE << "move $a0 $t" << get_temp(it->res) << " \n";
					CODE << "syscall\n";
					CODE << "move $a0 $k0\n";
				}
				else
				{
					CODE << "move $k0 $a0\n";
					//CODE << "la $a0 " << it->a << "\n";
					//Item* item1 = table->func_find(func_name, it->res);
					//Item* item2 = table->glo_find(it->res);
					//if ((item1 != nullptr && (item1->type == VAR_char|| item1->type == PAR_char)) || (item2 != nullptr && (item2->type == VAR_char || item2->type == PAR_char)))
					//{
					//	CODE << "li $v0 4 \n";
					//}
					//else
					//{
					//	CODE << "li $v0 1 \n";
					//	move << "$a0 $t"
					//}

					CODE << "li $v0 1 \n";
					CODE << "move $a0 $t" << get_temp(it->res) << " \n";
					CODE << "syscall\n";
					CODE << "move $a0 $k0\n";
				}
			}
			else if (it->op == "RD")
			{
				if (table->func_find(func_name, it->res) != nullptr)
				{
					if (table->func_find(func_name, it->res)->type == VAR_int || table->func_find(func_name, it->res)->type == PAR_int)
					{
						CODE << "li $v0 5 \n";
						CODE << "syscall\n";
						CODE << "move $s" << get_save(it->res) << " $v0\n";
					}
					else
					{
						CODE << "li $v0 12 \n";
						CODE << "syscall\n";
						CODE << "move $s" << get_save(it->res) << " $v0\n";
					}
				}
				else if (table->glo_find(it->res) != nullptr)
				{
					if (table->glo_find(it->res)->type == VAR_int || table->glo_find(it->res)->type == PAR_int)
					{
						CODE << "li $v0 5 \n";
						CODE << "syscall\n";
						CODE << "move $s" << get_save(it->res) << " $v0\n";
					}
					else
					{
						CODE << "li $v0 12 \n";
						CODE << "syscall\n";
						CODE << "move $s" << get_save(it->res) << " $v0\n";
					}
				}
				else
				{
					error(2, it->res);
					continue;
				}
			}
			else if (it->op == "const")
			{
				//int value;
				if (it->a == "int")
				{
					int reg = get_save(it->res);
					CODE << "li $s" << reg << " " << it->b << "\n";
					CODE << "sw $s" << reg << " -" << table->func_find(call_stack.back(), it->res)->offset << "($sp)\n";

				}
				else if (it->a == "char")
				{
					int reg = get_save(it->res);
					int num = it->b.c_str()[0];
					CODE << "li $s" << reg << " " << num << "\n";
					CODE << "sw $s" << reg << " -" << table->func_find(call_stack.back(), it->res)->offset << "($sp)\n";
				}
			}
		}


		CODE << "main_end:\n";
		CODE.close();
	}

	cout << "Compile end.\n";
}

void midcode::gen_globaldata()
{
	CODE << ".data\n";
	for (std::vector<Quaternary>::iterator it = code_mid.begin(); it != code_mid.end(); it++)
	{
		if (it->op == "text")
		{
			search_string();
			return;
		}
		if (it->op == "const")
		{
			if (it->a == "int")
			{
				CODE << it->res << ": .word " << it->b << "\n";
				it = code_mid.erase(it);
				it--;
			}
			else if (it->a == "char")
			{
				CODE << it->res << ": .word '" << it->b << "' \n";
				it = code_mid.erase(it);
				it--;
			}
		}
		else if (it->op == "int")
		{
			if (it->a != "")
			{
				CODE << it->res << ": .space " << strTOnum(it->a) * 4 << "\n";
				it = code_mid.erase(it);
				it--;
			}
			else
			{
				CODE << it->res << ": .word 0\n";
				it = code_mid.erase(it);
				it--;
			}
		}
		else if (it->op == "char")
		{
			if (it->a != "")
			{
				CODE << it->res << ": .space " << strTOnum(it->a) * 4 << "\n";
				it = code_mid.erase(it);
				it--;
			}
			else
			{
				CODE << it->res << ": .word '0'\n";
				it = code_mid.erase(it);
				it--;
			}
		}
	}
}

void midcode::printMidcode()
{
	ofstream fout;
	fout.open("Midcode.txt");
	for (std::vector<Quaternary>::iterator it = code_mid.begin(); it != code_mid.end(); ++it)
	{
		fout << it->op << "\t" << it->a << "\t" << it->b << "\t" << it->res << "\n";
	}
	fout.close();

	fout.open("Midcode_op.txt");
	vector<Quaternary> opmid(BBm->get_op());
	for (std::vector<Quaternary>::iterator it = opmid.begin(); it != opmid.end(); ++it)
	{
		fout << it->op << "\t" << it->a << "\t" << it->b << "\t" << it->res << "\n";
	}
	fout.close();

	if (BBm != nullptr)
	{
		BBm->Print_Block();
	}
}

void midcode::init_BB()
{
	BBm = new BB_Manager(code_mid);
}

void midcode::setOP()
{
	needop = true;
}

midcode::midcode()
{
	for (int i = 0; i < 10; i++)
	{
		temp_reg[i] = "1";
	}
	for (int i = 0; i < 8; i++)
	{
		save_reg[i] = "";
	}
}


midcode::~midcode()
{
	delete BBm;
}
