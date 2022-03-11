#pragma once
#include <iostream>
#include <string>
#include <vector>

class ColumnString
{
private:
	std::string name = "";
	std::vector<std::string> col;
	std::string defaultValue = "";
	int dataType = 0; //0 - int , 1 - float, 2 - string

public:
//	~ColumnString() {}
	ColumnString() {}
	ColumnString(std::string name) : name(name) {}
	

	ColumnString(std::string name, int size, std::string defaultValue, int dataType);

	ColumnString(const ColumnString& column);
	ColumnString& operator=(const ColumnString& column);
	void append(std::string data);
	int getSize() const;
	void setName(std::string name);
	const std::string getName() const;
	void setType(int type);
	int getType() const;
	bool contains(std::string value);
	void remove(int index);
	std::string getDefaultValue();
	void setDefaultValue(std::string value);
	operator float();
	ColumnString operator!();
	bool operator==(ColumnString col);
	void print();
	std::string& operator[](int i);
};