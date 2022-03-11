#pragma once
#include "DataBase.h"
class IOFile
{
public:
	static bool databaseDescriptionToText(DataBase& db);
	static bool tableToText(Table& t);
	static bool tableToCSV(Table& t);
	static Table tableFromCSV(string filename);
	static bool tableToBinary(Table& t);
	static Table tableFromBinary(string filename);
	static vector<string> databaseFilesFromText(string filename);

};

