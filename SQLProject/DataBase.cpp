#include "DataBase.h"

DataBase::DataBase(string name, vector<Table> tables) : name(name) {
	this->tables.clear();
	this->tables = tables;
}
DataBase::DataBase(const DataBase& database) :name(database.name) {
	this->tables.clear();
	this->tables = tables;

}
DataBase& DataBase::operator=(const DataBase& database) {
	if (this != &database) {
		this->name = name;

		this->tables.clear();
		this->tables = tables;
	}
	return *this;
}
void DataBase::append() {
	Table t;
	this->tables.push_back(t);
}
void DataBase::append(Table& t) { 
	int i = 0;
	for (i; i < this->tables.size() && this->tables[i].getName() != t.getName(); i++) {
	}
	if (i == this->tables.size()) {
		this->tables.push_back(t);
	}
	else {
		throw exception("Tabelul exista");
	}

}
string DataBase::getName() {
	return this->name;
}
int DataBase::getSize() {
	return this->tables.size();
}
Table& DataBase::getTable(string tableName) {
	bool found = false;
	for (int i = 0; i < this->tables.size(); i++) {
		if (this->tables[i].getName() == tableName) {
			found = true;
			return this->tables[i];
		}
	}
	if (found == false)
		throw exception("Tabela nu exista");
}
Table& DataBase::getTable(int index) {
	if (index > 0 && index < this->tables.size())
		return this->tables[index];
}
bool DataBase::operator()(string tableName, int* index) {
	for (int i = 0; i < this->tables.size(); i++) {
		if (this->tables[i].getName() == tableName) {
			*index = i;
			return true;
		}
	}
	return false;
}
Table& DataBase::operator[](int index) {
	return this->tables[index];
}
Table DataBase::Select(string* columnNames, int numberOfColumns, Table& table) {
	Table result = Table("Rezultat");
	for (int i = 0; i < numberOfColumns; i++) {
		result.appendColumn(table.getColumn(columnNames[i]));
	}
	result.SetNumberOfRecordings(table.getNumberOfRecordings());
	return result;
}

Table DataBase::Where(Table& table, string col, string value) {
	if (table.containsColumn(col)) 
	{
		Table r = Table("Rezultat", table.getColumnNamesAndTypes());
		ColumnString c = table.getColumn(col);
		for (int i = 0; i < c.getSize(); i++) 
		{
			if (c[i] == value) 
			{
				r = r + (table).getRecording(i);
			}
		}
		return r;
	}
	else
		throw invalid_argument("Coloana nu exista in aceasta tabela");
}

void DataBase::Delete(Table& table, string col, string value) 
{
	if (table.containsColumn(col)) 
	{
		ColumnString c = table.getColumn(col);
		int i = 0;
		while (i < c.getSize()) 
		{
			if (c[i] == value) 
			{
				c.remove(i);
				for (int j = 0; j < c.getSize(); j++) 
				{
					table[j].remove(i);
				}
				table.SetNumberOfRecordings(table.getNumberOfRecordings() - 1);
				i--;
			}
			i++;
		}
	}
	else
		throw invalid_argument("Coloana nu exista in aceasta tabela");
}

void DataBase::Insert(Table& table, Recording recording) 
{
	try 
	{
		table = table + recording;
	}
	catch (exception e) 
	{
		cout << e.what() << endl;
	}
}

void DataBase::Drop(string tableName)
{
	int index = 0;
	if ((*this)(tableName, &index))
		this->tables.erase(this->tables.begin() + index);
	else
		throw invalid_argument("Aceasta tabela nu se afla in baza de date");
}

void DataBase::Update(Table* table, string colCriteria, string colCriteriaValue, string colModify, string modifyWith) 
{
	int indexColCriteria, indexColModify;
	if ((*table).containsColumn(colCriteria, &indexColCriteria) && (*table).containsColumn(colModify, &indexColModify)) 
	{
		for (int i = 0; i < ((*table).getColumn(colCriteria)).getSize(); i++)
		{
			cout << endl << "i: " << i;
			if ((*table)[indexColCriteria][i] == colCriteriaValue)
			{
				cout << "if";
				(*table)[indexColModify][i] = modifyWith;
			}
		}
	}
	else
	{
		throw invalid_argument("Tabela nu are coloanele specificate");
	}
}

std::ostream& operator<<(std::ostream& out, DataBase& database) 
{
	out << "Nume Baza de date: " << database.name << endl;
	for (int i = 0; i < database.tables.size(); i++)
		out << database.tables[i] << endl;
	return out;
}