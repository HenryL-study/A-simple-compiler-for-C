#pragma once

#ifndef TYPES_TOOL_H
#define TYPES_TOOL_H

#include <sstream>
#include <string>
const int IDENT = 1, UNSIGHEDINT = 2, SIGNEDINT = 3, _STRING = 4, _CHAR = 5, INTSY = 6, CHARSY = 7, CONSTSY = 8, VOIDSY = 9, MAINSY = 10;
const int PLUSSY = 11, MINUSSY = 12, MULSY = 13, DIVSY = 14, BIGGERSY = 15, LESSSY = 16, ASSIGHSY = 17, EQUSY = 18, NOTEQU = 19, BIG_EQU = 20, LESS_EQU = 21;
const int COMMASY = 22, COLONSY = 23, SEMISY = 24, LPARSY = 25, RPARSY = 26, LBRAKET = 27, RBRAKET = 28, LBRACE = 29, RBRACE = 30;
const int IFSY = 31, ELSESY = 32, WHILESY = 33, SWITCHSY = 34, CASESY = 35, DEFAULTSY = 36, RETURNSY = 37, SCANFSY = 38, PRINTFSY = 39, COMMENT = 40;

inline int strTOnum(std::string str)
{
	int i;
	std::stringstream ss;
	ss << str;
	ss >> i;
	return i;
}

inline std::string numTOstrs(int num)
{
	std::string i;
	std::stringstream ss;
	ss << num;
	ss >> i;
	return i;
}

#endif