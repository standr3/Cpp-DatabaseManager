#include "Table.h"

Table::Table(std::string name, vector<pair<string, int>> colNames) 
	: name(name) 
{
	for (int i = 0; i < colNames.size(); i++) 
	{
		appendColumn(colNames[i].first, colNames[i].second);
	}
}
Table::Table(const Table& table) 
	: name(table.name) 
{
	this->stringCol.clear();
	for (int i = 0; i < table.stringCol.size(); i++)
		this->stringCol.push_back(table.stringCol[i]);
	this->numberOfRecordings = table.numberOfRecordings;
}
void Table::remove(int index) 
{
	for (int i = 0; i < this->stringCol.size(); i++) 
	{
		this->stringCol[i].remove(index);
	}
}
Table& Table::operator=(const Table& table) 
{
	if (this != &table)
	{
		this->name = table.name;
		this->stringCol.clear();
		for (int i = 0; i < table.stringCol.size(); i++)
			this->stringCol.push_back(table.stringCol[i]);
		this->numberOfRecordings = table.numberOfRecordings;
	}
	return *this;
}
int Table::getSize() 
{
	return this->stringCol.size();
}
void Table::SetNumberOfRecordings(int n) 
{
	this->numberOfRecordings = n;
}
const std::string Table::getName() 
{
	return this->name;
}
void Table::setName(std::string name) 
{
	this->name = name;
}
int Table::getNumberOfRecordings() 
{
	return this->numberOfRecordings;
}
vector<pair<string, int>> Table::getColumnNamesAndTypes() 
{
	vector<pair<string, int>> r;
	for (int i = 0; i < this->stringCol.size(); i++) 
	{
		r.push_back(pair<string, int>(this->stringCol[i].getName(), this->stringCol[i].getType()));
	}
	return r;
}

bool Table::containsColumn(std::string col, int* index) 
{
	for (int i = 0; i < this->stringCol.size(); i++) 
	{
		if (this->stringCol[i].getName() == col) 
		{
			*index = i;
			return true;
		}
	}
	return false;
}
bool Table::containsColumn(std::string col) 
{
	for (int i = 0; i < this->stringCol.size(); i++) 
	{
		if (this->stringCol[i].getName() == col) 
		{
			return true;
		}
	}
	return false;
}

void Table::appendColumn(std::string colName, int type) 
{ 
	//0 - int; 1 - float; 2 - string
	if (containsColumn(colName))
		throw std::invalid_argument("Exista deja o coloana cu acest nume in aceasta tabela");
	else if (type > 2 && type < 0) 
	{
		throw std::invalid_argument("Tip de date necunoscut");
	}
	else 
	{
		ColumnString a;
		a.setType(type);
		a.setName(colName);
		this->stringCol.push_back(a);
	}
}
void Table::appendColumn(ColumnString col) 
{
	this->stringCol.push_back(col);
}
void Table::appendInto(std::string colName, std::string data) 
{ 
	//0 - int; 1 - float; 2 - string
	int index = 0;
	if (containsColumn(colName, &index)) {
		try {
			this->stringCol[index].append(data);
		}
		catch (std::exception e) {
			std::cout << e.what();
		}
	}
	else
	{
		throw std::invalid_argument("Coloana nu exista.");
	}
}

void Table::appendRecording(Recording recording) {
	if (recording.getSize() != this->stringCol.size())
	{
		throw std::invalid_argument("Inregistrarea contine un numar de valori diferit de numarul coloanelor din aceasta tabela");
	}
	else 
	{
		for (int i = 0; i < this->stringCol.size(); i++) 
		{
			try 
			{
				appendInto(recording.getColumnName(i), recording.getColumnValue(i));
			}
			catch (std::exception e) 
			{
				std::cout << e.what();
			}
		}
		this->numberOfRecordings++;
	}
}
Recording Table::getRecording(int index) 
{
	if (index >= 0 && index < this->stringCol[0].getSize()) 
	{
		Recording r;
		for (int i = 0; i < this->stringCol.size(); i++)
			r.append(this->stringCol[i].getName(), (this->stringCol[i][index]));
		return r;
	}
	else
	{
		throw std::invalid_argument("Index invalid");
	}
}

ColumnString& Table::getColumn(std::string columnName) 
{
	int index = 0;
	if (containsColumn(columnName, &index))
		return (this->stringCol[index]);
	else
		throw std::exception("Coloana nu exista");
}
ColumnString& Table::operator[](int i) 
{
	if (i < this->stringCol.size() && i >= 0) 
		return (this->stringCol[i]);
	else
		throw std::invalid_argument("Index gresit: " + to_string(i) + " " + to_string(this->stringCol.size()));
}
std::ostream& operator<<(ostream& out, Table& table) 
{
	int dim = table.numberOfRecordings;
	vector<pair<string, int>> colNamesAndTypes = table.getColumnNamesAndTypes();
	for (int i = 0; i < table.stringCol.size(); i++)
		out << std::setw(20) << colNamesAndTypes[i].first;
	out << endl;

	for (int index = 0; index < dim; index++) 
	{
		for (int i = 0; i < table.stringCol.size(); i++) 
		{
			out << setw(20) << table[i][index] << std::setw(20);
		}
		out << endl;
	}
	return out;
}
void Table::print() {
	std::cout << "Nume tabela: " << this->name << std::endl;
	std::cout << "Numar inregistrari: " << this->numberOfRecordings << std::endl;
	std::cout << "Numar coloane: " << this->stringCol.size() << std::endl;
	std::cout << "Coloane: " << std::endl;

	for (int i = 0; i < this->stringCol.size(); i++) {
		std::cout << this->stringCol[i].getName() << " ";
		if (this->stringCol[i].getType() == 0)
			std::cout << "int";
		else if (this->stringCol[i].getType() == 1)
			std::cout << "float";
		else if (this->stringCol[i].getType() == 2)
			std::cout << "string";
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for (int i = 0; i < this->stringCol.size(); i++) {
		this->stringCol[i].print();
		std::cout << std::endl;
	}
	std::cout << std::endl << "====================" << std::endl;
}

Table& Table::appendTable(Table& t) {
	if (t.getSize() != this->getSize())
		throw exception("tabelele au un numar diferit de coloane");
	else {
		Table r = *this;
		for (int i = 0; i < t.getNumberOfRecordings(); i++)
			r.appendRecording(t.getRecording(i));
		return r;
	}

}

Table& Table::operator+(Recording recording) {
	if (recording.getSize() != this->stringCol.size())
	{
		throw std::invalid_argument("Inregistrarea contine un numar de valori diferit de numarul coloanelor din aceasta tabela");
	}
	else 
	{
		for (int i = 0; i < this->stringCol.size(); i++) 
		{
			try 
			{
				appendInto(recording.getColumnName(i), recording.getColumnValue(i));
			}
			catch (std::exception e) 
			{
				std::cout << e.what();
			}
		}
		this->numberOfRecordings++;
	}
	return *this;
}

