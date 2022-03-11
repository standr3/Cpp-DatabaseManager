#pragma once
#include <string>
std::string strToUpper(std::string s) 
{
	/* This method simply subtracts a value of 32 from
		the ASCII value of lowercase letter by Bitwise ANDing
		(&) with negation(~) of 32 converting the letter to uppercase. */
	for (char& c : s)
		c = c & ~' ';
	//cout << s << endl;
	return s;
}