#pragma once
#include <cstddef>
#include <string>
#include "Recording.h"
#include "Table.h"

class DataBase
{
private:
	std::string name = "";
	int size = 0;
	vector<Table> tables;
public:

	DataBase() {};
	DataBase(string name) :name(name) {}
	DataBase(string name, vector<Table> tables);
	DataBase(const DataBase& database);

	DataBase& operator=(const DataBase& database);
	void append();
	void append(Table& t);
	string getName();
	int getSize();
	Table& getTable(string tableName);
	Table& getTable(int index);
	bool operator()(string tableName, int* index);
	Table& operator[](int index);
	Table Select(string* columnNames, int numberOfColumns, Table& table);
	Table Where(Table& table, string col, string value);
	void Delete(Table& table, string col, string value);
	void Insert(Table& table, Recording recording);
	void Drop(string tableName);
	void Update(Table* table, string colCriteria, string colCriteriaValue, string colModify, string modifyWith);
	friend std::ostream& operator<<(std::ostream& out, DataBase& database);

};