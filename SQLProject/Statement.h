#pragma once
#include "DataBase.h"

namespace statement
{

	

	using namespace std;

	class ColumnDef
	{
	public:
		string name;
		string type;
		string dim;
		string default_value;
		ColumnDef() {}
	};
	class Statement
	{
	
	public:
		static bool verbose;
		Statement() { }
		virtual void run(DataBase& db) = 0;
		virtual void print() = 0;
	};
	class Help : public Statement
	{
	public:
		
		Help();
		
		void run(DataBase& db);
		void print();
	};

	class CreateTable : public Statement
	{
	public:
		string table_name;
		vector<ColumnDef> col_defs;
		CreateTable();
		CreateTable(string _tname, vector<ColumnDef> _cdef);
		
		void run(DataBase& db);
		void print();
	};
	class DropTable : public Statement
	{
	public:
		string table_name;
		DropTable();
		DropTable(string _tName);
		void run(DataBase& db);
		void print();
	};
	class DisplayTable : public Statement
	{
	public:
		static int dNumber;
		string table_name;
		DisplayTable();
		DisplayTable(string _tName);
		void run(DataBase& db);
		void print();
	};
	class InsertInto : public Statement
	{
	public:
		string table_name;
		vector<pair<string, string>> values; // pair <type,string>  =  value
		InsertInto();
		InsertInto(string _tName, vector<pair<string, string>> _values);
		void run(DataBase& db);
		void print();
	};
	class DeleteFrom : public Statement
	{
	public:
		string table_name;
		string col_name;
		string value;
		DeleteFrom();
		DeleteFrom(string table_name, string col_name, string value);
		void run(DataBase& db);
		void print();
	};
	class Update : public Statement
	{
	public:
		string table_name;
		string set_col_name;
		string set_col_value;
		string where_col_name;
		string where_col_value;
		Update();
		Update(string table_name, string set_col_name, string set_col_value, string where_col_name, string where_col_value);
		void run(DataBase& db);

		void print();

	};
	class Select : public Statement
	{
	public:
		static int sNumber;
		string table_name;
		string where_col_name = "-";
		string where_col_value = "-";
		vector<string> cols; // if empty -> ALL option

		Select();
		Select(string col_name, string table_name);
		Select(string col_name, string table_name, string where_col_name, string where_col_value);
		Select(vector<string> cols, string table_name);
		Select(vector<string> cols, string table_name, string where_col_name, string where_col_value);
		void run(DataBase& db);

		void print();

	};
	
}

