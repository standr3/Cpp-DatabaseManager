#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include "ColumnString.h"
#include "Recording.h"

class Table
{
private:
	std::string name = "";
	vector<ColumnString> stringCol;
	int numberOfRecordings = 0;

public:
	Table() {}
	
	Table(std::string name) :name(name) {}
	
	Table(std::string name, vector<pair<string, int>> colNames);

	Table(const Table& table);

	void remove(int index);

	void appendRecording(Recording recording);

	Recording getRecording(int index);

	ColumnString& getColumn(std::string columnName);

	ColumnString& operator[](int i);

	void print();

	Table& appendTable(Table& t);

	Table& operator+(Recording recording);

	Table& operator=(const Table& table);

	void appendColumn(ColumnString col);

	void appendColumn(std::string colName, int type);

	void appendInto(std::string colName, std::string data);

	int getSize();

	void SetNumberOfRecordings(int n);

	const std::string getName();

	void setName(std::string name);

	int getNumberOfRecordings();

	vector<pair<string, int>> getColumnNamesAndTypes();

	bool containsColumn(std::string col, int* index);

	bool containsColumn(std::string col);

	friend std::ostream& operator<<(ostream& out, Table& table);
};
