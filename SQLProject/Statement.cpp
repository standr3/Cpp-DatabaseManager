#include "Statement.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "DataBase.h"
#include "IOFile.h"
#include "Utils.h"

using namespace statement;
using std::string;
using std::vector;
using std::pair;

bool Statement::verbose = false;

/************************************************************************************************************ HELP CLASS **/
Help::Help() {}
void Help::run(DataBase& db)
{

	cout << "CREATE  TABLE TABLE_NAME ((COL_NAME, COL_TYPE, SIZE, DEF_VAL), ...);" << endl;
	cout << "SELECT  ALL/(COL_NAME1, ...) FROM TABLE_NAME [WHERE COL_NAME = VALUE];" << endl;
	cout << "UPDATE  TABLE_NAME SET COL_NAME1 = VALUE WHERE COL_NAME2 = VALUE;" << endl;
	cout << "DELETE  FROM  TABLE_NAME WHERE COL_NAME1 = VALUE; " << endl;
	cout << "INSERT  INTO  TABLE_NAME (VALUE, ...);" << endl;
	cout << "DISPLAY TABLE TABLE_NAME;" << endl;
	cout << "DROP    TABLE TABLE_NAME;" << endl;

}
void Help::print()
{
	if (Statement::verbose)
	{
		cout << "Statement::Help" << endl;

	}
}
/***************************************************************************************************** CREATETABLE CLASS **/
// string            table_name;
// vector<ColumnDef> col_defs;
//
CreateTable::CreateTable()
{}
CreateTable::CreateTable(string _tname, vector<ColumnDef> _cdef)
{
	table_name = _tname;
	col_defs = _cdef;
}
void CreateTable::run(DataBase& db)
{
	try {
		Table t = Table(table_name);
		ColumnString s = ColumnString();
		for (int i = 0; i < col_defs.size(); i++) {
			s.setName(col_defs[i].name);
			if (strToUpper(col_defs[i].type) == "FLOAT")
				s.setType(1);
			else if (strToUpper(col_defs[i].type) == "STRING" || strToUpper(col_defs[i].type) == "TEXT")
				s.setType(2);
			else
				s.setType(0);
			s.setDefaultValue(col_defs[i].default_value);
			t.appendColumn(s);
		}
		db.append(t);
		IOFile::databaseDescriptionToText(db);
		IOFile::tableToBinary(t);
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}
void CreateTable::print()
{
	if (Statement::verbose)
	{
		cout << "Statement::Create table: " << table_name << endl;
		for (ColumnDef cd : col_defs)
		{
			cout << cd.name << " ";
			cout << cd.type << " ";
			cout << cd.dim << " ";
			cout << cd.default_value << " ";
			cout << endl;
		}
	}
}

/******************************************************************************************************* DROPTABLE CLASS **/
DropTable::DropTable() {}
DropTable::DropTable(string _tName)
{
	table_name = _tName;
}
void DropTable::run(DataBase& db)
{
	try {
		db.Drop(table_name);
		IOFile::databaseDescriptionToText(db);
		string filename = table_name + ".txt";
		remove(filename.data());
		filename = table_name + ".tab";
		remove(filename.data());
	}
	catch (exception e) {
		cout << "Table does not exist in order to drop it!\n";
	}
}
void DropTable::print()
{
	if (Statement::verbose)
	{
		cout << "Statement::Drop table: " << table_name << endl;
	}
};

/**************************************************************************************************** DISPLAYTABLE CLASS **/
DisplayTable::DisplayTable() {}
DisplayTable::DisplayTable(string _tName)
{
	table_name = _tName;
}
void DisplayTable::run(DataBase& db)
{
	try {
		Table t = db.getTable(table_name);
		cout << t << endl;
		t.setName("Display_" + std::to_string(statement::DisplayTable::dNumber++));
		IOFile::tableToText(t);
	}
	catch (exception e) {
		cout << "Table does not exist in order to display it!\n";
	}

}
void DisplayTable::print()
{
	if (Statement::verbose)
	{
		cout << "Statement::Display table: " << table_name << endl;
	}
};

/**************************************************************************************************** INSERTTABLE CLASS **/
InsertInto::InsertInto() {}
InsertInto::InsertInto(string _tName, vector<pair<string, string>> _values)
{
	table_name = _tName;
	values = _values;
}
void InsertInto::run(DataBase& db)
{
	try {
		Recording r;
		vector<pair<string, int>> v = db.getTable(table_name).getColumnNamesAndTypes();
		for (int i = 0; i < v.size(); i++) {
			r.append(v[i].first, values[i].first);
			//cout << "first: " << v[i].first << " second: " << values[i].first << endl;
		}
		db.Insert(db.getTable(table_name), r);
		IOFile::databaseDescriptionToText(db);
		IOFile::tableToBinary(db.getTable(table_name));
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
	
}
void InsertInto::print()
{
	if (Statement::verbose)
	{
		cout << "Statement::Insert into table: " << table_name << endl;
		cout << "Values: " << endl;
		for (pair<string, string> v : values)
		{
			cout << " > " << "[" << v.first << ", " << v.second << "]\n";
		}
	}
}

/**************************************************************************************************** DELETETABLE CLASS **/
DeleteFrom::DeleteFrom() {};
DeleteFrom::DeleteFrom(string table_name, string col_name, string value)
{
	this->table_name = table_name;
	this->col_name = col_name;
	this->value = value;
}
void DeleteFrom::run(DataBase& db)
{
	try {
		db.Delete(db.getTable(table_name), col_name, value);
		IOFile::databaseDescriptionToText(db);
		IOFile::tableToBinary(db.getTable(table_name));
	}
	catch (exception e) {
		cout << e.what() << endl;
	}

}
void DeleteFrom::print()
{
	if (Statement::verbose)
	{
		cout << "Statement::Delete from table: " << table_name << endl;
		cout << "col: " << col_name << "\nvalue: " << value << endl;
	}
}

/********************************************************************************************************* UPDATE CLASS **/
Update::Update() {}
Update::Update(string table_name, string set_col_name, string set_col_value, string where_col_name, string where_col_value)
{
	this->table_name = table_name;
	this->set_col_name = set_col_name;
	this->set_col_value = set_col_value;
	this->where_col_name = where_col_name;
	this->where_col_value = where_col_value;
}
void Update::run(DataBase& db)
{
	try {
		db.Update(&db.getTable(table_name), where_col_name, where_col_value, set_col_name, set_col_value);
		IOFile::databaseDescriptionToText(db);
		IOFile::tableToBinary(db.getTable(table_name));
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

void Update::print()
{
	if (Statement::verbose)
	{
		cout << "Statement::Update table: " << table_name << endl;
		cout << "Set:   col: " << set_col_name << ", value: " << set_col_value << endl;
		cout << "Where: col: " << where_col_name << ", value: " << where_col_value << endl;
	}
}

/********************************************************************************************************** SELECT CLASS **/
Select::Select() {}
Select::Select(string col_name, string table_name)
{
	if (col_name != "ALL")
	{
		cols.push_back(col_name);
	}
	this->table_name = table_name;
}
Select::Select(string col_name, string table_name, string where_col_name, string where_col_value)
{
	if (col_name != "ALL")
	{
		cols.push_back(col_name);
	}
	this->table_name = table_name;
	this->where_col_name = where_col_name;
	this->where_col_value = where_col_value;
}
Select::Select(vector<string> cols, string table_name)
{
	this->cols = cols;
	this->table_name = table_name;
	this->where_col_name = where_col_name;
	this->where_col_value = where_col_value;
}
Select::Select(vector<string> cols, string table_name, string where_col_name, string where_col_value)
{
	this->cols = cols;
	this->table_name = table_name;
	this->where_col_name = where_col_name;
	this->where_col_value = where_col_value;
}
void Select::run(DataBase& db)
{
	
	try
	{
		Table r;
		if (this->where_col_name == "-") {
			if (!cols.empty()) {
				r = db.Select(cols.data(), cols.size(), db.getTable(table_name));
				cout << r;
			}
			else {
				//cout << "you should see me!" << endl;
				Table t = db.getTable(table_name);
				
				vector<string> v;
				for (int i = 0; i < t.getSize(); i++)
					v.push_back(t[i].getName());
				r = db.Select(v.data(), v.size(), t);
				cout << r ;
				//cout << t<<endl;
			}
		}
		else {
			if (!cols.empty()) {
				Table t = db.Where(db.getTable(table_name), where_col_name, where_col_value);
				r = db.Select(cols.data(), cols.size(), t);
				cout << r;
			}
			else {
				r = db.getTable(table_name);
				Table t;
				vector<string> v;
				for (int i = 0; i < r.getSize(); i++)
					v.push_back(r[i].getName());
				t = db.Select(v.data(), v.size(), r);
				r = db.Where(t, where_col_name, where_col_value);
				cout << r;
			}
		}
		r.setName("Select_" + std::to_string(statement::Select::sNumber++));
		IOFile::tableToText(r);


		
	}
	catch (exception e)
	{
		cout << e.what()<<endl;
	}
}

void Select::print()
{
	if (Statement::verbose)
	{
		cout << "Select::table: " << table_name << endl;
		cout << "Selected: ";
		if (cols.empty()) cout << "ALL";
		else
		{
			for (string s : cols)
				cout << s + " ";
		}
		cout << endl;
		cout << "Where: col: " << where_col_name << ", value: " << where_col_value << endl;
	}
}

int statement::Select::sNumber = 0;
int statement::DisplayTable::dNumber = 0;