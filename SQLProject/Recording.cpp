#include "Recording.h"

Recording::Recording() {}
Recording::Recording(const Recording& recording) 
{
	this->values = recording.values;
}
Recording::Recording(int size, string* stringColNames, string* stringColValues) 
{
	for (int i = 0; i < size; i++) 
	{
		this->values.push_back(make_pair(stringColNames[i], stringColValues[i]));
	}
}
Recording& Recording::operator=(const Recording& recording) 
{
	if (this == &recording)
		return *this;
	else 
		this->values = recording.values;
}
void Recording::append(string colName, string data) 
{
	this->values.push_back(make_pair(colName, data));
}

void Recording::clear() 
{
	this->values.clear();
}

void Recording::print() 
{
	cout << "total number of entries: " << this->values.size();
	for (int i = 0; i < this->values.size(); i++) {
		cout << " " << this->values[i].second;
	}
	cout << endl;
}

int Recording::getSize() 
{
	return this->values.size();
}
string Recording::getColumnName(int index) 
{
	return this->values[index].first;
}

string Recording::getColumnValue(string ColumnName) 
{
	vector<pair<string, string>>::iterator i;
	for (i = this->values.begin(); i != this->values.end() && (*i).first != ColumnName; i++) { }
	if (i != this->values.end()) 
	{
		return (*i).second;
	}
}
string Recording::getColumnValue(int index) 
{
	return this->values[index].second;
}
string* Recording::operator[](int i) 
{
	if (i < this->values.size() && i >= 0) 
	{
		string r[2] { this->values[i].first , this->values[i].second };
		return r;
	}
	else
	{
		throw std::invalid_argument("Index gresit");
	}
}
void Recording::modify(int index, string withValue) 
{
	this->values[index].second = withValue;
}