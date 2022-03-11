#include "ColumnString.h"
#include <string>
#include <vector>

using namespace std;

ColumnString::ColumnString(std::string name, int size, std::string defaultValue, int dataType) :name(name) {
	if (dataType > 2 || dataType < 0) {
		this->dataType = 0;
	}
	else
		this->dataType = dataType;
	this->col.reserve(size);
	for (int i = 0; i < size; i++)
		this->col.push_back(defaultValue);
	this->defaultValue = defaultValue;
}
ColumnString::ColumnString(const ColumnString& column) {
	this->name = column.name;
	this->defaultValue = column.defaultValue;
	this->dataType = column.dataType;
	this->col.reserve(column.col.size());
	for (int i = 0; i < column.col.size(); i++)
		this->col.push_back(column.col[i]);
}
ColumnString& ColumnString::operator=(const ColumnString& column) {
	if (this == &column)
		return *this;
	else {
		this->name = column.name;
		this->defaultValue = column.defaultValue;
		this->dataType = column.dataType;
		this->col.clear();
		this->col.reserve(column.col.size());
		for (int i = 0; i < column.col.size(); i++)
			this->col.push_back(column.col[i]);
	}
}
void ColumnString::append(std::string data) {
	this->col.push_back(data);
}
int ColumnString::getSize() const {
	return this->col.size();
}
const string ColumnString::getName() const {
	return this->name;
}
void ColumnString::setName(std::string name) {
	this->name = name;
}
void ColumnString::setType(int type) {
	if (type > 2 || type < 0)
		throw std::invalid_argument("Type does not exist");
	else
		this->dataType = type;
}
int ColumnString::getType() const {
	return this->dataType;
}
bool ColumnString::contains(std::string value) {
	if (std::find(this->col.begin(), this->col.end(), value) != this->col.end()) {
		return true;
	}

	return false;
}
void ColumnString::remove(int index) {
	try {
		this->col.erase(this->col.begin() + index);
	}
	catch (exception e) {
		cout << e.what() << endl;
	}

}
std::string ColumnString::getDefaultValue() {
	return this->defaultValue;
}
void ColumnString::setDefaultValue(std::string value) {
	if (this->dataType == 2)
		this->defaultValue = value;
	else if (this->dataType == 1) {
		try
		{
			float v = std::stod(value);
			this->defaultValue = value;
		}
		catch (std::exception& e)
		{
			std::cout << "Incorrect default value;" << std::endl;
		}
	}
	else if (this->dataType == 0) {
		try
		{
			int v = std::stoi(value);
			this->defaultValue = value;
		}
		catch (std::exception& e)
		{
			std::cout << "Incorrect default value;" << std::endl;
		}
	}
}

string& ColumnString::operator[](int i) {
	try {
		return this->col[i];
	}
	catch (exception e) {
		cout << e.what() << endl;
	}

}
ColumnString::operator float() {
	if (this->dataType == 0 || this->dataType == 1) {
		float sum = 0;
		for (int i = 0; i < this->col.size(); i++) {
			sum = sum + stof(this->col[i]);
		}
		return sum / this->col.size();
	}
	else
		return -1;
}
ColumnString ColumnString::operator!() {
	ColumnString r = *this;
	r.setName(r.getName() + " Negat");
	for (int i = 0; i < r.getSize(); i++)
		r.col[i] = r.defaultValue;
	return r;
}
bool ColumnString::operator==(ColumnString col) {
	if (this->col.size() != (col).getSize())
		return false;
	for (int i = 0; i < this->col.size(); i++) {
		if (this->col[i] != (col).col[i])
			return false;
	}
	return true;
}
void ColumnString::print() {
	for (int i = 0; i < this->col.size(); i++) {
		std::cout << " " << this->col[i];
	}
}