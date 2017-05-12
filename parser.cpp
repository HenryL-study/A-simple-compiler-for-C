#include "stdafx.h"
#include "parser.h"

#define Del types.pop();symbols.pop()
#define Del_Get types.pop();symbols.pop();getsym();type = types.front();
#define Clean cln_temp();cln_exp();cln_term();cln_fac();

/*
TODO: 
	1.no error process for getsym().
*/

bool parser::getsym()
{
	int num;
	while ((num=s->step_once()) == 0)
	{
		if (s->getsymbol() == COMMENT)
		{
			continue;
		}
		types.push(s->getsymbol());
		if (s->getsymbol() == UNSIGHEDINT || s->getsymbol() == SIGNEDINT)
		{
			symbols.push(numTOstrs(s->getnum()));
		}
		else
		{
			if (types.back() == IDENT)
			{
				std::string lower, origin = s->gettoken();
				lower.resize(origin.size());
				transform(origin.begin(), origin.end(), lower.begin(), ::tolower);
				symbols.push(lower);
			}
			else
			{
				symbols.push(s->gettoken());
			}
		}
		return true;
	}
	if (num == -1)
	{
		if (types.size() > 0)
		{
			return true;
		}
		//file end
		return false;
	}
	else
	{
		// TODO: ERROR PROCESS
		error(s->get_err());
		while (num != 0)
		{
			num = s->step_once();
			if (num == -1)
			{
				if (types.size() > 0)
				{
					return true;
				}
				//file end
				return false;
			}
			else if (num == 0)
			{
				if (s->getsymbol() == COMMENT)
				{
					continue;
				}
				types.push(s->getsymbol());
				if (s->getsymbol() == UNSIGHEDINT || s->getsymbol() == SIGNEDINT)
				{
					symbols.push(numTOstrs(s->getnum()));
				}
				else
				{
					if (types.back() == IDENT)
					{
						std::string lower, origin = s->gettoken();
						lower.resize(origin.size());
						transform(origin.begin(), origin.end(), lower.begin(), ::tolower);
						symbols.push(lower);
					}
					else
					{
						symbols.push(s->gettoken());
					}
				}
				return true;
			}
		}
		return true;
	}
}

void parser::program()
{
	//string sym = symbols.back();
	int type = types.front();
	int second;
	is_glo = true;
	m->insert("global", "", "", "");
	if (type == CONSTSY)
	{
		is_con = true;
		con_state();
	}
	type = types.front();
	if (type == INTSY || type == CHARSY)
	{
		getsym();
		second = types.back();
		getsym();
		if (types.back() != LPARSY)
		{
			var_state();
		}
	}
	if (types.size() == 1)
	{
		getsym();
		second = types.back();
		getsym();
	}
	m->insert("text", "", "", "");
	type = types.front();
	is_glo = false;
	while (second != MAINSY)
	{
		if (type == INTSY || type == CHARSY)
		{
			//already 3 words get
			func_re();
		}
		else if(type == VOIDSY)
		{
			//already 3 words get
			func_vo();
		}
		else
		{
			error(12);
			Del;
			Del;
			while (type != RBRACE)
			{
				if (type == RPARSY)
					break;
				Del_Get;
			}
			Del_Get;
		}
		type = types.front();
		getsym();
		second = types.back();
		getsym();
	}
	//already 3 words get
	func_main();
}

void parser::con_state()
{
	int type = types.front();
	if (type == CONSTSY)
	{
		Del;
		getsym();
		con_def();
		type = types.front();
		if (type == SEMISY)
		{
			Del_Get;
			while (type == CONSTSY)
			{
				Del;
				getsym();
				con_def();
				type = types.front();
				if (type == SEMISY)
				{
					Del;
					getsym();
					type = types.front();
				}
				else
				{
					error(11);
				}
			}
			return;
		}
		else
		{
			error(11);
			while (type == CONSTSY)
			{
				Del;
				getsym();
				con_def();
				type = types.front();
				if (type == SEMISY)
				{
					Del;
					getsym();
					type = types.front();
				}
				else
				{
					error(11);
				}
			}
		}
	}
	else
	{
		error(13);
	}
}

//already 3 words get
void parser::var_state()
{
	int type;
	var_def();
	if (types.front() == SEMISY)
	{
		Del_Get;
		if(type == INTSY || type == CHARSY)
		{
			while (type == INTSY || type == CHARSY)
			{
				getsym();
				getsym();
				if (types.back() != LPARSY)
				{
					var_def();
					if (types.front() == SEMISY)
					{
						Del_Get;
					}
					else
					{
						//TODO: NO PROCESS
						error(11);
					}
				}
				else
				{
					return;
				}
			}
		}
		else
		{
			return;
		}
	}
	else
	{
		error(11);
	}
}

//already 3 words get
void parser::func_re()
{
	int type = types.front();
	Type funct;
	//save type
	if (type == INTSY)
	{
		funct = FUNC_R_int;
	}
	else
	{
		funct = FUNC_R_char;
	}
	Del;
	if (types.front() == IDENT)
	{
		//save function name
		ident_name = symbols.front();
		func_name = ident_name;
		if (table->glo_find(func_name) != nullptr)
		{
			error(31);
			Del;
			Del_Get;
			return;
		}
		Del;
		m->insert("BEGIN", "", "", func_name);
		if (types.front() == LPARSY)
		{
			Del_Get;
			para_num = 0;
			addr_offset = 0;
			para_list();
			if (table->glo_find(func_name) != nullptr)
			{
				error(31);
			}
			else
			{
				table->insert_global(func_name, funct, 1, -1000, false, false, para_num, s->getline(), 0);
			}
			if (types.front() == RPARSY)
			{
				Del_Get;
				if (type == LBRACE)
				{
					Del_Get;
					compound_state();
					type = types.front();
					if (type == RBRACE)
					{
						Del_Get;
						m->insert("END", "", "", func_name);
						Clean;
						return;
					}
					else
					{
						error(14);
						m->insert("END", "", "", func_name);
						Clean;
						return;
					}
				}
				else
				{
					error(15);
					compound_state();
					type = types.front();
					if (type == RBRACE)
					{
						Del_Get;
						m->insert("END", "", "", func_name);
						Clean;
						return;
					}
					else
					{
						error(14);
						m->insert("END", "", "", func_name);
						Clean;
						return;
					}
				}
			}
			else
			{
				error(17);
				while (type != RBRACE)
				{
					Del_Get;
				}
				Clean;
				Del_Get;
				return;
			}
		}
		else
		{
			error(16);
			while (type != RBRACE)
			{
				Del_Get;
			}
			Clean;
			Del_Get;
			return;
		}
	}
	else
	{
		error(18);
	}
}

//already 3 words get
void parser::func_vo()
{
	int type = types.front();
	if (type == VOIDSY)
	{
		Del;
		type = types.front();
		if (type == IDENT)
		{
			func_name = symbols.front();
			//save IDENT
			m->insert("BEGIN", "", "", func_name);
			if (table->glo_find(func_name) != nullptr)
			{
				error(31);
				Del;
				Del_Get;
				return;
			}
			Del;
			if (types.front() == LPARSY)
			{
				Del_Get;
				para_num = 0;
				addr_offset = 0;
				para_list();
				if (table->glo_find(func_name) != nullptr)
				{
					error(31);
				}
				else
				{
					Type a = FUNC_V;
					table->insert_global(func_name, a, 1, -1000, false, false, para_num, s->getline(), 0);
				}
				if (types.front() == RPARSY)
				{
					Del_Get;
					if (type == LBRACE)
					{
						Del_Get;
						compound_state();
						type = types.front();
						if (type == RBRACE)
						{
							m->insert("END", "", "", func_name);
							Clean;
							Del_Get;
							return;
						}
						else
						{
							error(14);
							Clean;
							return;
						}
					}
					else
					{
						error(15);
						compound_state();
						type = types.front();
						if (type == RBRACE)
						{
							m->insert("END", "", "", func_name);
							Clean;
							Del_Get;
							return;
						}
						else
						{
							error(14);
							Clean;
							return;
						}
					}
				}
			}
			else
			{
				error(17);
				while (type != RBRACE)
				{
					Del_Get;
				}
				Clean;
				Del_Get;
				return;
			}
		}
		else
		{
			error(16);
			while (type != RBRACE)
			{
				Del_Get;
			}
			Clean;
			Del_Get;
			return;
		}

	}
	else
	{
		error(19);
	}
}

//already 3 words get
void parser::func_main()
{
	func_name = "main";
	addr_offset = 0;
	Type a = FUNC_V;
	table->insert_global(func_name, a, 1, -1000, false, false, 0, s->getline(), 0);
	m->insert("BEGIN", "", "", "main");
	int type = types.front();
	if (type == VOIDSY)
	{
		Del;
		type = types.front();
		if (type == MAINSY)
		{
			Del;
			type = types.front();
			if (type == LPARSY)
			{
				Del_Get;
			}
			else
			{
				error(16);
			}
			if (type == RPARSY)
			{
				Del_Get;
				if (type == LBRACE)
				{
					Del_Get;
					compound_state();
					if (types.front() == RBRACE)
					{
						std::cout << "Analysis end.\n";
						if (errs > 0)
						{
							std::cout << "Find " << errs << " error(s).\n";
						}
						else
						{
							std::cout << "No Grammar errors.\n";
						}
						table->printTable();
						if (need == 1)
						{
							m->setOP();
						}
						m->init_BB();
						m->printMidcode();
						m->gen_code(table);
						return;
					}
					else
					{
						error(14);
						std::cout << "Compile end.\n";
						if (errs > 0)
						{
							std::cout << "Find " << errs << " error(s).\n";
						}
						else
						{
							std::cout << "No Grammar errors.\n";
						}
						return;
					}
				}
				else
				{
					error(15);
					compound_state();
					if (types.front() == RBRACE)
					{
						std::cout << "Compile end.\n";
						if (errs > 0)
						{
							std::cout << "Find " << errs << " error(s).\n";
						}
						else
						{
							std::cout << "No Grammar errors.\n";
						}
						table->printTable();
						m->printMidcode();
						m->gen_code(table);
						return;
					}
					else
					{
						error(14);
						std::cout << "Compile end.\n";
						if (errs > 0)
						{
							std::cout << "Find " << errs << " error(s).\n";
						}
						else
						{
							std::cout << "No Grammar errors.\n";
						}
						return;
					}
				}
			}
			else
			{
				error(17);
				std::cout << "Compile end.\n";
				if (errs > 0)
				{
					std::cout << "Find " << errs << " error(s).\n";
				}
				else
				{
					std::cout << "No Grammar errors.\n";
				}
				return;
			}
		}
		else
		{
			error(20);
		}
	}
	else
	{
		error(19);
	}
}

void parser::con_def() 
{
	is_arr = false;
	//remember save type
	int type = types.front();
	//var_type = type;
	if (type == INTSY)
	{
		Del_Get;
		if (type == IDENT)
		{
			ident_name = symbols.front();
			Del_Get;
			if (type == ASSIGHSY)
			{
				Del;
				getsym();
				_integer();
				t = VAR_int;
				if (is_glo)
				{
					if (table->glo_find(ident_name)!= nullptr)
					{
						error(31);
					}
					else
					{
						m->insert("const", "int", numTOstrs(num_value), ident_name);
						if (t == VAR_int && data_addr % 4 != 0)
						{
							int dump = 4 - data_addr % 4;
							data_addr += dump;
						}
						table->insert_global(ident_name, t, 1, data_addr, is_arr, is_con, 0, s->getline(), 0);
						data_addr += 4; //1
					}
				}
				else
				{
					if (table->func_find(func_name, ident_name) != nullptr)
					{
						error(31);
					}
					else
					{
						m->insert("const", "int", numTOstrs(num_value), ident_name);
						if (t == VAR_int && addr_offset % 4 != 0)
						{
							int dump = 4 - addr_offset % 4;
							addr_offset += dump;
						}
						table->insert_func(func_name, ident_name, t, 1, data_addr, is_arr, is_con, 0, s->getline(), addr_offset);
						addr_offset += 4;
					}	
				}
				type = types.front();
				while (type == COMMASY)
				{
					Del_Get;
					if (type == IDENT)
					{
						ident_name = symbols.front();
						Del_Get;
						if (type == ASSIGHSY)
						{
							Del;
							getsym();
							_integer();
							t = VAR_int;
							if (is_glo)
							{
								if (table->glo_find(ident_name) != nullptr)
									error(31);
								else
								{
									m->insert("const", "int", numTOstrs(num_value), ident_name);
									if (t == VAR_int && data_addr % 4 != 0)
									{
										int dump = 4 - data_addr % 4;
										data_addr += dump;
									}
									table->insert_global(ident_name, t, 1, data_addr, is_arr, is_con, 0, s->getline(), 0);
									data_addr += 4; //1
								}
							}
							else
							{
								if (table->func_find(func_name, ident_name))
									error(31);
								else
								{
									m->insert("const", "int", numTOstrs(num_value), ident_name);
									if (t == VAR_int && addr_offset % 4 != 0)
									{
										int dump = 4 - addr_offset % 4;
										addr_offset += dump;
									}
									table->insert_func(func_name, ident_name, t, 1, data_addr, is_arr, is_con, 0, s->getline(), addr_offset);
									addr_offset += 4;
								}	
							}
							type = types.front();
						}
						else
						{
							error(21);
						}
					}
					else
					{
						error(18);
					}
				}
				return;
			}
			else
			{
				error(21);
			}
		}
		else
		{
			error(18);
		}
	}
	else if (type == CHARSY)
	{
		Del_Get;
		if (type == IDENT)
		{
			ident_name = symbols.front();
			Del_Get;
			if (type == ASSIGHSY)
			{
				Del_Get;
				if (type == _CHAR)
				{
					//record a char
					std::string c = symbols.front();
					Type t = VAR_char;
					if (is_glo)
					{
						if (table->glo_find(ident_name) != nullptr)
							error(31);
						else
						{
							m->insert("const", "char", c, ident_name);
							table->insert_global(ident_name, t, 1, data_addr, is_arr, is_con, 0, s->getline(), 0);
							data_addr += 4;
						}
					}
					else
					{
						if (table->func_find(func_name, ident_name))
							error(31);
						else
						{
							m->insert("const", "char", c, ident_name);
							table->insert_func(func_name, ident_name, t, 1, data_addr, is_arr, is_con, 0, s->getline(), addr_offset);
							addr_offset += 4; //1
						}
					}
					Del_Get;
					while (type == COMMASY)
					{
						Del_Get;
						if (type == IDENT)
						{
							ident_name = symbols.front();
							Del_Get;
							if (type == ASSIGHSY)
							{
								Del_Get;
								if (type == _CHAR)
								{
									//TODO: emit a char
									std::string c = symbols.front();
									Type t = VAR_char;
									if (is_glo)
									{
										if (table->glo_find(ident_name) != nullptr)
											error(31);
										else
										{
											m->insert("const", "char", c, ident_name);
											table->insert_global(ident_name, t, 1, data_addr, is_arr, is_con, 0, s->getline(), 0);
											data_addr += 4;
										}
									}
									else
									{
										if (table->func_find(func_name, ident_name))
											error(31);
										else
										{
											m->insert("const", "char", c, ident_name);
											table->insert_func(func_name, ident_name, t, 1, data_addr, is_arr, is_con, 0, s->getline(), addr_offset);
											addr_offset += 4; //1
										}
									}
									Del_Get;
								}
								else
								{
									error(22);
								}
							}
							else
							{
								error(21);
							}
						}
						else
						{
							error(18);
						}
					}
					return;
				}
				else
				{
					error(22);
				}
			}
			else
			{
				error(18);
			}
		}
		else
		{
			error(22);
		}
	}
	else
	{
		error(12);
		while (type != SEMISY)
		{
			Del_Get;
		}
	}
}

//already 3 words get
void parser::var_def() 
{
	//aleady get WORD1(int/char) WORD2 WORD3
	//remember save type
	if (types.front() == CHARSY)
	{
		t = VAR_char;
	}
	else
	{
		t = VAR_int;
	}
	Del;
	int type = types.front();
	if (type == IDENT)
	{
		ident_name = symbols.front();
		Del;
		type = types.front();
		if (type == LBRAKET)
		{
			is_arr = true;
			Del_Get;
			if (type == UNSIGHEDINT)
			{
				int num = strTOnum(symbols.front());
				//emit a array
				if (is_glo)
				{
					if (table->glo_find(ident_name) != nullptr)
						error(31);
					else
					{
						if (data_addr % 4 != 0)
						{
							int dump = 4 - data_addr % 4;
							data_addr += dump;
						}
						table->insert_global(ident_name, t, num, data_addr, is_arr, false, 0, s->getline(), addr_offset);
						if (t == VAR_int)
						{
							m->insert("int", numTOstrs(num), "", ident_name);
							data_addr += num * 4;
						}
						else
						{
							m->insert("char", numTOstrs(num), "", ident_name);
							data_addr += num * 4;
						}
					}
				}
				else
				{
					if (table->func_find(func_name, ident_name) != nullptr)
					{
						error(31);
					}
					else
					{
						if (addr_offset % 4 != 0)
						{
							int dump = 4 - addr_offset % 4;
							addr_offset += dump;
						}
						table->insert_func(func_name, ident_name, t, num, data_addr, is_arr, false, 0, s->getline(), addr_offset);
						if (t == VAR_int)
						{
							//m->insert("int", numTOstrs(num), "", ident_name);
							addr_offset += num * 4;
						}
						else
						{
							//m->insert("char", numTOstrs(num), "", ident_name);
							addr_offset += num;
						}
					}	
				}
				Del_Get;
				if (type == RBRAKET)
				{
					is_arr = false;
					Del_Get;
					if (type == COMMASY)
					{
						while (type == COMMASY)
						{
							Del_Get;
							if (type == IDENT)
							{
								ident_name = symbols.front();
								Del_Get;
								if (type == LBRAKET)
								{
									is_arr = true;
									Del_Get;
									if (type == UNSIGHEDINT)
									{
										int num = strTOnum(symbols.front());
										//emit a array
										if (is_glo)
										{
											if (table->glo_find(ident_name) != nullptr)
												error(31);
											else
											{
												if (data_addr % 4 != 0)
												{
													int dump = 4 - data_addr % 4;
													data_addr += dump;
												}
												table->insert_global(ident_name, t, num, data_addr, is_arr, false, 0, s->getline(), addr_offset);
												if (t == VAR_int)
												{
													m->insert("int", numTOstrs(num), "", ident_name);
													data_addr += num * 4;
												}
												else
												{
													m->insert("char", numTOstrs(num), "", ident_name);
													data_addr += num * 4;
												}
											}
										}
										else
										{
											if (table->func_find(func_name, ident_name) != nullptr)
											{
												error(31);
											}
											else
											{
												if (addr_offset % 4 != 0)
												{
													int dump = 4 - addr_offset % 4;
													addr_offset += dump;
												}
												table->insert_func(func_name, ident_name, t, num, data_addr, is_arr, false, 0, s->getline(), addr_offset);
												if (t == VAR_int)
												{
													//m->insert("int", numTOstrs(num), "", ident_name);
													addr_offset += 4 * num;
												}
												else
												{
													//m->insert("char", numTOstrs(num), "", ident_name);
													addr_offset += num;
												}
											}
										}
										Del_Get;
										if (type == RBRAKET)
										{
											is_arr = false;
											Del_Get;
										}
										else
										{
											error(23);
										}
									}
									else
									{
										error(24);
									}
								}
								else
								{
									is_arr = false;
									if (is_glo)
									{
										if (table->glo_find(ident_name) != nullptr)
											error(31);
										else
										{
											if (t == VAR_int && data_addr % 4 != 0)
											{
												int dump = 4 - data_addr % 4;
												data_addr += dump;
											}
											table->insert_global(ident_name, t, 1, data_addr, is_arr, false, 0, s->getline(), 0);
											if (t == VAR_int)
											{
												m->insert("int", "", "", ident_name);
												data_addr += 4; //1
											}
											else
											{
												m->insert("char", "", "", ident_name);
												data_addr += 4;
											}
										}					
									}
									else
									{
										if (table->func_find(func_name, ident_name) != nullptr)
										{
											error(31);
										}
										else
										{
											if (t == VAR_int && addr_offset % 4 != 0)
											{
												int dump = 4 - addr_offset % 4;
												addr_offset += dump;
											}
											table->insert_func(func_name, ident_name, t, 1, data_addr, is_arr, false, 0, s->getline(), addr_offset);
											if (t == VAR_int)
											{
												//m->insert("int", "", "", ident_name);
												addr_offset += 4;
											}
											else
											{
												//m->insert("char", "", "", ident_name);
												addr_offset += 4; //1
											}
										}
									}
								}
							}
							else
							{
								error(18);
							}
						}
						return;
					}
					else
					{
						return;
					}
				}
				else
				{
					error(23);
				}

			}
			else
			{
				error(25);
			}
		}
		else if (type == COMMASY)
		{
			is_arr = false;
			if (is_glo)
			{
				if (table->glo_find(ident_name) != nullptr)
					error(31);
				else
				{
					if (t == VAR_int && data_addr % 4 != 0)
					{
						int dump = 4 - data_addr % 4;
						data_addr += dump;
					}
					table->insert_global(ident_name, t, 1, data_addr, is_arr, false, 0, s->getline(), 0);
					if (t == VAR_int)
					{
						m->insert("int", "", "", ident_name);
						data_addr += 4; //1
					}
					else
					{
						m->insert("char", "", "", ident_name);
						data_addr += 4;
					}
				}
			}
			else
			{
				if (table->func_find(func_name, ident_name) != nullptr)
				{
					error(31);
				}
				else
				{
					if (t == VAR_int && addr_offset % 4 != 0)
					{
						int dump = 4 - addr_offset % 4;
						addr_offset += dump;
					}
					table->insert_func(func_name, ident_name, t, 1, data_addr, is_arr, false, 0, s->getline(), addr_offset);
					if (t == VAR_int)
					{
						//m->insert("int", "", "", ident_name);
						addr_offset += 4;
					}
					else
					{
						//m->insert("char", "", "", ident_name);
						addr_offset += 4; //1
					}
				}
			}
			while (type == COMMASY)
			{
				Del_Get;
				if (type == IDENT)
				{
					//save IDENT
					ident_name = symbols.front();
					Del_Get;
					if (type == LBRAKET)
					{
						is_arr = true;
						Del_Get;
						if (type == UNSIGHEDINT)
						{
							int num = strTOnum(symbols.front());
							//save num
							if (is_glo)
							{
								if (table->glo_find(ident_name) != nullptr)
									error(31);
								else
								{
									if (data_addr % 4 != 0)
									{
										int dump = 4 - data_addr % 4;
										data_addr += dump;
									}
									table->insert_global(ident_name, t, num, data_addr, is_arr, false, 0, s->getline(), addr_offset);
									if (t == VAR_int)
									{
										m->insert("int", numTOstrs(num), "", ident_name);
										data_addr += num * 4;
									}
									else
									{
										m->insert("char", numTOstrs(num), "", ident_name);
										data_addr += num * 4;
									}
								}
							}
							else
							{
								if (table->func_find(func_name, ident_name) != nullptr)
								{
									error(31);
								}
								else
								{
									if (addr_offset % 4 != 0)
									{
										int dump = 4 - addr_offset % 4;
										addr_offset += dump;
									}
									table->insert_func(func_name, ident_name, t, num, data_addr, is_arr, false, 0, s->getline(), addr_offset);
									if (t == VAR_int)
									{
										//m->insert("int", numTOstrs(num), "", ident_name);
										addr_offset += 4 * num;
									}
									else
									{
										//m->insert("char", numTOstrs(num), "", ident_name);
										addr_offset += num;
									}
								}
							}
							Del_Get;
							if (type == RBRAKET)
							{
								is_arr = false;
								Del_Get;
							}
							else
							{
								error(23);
							}
						}
						else
						{
							error(25);
						}
					}
					else
					{
						is_arr = false;
						if (is_glo)
						{
							if (table->glo_find(ident_name) != nullptr)
								error(31);
							else
							{
								if (t == VAR_int && data_addr % 4 != 0)
								{
									int dump = 4 - data_addr % 4;
									data_addr += dump;
								}
								table->insert_global(ident_name, t, 1, data_addr, is_arr, false, 0, s->getline(), 0);
								if (t == VAR_int)
								{
									m->insert("int", "", "", ident_name);
									data_addr += 4; //1
								}
								else
								{
									m->insert("char", "", "", ident_name);
									data_addr += 4;
								}
							}
						}
						else
						{
							if (table->func_find(func_name, ident_name) != nullptr)
							{
								error(31);
							}
							else
							{
								if (t == VAR_int && addr_offset % 4 != 0)
								{
									int dump = 4 - addr_offset % 4;
									addr_offset += dump;
								}
								table->insert_func(func_name, ident_name, t, 1, data_addr, is_arr, false, 0, s->getline(), addr_offset);
								if (t == VAR_int)
								{
									//m->insert("int", "", "", ident_name);
									addr_offset += 4;
								}
								else
								{
									//m->insert("char", "", "", ident_name);
									addr_offset += 4; //1
								}
							}
						}
					}
				}
				else
				{
					error(18);
				}
			}
			return;
		}
		else
		{
			is_arr = false;
			if (is_glo)
			{
				if (table->glo_find(ident_name) != nullptr)
					error(31);
				else
				{
					if (t == VAR_int && data_addr % 4 != 0)
					{
						int dump = 4 - data_addr % 4;
						data_addr += dump;
					}
					table->insert_global(ident_name, t, 1, data_addr, is_arr, false, 0, s->getline(), 0);
					if (t == VAR_int)
					{
						m->insert("int", "", "", ident_name);
						data_addr += 4; //1
					}
					else
					{
						m->insert("char", "", "", ident_name);
						data_addr += 4;
					}
				}
			}
			else
			{
				if (table->func_find(func_name, ident_name) != nullptr)
				{
					error(31);
				}
				else
				{
					if (t == VAR_int && addr_offset % 4 != 0)
					{
						int dump = 4 - addr_offset % 4;
						addr_offset += dump;
					}
					table->insert_func(func_name, ident_name, t, 1, data_addr, is_arr, false, 0, s->getline(), addr_offset);
					if (t == VAR_int)
					{
						//m->insert("int", "", "", ident_name);
						addr_offset += 4;
					}
					else
					{
						//m->insert("char", "", "", ident_name);
						addr_offset += 4; //1
					}
				}
			}
			return;
		}
	}
	else
	{
		error(18);
	}

}

void parser::def_head() 
{

}

void parser::func_para() 
{

}

void parser::compound_state()
{
	int type = types.front();
	if (type == CONSTSY)
	{
		con_state();
	}
	if (types.front() == CHARSY || types.front() == INTSY)
	{
		getsym();
		getsym();
		var_state();
	}
	statement_col();
	return;
}

void parser::para_list() 
{
	int type = types.front();
	if (type == RPARSY)
	{
	}
	else if (type == CHARSY || type == INTSY)
	{

		//SAVE type
		if (type == CHARSY)
		{
			t = PAR_char;
		}
		else
		{
			t = PAR_int;
		}
		Del_Get;
		if (type == IDENT)
		{
			//save ident
			ident_name = symbols.front();
			if (table->func_find(func_name, ident_name) != nullptr)
			{
				error(32);
			}
			else
			{
				if (t == PAR_int && addr_offset % 4 != 0)
				{
					int dump = 4 - addr_offset % 4;
					addr_offset += dump;
				}
				table->insert_func(func_name, ident_name, t, para_num, -1, false, false, 0, s->getline(), addr_offset);
				if (para_num > 4)
				{
					if (t == PAR_int)
					{
						//m->insert("PARA", "int", "", ident_name);
						addr_offset += 4;
					}
					else
					{
						//m->insert("PARA", "char", "", ident_name);
						addr_offset += 4; //1
					}
				}
				para_num += 1;
			}
			Del_Get;
			while (type == COMMASY)
			{
				Del_Get;
				if (type == CHARSY || type == INTSY)
				{
					//SAVE type
					if (type == CHARSY)
					{
						t = PAR_char;
					}
					else
					{
						t = PAR_int;
					}
					Del_Get;
					if (type == IDENT)
					{
						//save ident
						ident_name = symbols.front();
						if (table->func_find(func_name, ident_name) != nullptr)
						{
							error(32);
						}
						else
						{
							if (t == PAR_int && addr_offset % 4 != 0)
							{
								int dump = 4 - addr_offset % 4;
								addr_offset += dump;
							}
							table->insert_func(func_name, ident_name, t, para_num, -1, false, false, 0, s->getline(), addr_offset);
							if (para_num > 4)
							{
								if (t == PAR_int)
								{
									//m->insert("PARA", "int", "", ident_name);
									addr_offset += 4;
								}
								else
								{
									//m->insert("PARA", "char", "", ident_name);
									addr_offset += 4; //1
								}
							}
							para_num += 1;
						}
						Del_Get;
					}
					else
					{
						error(18);
					}
				}
				else
				{
					error(12);
					while (type != COMMASY)
					{
						if (type == RPARSY)
							break;
						Del_Get;
					}
				}
			}
		}
		else
		{
			error(18);
		}
	}
}

void parser::statement_col() 
{
	int type = types.front();
	while (type != RBRACE)
	{
		
		if (type == IFSY)
		{
			_if();
			type = types.front();
			continue;
		}
		else if (type == WHILESY)
		{
			_while();
			type = types.front();
			continue;
		}
		else if (type == LBRACE)
		{
			Del_Get;
			statement_col();
			if (types.front() == RBRACE)
			{
				Del_Get;
				continue;
			}
			else
			{
				//missing '}'
				error(14);
				Del_Get;
				continue;
			}
		}
		else if (type == IDENT)
		{
			std::string _ident = symbols.front();
			getsym();
			if (types.back() == ASSIGHSY || types.back() == LBRAKET)
			{
				//already get 2 words
				assign();
			}
			else if (types.back() == LPARSY)
			{
				//search_table();
				//but now: all re
				//std::cout << "This is a function call!\n";
				//if (true)
				//{
				//	//already get 2 words
				//	funcre_call();
				//}
				//else
				//{
				//	//already get 2 words
				//	funcvo_call();
				//}
				Del;
				Del_Get;
				int para_nums = 0;
				if (type == RPARSY)
				{
					//no para
					Del_Get;
				}
				else
				{
					gen_exp();
					std::string _exp = get_exp();
					expression();
					m->insert("PUSH", "", "", _exp);
					para_nums++;
					type = types.front();
					while (type == COMMASY)
					{
						Del_Get;
						gen_exp();
						std::string _exp1 = get_exp();
						expression();
						type = types.front();
						m->insert("PUSH", "", "", _exp1);
						para_nums++;
					}
					if (type == RPARSY)
					{
						//have para
						Del_Get;
					}
					else
					{
						error(17);
					}
				}
				gen_temp();
				if (table->glo_find(_ident) == nullptr)
				{
					error(27);
				}
				else if (table->glo_find(_ident)->para_num != para_nums)
				{
					error(34);
				}
				else
				{
					m->insert("CALL", "", "", _ident);
				}
			}
			else
			{
				error(26);
			}
		}
		else if (type == SCANFSY)
		{
			_read();
		}
		else if (type == PRINTFSY)
		{
			_write();
		}
		else if (type == SWITCHSY)
		{
			_switch();
			type = types.front();
			continue;
		}
		else if (type == RETURNSY)
		{
			_return();
		}
		else if (type == SEMISY)
		{
		}
		else
		{
			error(27);
		}
		type = types.front();
		if (type == SEMISY)
		{
			Del_Get;
		}
		else
		{
			error(11);
		}
	}
}

void parser::statement() 
{
	int type = types.front();
	if (type == IFSY)
	{
		_if();
		return;
	}
	else if (type == WHILESY)
	{
		_while();
		return;
	}
	else if (type == LBRACE)
	{
		Del_Get;
		statement_col();
		if (types.front() == RBRACE)
		{
			Del_Get;
			return;
		}
		else
		{
			//missing '}'
			error(14);
			return;
		}
	}
	else if (type == IDENT)
	{
		std::string _ident = symbols.front();
		getsym();
		if (types.back() == ASSIGHSY || types.back() == LBRAKET)
		{
			//already get 2 words
			assign();
		}
		else if (types.back() == LPARSY)
		{
			//search_table();
			//but now: all re
			//std::cout << "This is a function call!\n";
			//if (true)
			//{
			//	//already get 2 words
			//	funcre_call();
			//}
			//else
			//{
			//	//already get 2 words
			//	funcvo_call();
			//}
			Del;
			Del_Get;
			int para_nums = 0;
			if (type == RPARSY)
			{
				//no para
				Del_Get;
			}
			else
			{
				gen_exp();
				std::string _exp = get_exp();
				expression();
				m->insert("PUSH", "", "", _exp);
				para_nums++;
				type = types.front();
				while (type == COMMASY)
				{
					Del_Get;
					gen_exp();
					std::string _exp1 = get_exp();
					expression();
					type = types.front();
					m->insert("PUSH", "", "", _exp1);
					para_nums++;
				}
				if (type == RPARSY)
				{
					//have para
					Del_Get;
				}
				else
				{
					error(17);
				}
			}
			gen_temp();
			if (table->glo_find(_ident) == nullptr)
			{
				error(27);
			}
			else if (table->glo_find(_ident)->para_num != para_nums)
			{
				error(34);
			}
			else
			{
				m->insert("CALL", "", "", _ident);
			}

		}
		else
		{
			error(26);
		}
	}
	else if (type == SCANFSY)
	{
		_read();
	}
	else if (type == PRINTFSY)
	{
		_write();
	}
	else if (type == SWITCHSY)
	{
		_switch();
		return;
	}
	else if (type == RETURNSY)
	{
		_return();
	}
	else if (type == SEMISY)
	{
	}
	else
	{
		error(26);
	}
	type = types.front();
	if (type == SEMISY)
	{
		Del_Get;
	}
	else
	{
		error(11);
	}
}

void parser::_if() 
{
	int type = types.front();
	if (type == IFSY)
	{

		Del_Get;
		if (type == LPARSY)
		{
			Del_Get;
			//ifcon = true;
			gen_label();
			std::string _label = get_label();
			gen_label();
			std::string _label1 = get_label();
			condition();
			if (types.front() == RPARSY)
			{
				Del_Get;
			}
			else
			{
				error(17);
			}
			statement();
			if (types.front() == ELSESY)
			{
				Del_Get;
				m->insert("GO", "", "", _label);
				m->insert("SET", "", "", _label1);
				statement();
				m->insert("SET", "", "", _label);
				return;
			}
			else
			{
				m->insert("SET", "", "", _label1);
				return;
			}
		}
		else
		{
			error(16);
			while (type != RPARSY)
			{
				Del_Get;
			}
		}
	}
	else
	{
		error(0);
	}
}

void parser::_while() 
{
	int type = types.front();
	if (type == WHILESY)
	{
		Del_Get;
		if (type == LPARSY)
		{
			Del_Get;
			gen_label();
			std::string _label0 = get_label();
			m->insert("SET", "", "", _label0);

			gen_label();
			std::string _label1 = get_label();
			condition();

			if (types.front() == RPARSY)
			{
				Del_Get;
				statement();
				m->insert("GO", "", "", _label0);
				m->insert("SET", "", "", _label1);
			}
			else
			{
				error(17);
			}
		}
		else
		{
			error(16);
			while (type != RBRACE)
			{
				Del_Get;
			}
		}
	}
	else
	{
		error(0);
	}
}

//already get 2 words
void parser::assign() 
{
	int type = types.front();
	if (type == IDENT)
	{
		std::string _ident = symbols.front();
		Del;
		type = types.front();
		if (type == ASSIGHSY)
		{
			Del_Get;
			gen_exp();
			std::string _exp = get_exp();
			expression();
			m->insert("ASSIGN", _exp, "", _ident);
		}
		else if (type == LBRAKET)
		{
			Del_Get;
			gen_exp();
			std::string _exp0 = get_exp();
			expression();
			if (types.front() == RBRAKET)
			{
				Del_Get;
				if (type == ASSIGHSY)
				{
					Del_Get;
					gen_exp();
					std::string _exp1 = get_exp();
					expression();
					m->insert("ASSTA", _exp1, _exp0, _ident);
				}
				else
				{
					error(21);
				}
			}
			else
			{
				error(23);
			}
		}
	}
	else
	{
		error(18);
	}
}

void parser::_read() 
{
	int type = types.front();
	if (type == SCANFSY)
	{
		Del_Get;
		if (type == LPARSY)
		{
			Del_Get;
			if (type == IDENT)
			{
				//Save IDENT
				if (table->func_find(func_name,symbols.front()) == nullptr && table->glo_find(symbols.front()) == nullptr)
				{
					error(27);
				}
				else if (table->func_find(func_name,symbols.front()) != nullptr && ((table->func_find(func_name, symbols.front())->type != VAR_char && table->func_find(func_name, symbols.front())->type != VAR_int) || table->func_find(func_name, symbols.front())->isArray))
				{
					error(37);
				}
				else if (table->glo_find(symbols.front()) != nullptr && ((table->glo_find(symbols.front())->type != VAR_char && table->glo_find(symbols.front())->type != VAR_int) || table->glo_find(symbols.front())->isArray))
				{
					error(37);
				}
				m->insert("RD", "", "", symbols.front());
				Del_Get;
				while (type == COMMASY)
				{
					Del_Get;
					//Save IDENT
					if (table->func_find(func_name, symbols.front()) == nullptr && table->glo_find(symbols.front()) == nullptr)
					{
						error(27);
					}
					m->insert("RD", "", "", symbols.front());
					Del_Get;
				}
				if (type == RPARSY)
				{
					Del_Get;
				}
				else
				{
					error(17);
				}
			}
			else
			{
				error(18);
			}
		}
		else
		{
			error(16);
			while (type != SEMISY)
			{
				Del_Get;
			}
		}
	}
	else
	{
		error(0);
	}
}

void parser::_write() 
{
	int type = types.front();
	if (type == PRINTFSY)
	{
		Del_Get;
		if (type == LPARSY)
		{
			Del_Get;
			if (type == _STRING)
			{
				//TODO: Print string.
				Type str_t = STR_p;
				std::string str_name = gen_str();
				table->insert_global(str_name, str_t, 1, data_addr, false, true, 0, s->getline(), 0);
				m->insert("PD", str_name, symbols.front(), "");
				Del_Get;
				if (type == COMMASY)
				{
					Del_Get;
					if (true)
					{
						gen_exp();
						std::string _exp = get_exp();
						expression();
						if (types.front() == RPARSY)
						{
							if (is_CHAR != "")
							{
								if ((table->glo_find(is_CHAR) != nullptr && (table->glo_find(is_CHAR)->type == VAR_char || table->glo_find(is_CHAR)->type == FUNC_R_char)) || (table->func_find(func_name, is_CHAR) != nullptr && (table->func_find(func_name, is_CHAR)->type == VAR_char || table->func_find(func_name, is_CHAR)->type == PAR_char)))
								{
									m->insert("PD", "", "c", _exp);
								}
								else
								{
									m->insert("PD", "", "", _exp);
								}
							}
							else
							{
								m->insert("PD", "", "", _exp);
							}
							Del_Get;
						}
						else
						{
							error(17);
						}
					}
				}
				else if (types.front() == RPARSY)
				{
					Del_Get;
				}
				else
				{
					error(28);
				}
			}
			else
			{
				gen_exp();
				std::string _exp = get_exp();
				expression();
				if (types.front() == RPARSY)
				{
					if (is_CHAR != "")
					{
						if ((table->glo_find(is_CHAR) != nullptr && (table->glo_find(is_CHAR)->type == VAR_char || table->glo_find(is_CHAR)->type == FUNC_R_char)) || (table->func_find(func_name, is_CHAR) != nullptr && (table->func_find(func_name, is_CHAR)->type == VAR_char || table->func_find(func_name, is_CHAR)->type == PAR_char)))
						{
							m->insert("PD", "", "c", _exp);
						}
						else
						{
							m->insert("PD", "", "", _exp);
						}
					}
					else
					{
						m->insert("PD", "", "", _exp);
					}
					Del_Get;
				}
				else
				{
					error(17);
				}
			}
		}
		else
		{
			error(16);
			while (type != SEMISY)
			{
				Del_Get;
			}
		}
	}
	else
	{
		error(28);
	}
}

void parser::_switch() 
{
	int type = types.front();
	if (type == SWITCHSY)
	{
		Del_Get;
		if (type == LPARSY)
		{
			Del_Get;
		}
		else
		{
			error(16);
		}
		gen_exp();
		swi_ans = get_exp();
		expression();
		if (types.front() == RPARSY)
		{
			Del_Get;
			if (type == LBRACE)
			{
				Del_Get;
				gen_label();
				swi_lab = get_label();
				condition_list();
				if (types.front() == DEFAULTSY)
				{
					_default();
				}
				if (types.front() == RBRACE)
				{
					Del_Get;
					m->insert("SET", "", "", swi_lab);
					return;
				}
				else
				{
					error(14);
					m->insert("SET", "", "", swi_lab);
					return;
				}
			}
			else
			{
				error(15);
				while (type != RBRACE)
				{
					Del_Get;
				}
				Del_Get;
				return;
			}
		}
		else
		{
			error(17);
			while (type != RBRACE)
			{
				Del_Get;
			}
			Del_Get;
			return;
		}
	}
	else
	{
		error(0);
	}
}

void parser::_return() 
{
	int type = types.front();
	if (type == RETURNSY)
	{
		Del_Get;
		if (type == LPARSY)
		{
			if (table->glo_find(func_name) != nullptr && table->glo_find(func_name)->type == FUNC_V)
			{
				error(35);
			}
			Del_Get;
			gen_exp();
			std::string _exp = get_exp();
			expression();
			if (types.front() == RPARSY)
			{
				Del_Get;
				m->insert("RET", "", "", _exp);
			}
			else
			{
				error(17);
			}
		}
		else
		{
			m->insert("RET", "", "", "");
		}
	}
	else
	{
		error(0);
	}
}

void parser::condition() 
{
	gen_exp();
	std::string exp_t1 = get_exp();
	std::string exp_t2;
	expression();
	int type = types.front();
	int reOP = type;
	if (type == BIGGERSY || type == LESS_EQU || type == LESSSY || type == BIG_EQU || type == NOTEQU || type == EQUSY)
	{
		//TODO: save relation op
		Del_Get;
		gen_exp();
		exp_t2 = get_exp();
		expression();
	}
	else if (type == RPARSY)
	{
		gen_exp();
		exp_t2 = get_exp();
		m->insert("ASSIGN", "0", "", exp_t2);
		reOP = NOTEQU;
	}
	else
	{
		error(26);
	}
	/*
	BEQ OP1 OP2 label	OP1 = OP2则跳转至label
	BNE OP1 OP2 label	OP1 != OP2则跳转至label
	BL OP1 OP2 label	OP1 > OP2则跳转至label
	BS OP1 OP2 label	OP1 < OP2则跳转至label
	BLE OP1 OP2 label	OP1 >= OP2则跳转至label
	BSE OP1 OP2 label	OP1 <= OP2则跳转至label
	*/

	switch (reOP)
	{
	case BIGGERSY:
		m->insert("BSE", exp_t1, exp_t2, get_label());
		break;
	case LESS_EQU:
		m->insert("BL", exp_t1, exp_t2, get_label());
		break;
	case LESSSY:
		m->insert("BLE", exp_t1, exp_t2, get_label());
		break;
	case BIG_EQU:
		m->insert("BS", exp_t1, exp_t2, get_label());
		break;
	case NOTEQU:
		m->insert("BEQ", exp_t1, exp_t2, get_label());
		break;
	case EQUSY:
		m->insert("BNE", exp_t1, exp_t2, get_label());
		break;
	default:
		std::cout << "????????????????????";
		break;
	}
}

void parser::condition_list() 
{
	int type = types.front();
	if (type == CASESY)
	{
		gen_label();
		std::string setlab = get_label();
		condition_sub();
		while (types.front() == CASESY)
		{
			m->insert("SET", "", "", setlab);
			gen_label();
			setlab = get_label();
			condition_sub();
		}
		m->insert("SET", "", "", setlab);
	}
	else
	{
		error(0);
	}
}

void parser::_default() 
{
	int type = types.front();
	if (type == DEFAULTSY)
	{
		Del_Get;
		if (type == COLONSY)
		{
			Del_Get;
			statement();
		}
		else
		{
			error(29);
		}
	}
	else
	{
		error(0);
	}
}

void parser::condition_sub() 
{
	int type = types.front();
	std::string next = get_label();
	if (type == CASESY)
	{
		Del_Get;
		gen_temp();
		std::string tmp = get_temp();
		if (type == SIGNEDINT)
		{
			//case:0
			//TODO: something
			m->insert("ASSIGN", "0", "", tmp);
			m->insert("BNE", tmp, swi_ans, next);
			Del_Get;
		}
		else if (type == _CHAR)
		{
			//case: char
			int c = symbols.front().c_str()[0];
			m->insert("ASSIGN", numTOstrs(c), "", tmp);
			m->insert("BNE", tmp, swi_ans, next);
			//TODO:
			Del_Get;
		}
		else
		{
			_integer();
			m->insert("ASSIGN", numTOstrs(num_value), "", tmp);
			m->insert("BNE", tmp, swi_ans, next);
		}
		if (types.front() == COLONSY)
		{
			Del_Get;
			statement();
			m->insert("GO", "", "", swi_lab);
		}
		else
		{
			error(29);
		}
	}
	else
	{
		error(0);
	}
}

void parser::expression() 
{
	int type = types.front();
	int hasP = false;
	std::string exp_r = get_exp();   //return this 
	if (type == PLUSSY || type == MINUSSY)
	{
		
		//TODO: DO something.
		if (type == MINUSSY)
		{
			hasP = true;
		}
		Del_Get;
	}
	gen_term();
	std::string myt = get_term();
	term();
	if (hasP)
	{
		is_CHAR = "";
		m->insert("SUB", "0", myt, myt);
	}
	while (types.front() == PLUSSY || types.front() == MINUSSY)
	{
		is_CHAR = "";
		int op = types.front();
		Del_Get;
		gen_term();
		std::string myt1 = get_term();
		term();
		if (op == PLUSSY)
		{
			m->insert("ADD", myt, myt1, myt);
		}
		else
		{
			m->insert("SUB", myt, myt1, myt);
		}
	}
	m->insert("ASSIGN", myt, "", exp_r);
}

void parser::term() 
{
	std::string trm_r = get_term();
	int type;
	std::string _fac = gen_fac();
	factor();
	while (types.front() == MULSY || types.front() == DIVSY)
	{
		int op = types.front();
		Del_Get;
		std::string _fac1 = gen_fac();
		factor();
		if (op == MULSY)
		{
			m->insert("MUL", _fac, _fac1, _fac);
		}
		else
		{
			m->insert("DIV", _fac, _fac1, _fac);
		}
	}
	m->insert("ASSIGN", _fac, "", trm_r);
}

void parser::factor() 
{
	std::string fac_r = get_fac();
	int type = types.front();
	if (type == IDENT)
	{
		//Now I dont have a symbol table so I dont know whether it's a function call.
		//So there a some specail process here and in funcre_call & funcvo_call.
		//save IDENT
		std::string _ident = symbols.front();
		is_CHAR = _ident;
		Del_Get;
		if (type == LBRAKET)
		{
			Del_Get;
			gen_exp();
			std::string _exp = get_exp();
			expression();
			if (types.front() == RBRAKET)
			{
				//TODO: Do array's things.
				m->insert("ASSFA", _ident, _exp, fac_r);
				is_CHAR = _ident;
				Del_Get;
			}
			else
			{
				error(23);
			}
		}
		else if (type == LPARSY)
		{
			int para_nums = 0;
			Del_Get;
			if (type == RPARSY)
			{
				//no para
				Del_Get;
			}
			else
			{
				gen_exp();
				std::string _exp = get_exp();
				expression();
				m->insert("PUSH", "", "", _exp);
				para_nums++;
				type = types.front();
				while (type == COMMASY)
				{
					Del_Get;
					gen_exp();
					std::string _exp1 = get_exp();
					expression();
					type = types.front();
					m->insert("PUSH", "", "", _exp1);
					para_nums++;
				}
				if (type == RPARSY)
				{
					//have para
					Del_Get;
				}
				else
				{
					error(17);
				}
			}
			//CALL / OP2 func
			if (table->glo_find(_ident) == nullptr)
			{
				error(27);
			}
			else if (table->glo_find(_ident)->type != FUNC_R_char && table->glo_find(_ident)->type != FUNC_R_int)
			{
				error(36);
			}
			else if (table->glo_find(_ident)->para_num != para_nums)
			{
				error(34);
			}
			else
			{
				m->insert("CALL", "", fac_r, _ident);
			}
		}
		else
		{
			m->insert("ASSIGN", _ident, "", fac_r);
		}
	}
	else if (type == SIGNEDINT || type == PLUSSY || type == MINUSSY || type == UNSIGHEDINT)
	{
		if (type == SIGNEDINT || type == UNSIGHEDINT)
		{
			//TODO: record num.
			m->insert("ASSIGN", symbols.front(), "", fac_r);
		}
		else
		{
			//TODO: record symbol
			bool isM = false;
			if (type == MINUSSY)
			{
				isM = true;
			}
			Del_Get;
			if (type == UNSIGHEDINT)
			{
				//TODO: record num.
				int num = strTOnum(symbols.front());
				if (isM)
				{
					num = -num;
				}
				m->insert("ASSIGN", numTOstrs(num), "", fac_r);
			}
			else
			{
				error(25);
			}
		}
		Del_Get;
	}
	else if (type == _CHAR)
	{
		//TODO: record char.
		int num = symbols.front().c_str()[0];
		m->insert("ASSIGN", numTOstrs(num), "", fac_r);
		Del_Get;
	}
	else if (type == LPARSY)
	{
		Del_Get;
		gen_exp();
		std::string _exp = get_exp();
		expression();
		type = types.front();
		if (type == RPARSY)
		{
			m->insert("ASSIGN", _exp, "", fac_r);
			Del_Get;
		}
		else
		{
			error(17);
		}
	}
	else
	{
		//Unknown factor.
		error(30);
	}
}

void parser::_integer()
{
	int type = types.front();
	if (type == PLUSSY || type == MINUSSY)
	{
		int PM = type;
		Del_Get;
		if (type == UNSIGHEDINT)
		{
			//TODO: record num
			int num = strTOnum(symbols.front());
			if (PM == MINUSSY)
			{
				num = -num;
			}
			num_value = num;
			Del_Get;
		}
		else
		{
			error(25);
			Del_Get;
		}
	}
	else if (type == UNSIGHEDINT)
	{
		//record num
		num_value = strTOnum(symbols.front());
		Del_Get;
	}
	else if (type == SIGNEDINT)
	{
		//TODO: record 0
		num_value = 0;
		Del_Get;
	}
	else
	{
		error(25);
	}
}

void parser::value_paras() 
{
	if (types.front() == RPARSY)
	{
		return;
	}
	expression();
	while (types.front() == COMMASY)
	{
		Del;
		getsym();
		expression();
	}
}

//already get 2 words
void parser::funcre_call()
{
	int type = types.front();
	if (type == IDENT)
	{
		Del;
		type = types.front();
		if (type == LPARSY)
		{
			Del_Get;
		}
		else
		{
			error(16);
		}
		value_paras();
		if (types.front() == RPARSY)
		{
			Del_Get;
		}
		else
		{
			error(17);
		}
	}
	else
	{
		error(18);
	}
}

//already get 2 words
void parser::funcvo_call()
{
	int type = types.front();
	if (type == IDENT)
	{
		Del;
		type = types.front();
		if (type == LPARSY)
		{
			Del_Get;
		}
		else
		{
			error(16);
		}
		value_paras();
		if (types.front() == RPARSY)
		{
			Del_Get;
		}
		else
		{
			error(17);
		}
	}
	else
	{
		error(18);
	}
}
/*
process to 18
*/
void parser::error(int type)
{
	errs++;
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
		reason = "Should be ‘const’(not useful)";
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
		while (types.front() != SEMISY)
		{
			Del;
			getsym();
		}
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
	case 36:
		reason = "Should be a function with return value.";
		break;
	case 37:
		reason = "Should be a variable.";
		break;
	default:
		break;
	}
	std::cout << "-------------------------------\n";
	std::cout << "Find error! E:\n" << type << ":" << reason <<"\nInformation:\n" << "symbol: " << symbols.front() << "in line: " << s->getline() <<"col: " << s->getcol() <<"\n" << "Symbol: " << types.front() << "\n";
	std::cout << "-------------------------------\n";
	system("pause");
}

void parser::analysis(int need)
{
	this->need = need;
	while (!getsym())
	{

	}
	program();
}

parser::parser(std::string file_name)
{
	this->file_name = file_name;
	s = new lexical_analysis(file_name);
	table = new Symbol_table();
	m = new midcode();
	data_addr = 0;
}


parser::~parser()
{
	delete(s);
	delete(table);
}
