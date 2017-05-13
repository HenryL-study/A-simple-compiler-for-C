#include "stdafx.h"
#include "lexical_analysis.h"


bool lexical_analysis::isLetter(char ch)
{
	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z') || (ch == '_'))
		return true;
	else
		return false;
}

bool lexical_analysis::isColon(char ch)
{
	if (ch == ':')
		return true;
	else
		return false;
}

bool lexical_analysis::isBigger(char ch)
{
	if (ch == '>')
		return true;
	else
		return false;
}

bool lexical_analysis::isLess(char ch)
{
	if (ch == '<')
		return true;
	else
		return false;
}

bool lexical_analysis::isSurprise(char ch)
{
	if (ch == '!')
		return true;
	else
		return false;
}

bool lexical_analysis::isEqu(char ch)
{
	if (ch == '=')
		return true;
	else
		return false;
}

bool lexical_analysis::isPlus(char ch)
{
	if (ch == '+')
		return true;
	else
		return false;
}

bool lexical_analysis::isMinus(char ch)
{
	if (ch == '-')
		return true;
	else
		return false;
}

bool lexical_analysis::isMul(char ch)
{
	if (ch == '*')
		return true;
	else
		return false;
}

bool lexical_analysis::isLpar(char ch)
{
	if (ch == '(')
		return true;
	else
		return false;
}

bool lexical_analysis::isRpar(char ch)
{
	if (ch == ')')
		return true;
	else
		return false;
}

bool lexical_analysis::isLbraket(char ch)
{
	if (ch == '[')
		return true;
	else
		return false;
}

bool lexical_analysis::isRbraket(char ch)
{
	if (ch == ']')
		return true;
	else
		return false;
}

bool lexical_analysis::isLbrace(char ch)
{
	if (ch == '{')
		return true;
	else
		return false;
}

bool lexical_analysis::isRbrace(char ch)
{
	if (ch == '}')
		return true;
	else
		return false;
}

bool lexical_analysis::isComma(char ch)
{
	if (ch == ',')
		return true;
	else
		return false;
}

bool lexical_analysis::isSemi(char ch)
{
	if (ch == ';')
		return true;
	else
		return false;
}

bool lexical_analysis::isDivi(char ch)
{
	if (ch == '/')
		return true;
	else
		return false;
}

bool lexical_analysis::isSquote(char ch)
{
	if (ch == '\'')
		return true;
	else
		return false;
}

bool lexical_analysis::isDquote(char ch)
{
	if (ch == '\"')
		return true;
	else
		return false;
}

int lexical_analysis::reserver(std::string word)
{
	if (word == "int")
		return INTSY;
	else if (word == "char")
		return CHARSY;
	else if (word == "const")
		return CONSTSY;
	else if (word == "void")
		return VOIDSY;
	else if (word == "main")
		return MAINSY;
	else if (word == "if")
		return IFSY;
	else if (word == "else")
		return ELSESY;
	else if (word == "while")
		return WHILESY;
	else if (word == "switch")
		return SWITCHSY;
	else if (word == "case")
		return CASESY;
	else if (word == "default")
		return DEFAULTSY;
	else if (word == "return")
		return RETURNSY;
	else if (word == "scanf")
		return SCANFSY;
	else if (word == "printf")
		return PRINTFSY;
	else
		return 0;
}

int lexical_analysis::transNum(std::string num)
{
	int i;
	std::stringstream ss;
	ss << num;
	ss >> i;
	return i;
}

/*
1: Number begin with 0
2: Unknown Symbol
3: Missing '
4: Missing "
5: Unknown char
6: Unknown string
7: Missing =
*/
int lexical_analysis::error(int type)
{
	//std::cout << "ERROR!!!  \t" << type << "\n";
	error_type = type;
	return 1;
}
unsigned int lexical_analysis::getline()
{
	return line;
}
unsigned int lexical_analysis::getcol()
{
	return location;
}
/*
-1: file end.
0: Normal get one word.
1: FIND ERROR.
*/
int lexical_analysis::step_once()
{
	if (file_status != FILE_READING)
	{
		return -1;
	}
	int status = getsym(word_s);
	if (status == 2)
	{
		return 0;
	}
	else if (status == 1)
	{
		return 1;
	}
	else
	{
		while (status == 0)
		{
			if (!cfile.eof())
			{
				cfile.getline(word, 1024);
				word_s = std::string(word);
				status = getsym(word_s);
			}
			else
			{
				file_status = FILE_END;
				return -1;
			}
		}
		if (status == 1)
			return 1;
		else
			return 0;
	}
}

/*
0: Normal end.
1: FIND ERROR.
2: Not end.
*/
int lexical_analysis::getsym(std::string words)
{
	char ch = ' ';
	token = "";
	//location = 0;
	if (words.length() < location)
	{
		location = 0;
		line++;
		return 0;
	}
	while (isspace(ch))
	{
		ch = words[location];
		location++;
		if (words.length() < location)
		{
			location = 0;
			line++;
			return 0;
		}

	}
	if (isLetter(ch))
	{
		while (isLetter(ch) || isdigit(ch))
		{
			token.append(std::string(1, ch));
			ch = words[location];
			location++;
		}
		location--;
		int resultValue = reserver(token);
		if (resultValue == 0)
			symbol = IDENT;
		else
			symbol = resultValue;
	}
	else if (isdigit(ch))
	{
		bool zero_begin = false;
		if (ch == '0')
		{
			zero_begin = true;
		}
		while (isdigit(ch))
		{
			token.append(std::string(1, ch));
			ch = words[location];
			location++;
		}
		location--;
		if ((token.length()>1) && zero_begin)
		{
			error(1);
			return 2;
		}
		if (token.length() == 1 && zero_begin)
		{
			symbol = SIGNEDINT;
		}
		else
		{
			symbol = UNSIGHEDINT;
		}
		num = transNum(token);
	}
	else if (isColon(ch)) //:
		symbol = COLONSY;
	else if (isPlus(ch))
		symbol = PLUSSY;
	else if (isMinus(ch))
		symbol = MINUSSY;
	else if (isMul(ch))
		symbol = MULSY;
	else if (isBigger(ch))
	{
		ch = words[location];
		if (isEqu(ch))
		{
			location++;
			symbol = BIG_EQU;
		}
		else
			symbol = BIGGERSY;
	}
	else if (isLess(ch))
	{
		ch = words[location];
		if (isEqu(ch))
		{
			location++;
			symbol = LESS_EQU;
		}
		else
			symbol = LESSSY;
	}
	else if (isEqu(ch))
	{
		ch = words[location];
		if (isEqu(ch))
		{
			location++;
			symbol = EQUSY;
		}
		else
			symbol = ASSIGHSY;
	}
	else if (isSurprise(ch))
	{
		ch = words[location];
		if (isEqu(ch))
		{
			location++;
			symbol = NOTEQU;
		}
		else
			error(7);
	}
	else if (isLpar(ch))
		symbol = LPARSY;
	else if (isRpar(ch))
		symbol = RPARSY;
	else if (isLbraket(ch))
		symbol = LBRAKET;
	else if (isRbraket(ch))
		symbol = RBRAKET;
	else if (isLbrace(ch))
		symbol = LBRACE;
	else if (isRbrace(ch))
		symbol = RBRACE;
	else if (isComma(ch))
		symbol = COMMASY;
	else if (isSemi(ch))
		symbol = SEMISY;
	else if (isDivi(ch))
	{
		ch = words[location];
		location++;
		if (isMul(ch))
		{
			do
			{
				do
				{
					ch = words[location];
					location++;
				} while (!isMul(ch));
				do
				{
					ch = words[location];
					location++;
					if (isDivi(ch))
					{
						symbol = COMMENT;
						return 2;
					}
				} while (isMul(ch));
			} while (!isMul(ch));
		}
		else
		{
			location--;
			symbol = DIVSY;
		}
	}
	else if (isSquote(ch))
	{
		ch = words[location];
		location++;
		if (isdigit(ch) || isLetter(ch) || isPlus(ch) || isMinus(ch) || isMul(ch) || isDivi(ch))
		{
			token.append(std::string(1, ch));
			ch = words[location];
			location++;
			if (isSquote(ch))
			{
				symbol = _CHAR;
			}
			else
			{
				location--;
				error(3);
			}
		}
		else
		{
			location--;
			error(5);
		}
	}
	else if (isDquote(ch))
	{
		ch = words[location];
		location++;
		while (!isDquote(ch))
		{
			if (words.length() < location)
			{
				error(4);
				location = 0;
				line++;
				return 1;
			}
			else
			{
				if (ch == 32 || ch == 33 || (ch >= 35 && ch <= 126))
				{
					token.append(std::string(1, ch));
					ch = words[location];
					location++;
				}
				else
				{
					location--;
					error(6);
					return 2;
				}
			}

		}
		symbol = _STRING;
	}
	else
	{
		error(2);
		return 1;
	}

	return 2;
}

void lexical_analysis::print_sym(std::ofstream &outfile)
{

	using namespace std;
	if (symbol == UNSIGHEDINT || symbol == SIGNEDINT)
		if (symbol == UNSIGHEDINT)
			outfile << "UNSIGHEDINT" << " \t\t\t" << num << endl;
		else
			outfile << "SIGNEDINT" << " \t\t\t" << num << endl;
	else
	{
		switch (symbol)
		{
		case IDENT:
			outfile << "IDENT" << " \t\t\t" << token << endl;
			break;
		case _STRING:
			outfile << "_STRING" << " \t\t\t" << token << endl;
			break;
		case _CHAR:
			outfile << "_CHAR" << " \t\t\t" << token << endl;
			break;
		case INTSY:
			outfile << "INTSY" << " \t\t\t" << "int" << endl;
			break;
		case CHARSY:
			outfile << "CHARSY" << " \t\t\t" << "char" << endl;
			break;
		case CONSTSY:
			outfile << "CONSTSY" << " \t\t\t" << "const" << endl;
			break;
		case VOIDSY:
			outfile << "VOIDSY" << " \t\t\t" << "void" << endl;
			break;
		case MAINSY:
			outfile << "MAINSY" << " \t\t\t" << "main" << endl;
			break;
		case PLUSSY:
			outfile << "PLUSSY" << " \t\t\t" << "+" << endl;
			break;
		case MINUSSY:
			outfile << "MINUSSY" << " \t\t\t" << "-" << endl;
			break;
		case MULSY:
			outfile << "MULSY" << " \t\t\t" << "*" << endl;
			break;
		case DIVSY:
			outfile << "DIVSY" << " \t\t\t" << "/" << endl;
			break;
		case BIGGERSY:
			outfile << "BIGGERSY" << " \t\t\t" << ">" << endl;
			break;
		case LESSSY:
			outfile << "LESSSY" << " \t\t\t" << "<" << endl;
			break;
		case ASSIGHSY:
			outfile << "ASSIGHSY" << " \t\t\t" << "=" << endl;
			break;
		case EQUSY:
			outfile << "EQUSY" << " \t\t\t" << "==" << endl;
			break;
		case NOTEQU:
			outfile << "NOTEQU" << " \t\t\t" << "!=" << endl;
			break;
		case BIG_EQU:
			outfile << "BIG_EQU" << " \t\t\t" << ">=" << endl;
			break;
		case LESS_EQU:
			outfile << "LESS_EQU" << " \t\t\t" << "<=" << endl;
			break;
		case COMMASY:
			outfile << "COMMASY" << " \t\t\t" << "," << endl;
			break;
		case COLONSY:
			outfile << "COLONSY" << " \t\t\t" << ":" << endl;
			break;
		case SEMISY:
			outfile << "SEMISY" << " \t\t\t" << ";" << endl;
			break;
		case LPARSY:
			outfile << "LPARSY" << " \t\t\t" << "(" << endl;
			break;
		case RPARSY:
			outfile << "RPARSY" << " \t\t\t" << ")" << endl;
			break;
		case LBRAKET:
			outfile << "LBRAKET" << " \t\t\t" << "[" << endl;
			break;
		case RBRAKET:
			outfile << "RBRAKET" << " \t\t\t" << "]" << endl;
			break;
		case LBRACE:
			outfile << "LBRACE" << " \t\t\t" << "{" << endl;
			break;
		case RBRACE:
			outfile << "RBRACE" << " \t\t\t" << "}" << endl;
			break;
		case COMMENT:
			outfile << "COMMENT" << " \t\t\t" << "COMMENT" << endl;
			break;
		case IFSY:
			outfile << "IFSY" << " \t\t\t" << token << endl;
			break;
		case ELSESY:
			outfile << "ELSESY" << " \t\t\t" << token << endl;
			break;
		case WHILESY:
			outfile << "WHILESY" << " \t\t\t" << token << endl;
			break;
		case SWITCHSY:
			outfile << "SWITCHSY" << " \t\t\t" << token << endl;
			break;
		case CASESY:
			outfile << "CASESY" << " \t\t\t" << token << endl;
			break;
		case DEFAULTSY:
			outfile << "DEFAULTSY" << " \t\t\t" << token << endl;
			break;
		case RETURNSY:
			outfile << "RETURNSY" << " \t\t\t" << token << endl;
			break;
		case SCANFSY:
			outfile << "SCANFSY" << " \t\t\t" << token << endl;
			break;
		case PRINTFSY:
			outfile << "PRINTFSY" << " \t\t\t" << token << endl;
			break;
		default:
			outfile << "????" << " \t\t\t" << token << endl;
			break;
		}
	}
}

int lexical_analysis::getnum()
{
	return num;
}

int lexical_analysis::getsymbol()
{
	return symbol;
}

std::string lexical_analysis::gettoken()
{
	return token;
}



lexical_analysis::lexical_analysis(std::string file)
{
	cfile.open(file);
	if (!cfile.is_open())
	{
		file_status = FILE_NO_FIND;
	}
	else if (cfile.eof())
	{
		file_status = FILE_END;
	}
	else
	{
		file_status = FILE_READING;
	}
	cfile.getline(word, 1024);
	word_s = std::string(word);
	this->token = "";
	this->error_type = -1;
	this->location = 0;
	this->line = 0;
}


lexical_analysis::~lexical_analysis()
{
	cfile.close();
}
