#pragma once
//#include "BaseData.h"
#include <cstddef>
#include <string>
#include<iostream>
#include<vector>
using namespace std;
class Recording
{
private:
	vector<pair<string, string>> values;
	//int size = 0;
	//string* stringColNames = NULL;
	//string* stringColValues = NULL;

public:
	
	
	Recording();

	Recording(const Recording& recording);

	Recording(int size, string* stringColNames, string* stringColValues);

	Recording& operator=(const Recording& recording);

	void append(string colName, string data);

	void clear();

	void print();

	int getSize();

	string getColumnName(int index);

	string getColumnValue(string ColumnName);

	string getColumnValue(int index);

	string* operator[](int i);

	void modify(int index, string withValue);



};
