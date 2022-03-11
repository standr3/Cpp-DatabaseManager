#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include "IOFile.h"

using namespace std;

bool IOFile::databaseDescriptionToText(DataBase& db)
{
    string filename = db.getName() + ".txt";
    remove(filename.data());    
    ofstream MyFile(filename);

    MyFile << db.getName()<<endl;
    for (int i = 0; i < db.getSize(); i++) {
        MyFile << db[i].getName() << " " << db[i].getSize() << " " << db[i].getNumberOfRecordings() << endl;
    }

    MyFile.close();
    return false;
}

vector<string> IOFile::databaseFilesFromText(string filename)
{
    vector<string> r;
    ifstream file(filename + ".txt");
    string line;
    stringstream streamLine;
    string tableName;

    getline(file, line);

    while (std::getline(file, line))
    {
        streamLine = stringstream(line);
        getline(streamLine, tableName, ' ');
        r.push_back(tableName);
    }

    return r;
}

bool IOFile::tableToText(Table& t)
{
    string filename = t.getName() + ".txt";
    remove(filename.data());
    try 
    {
        ofstream MyFile(t.getName() + ".txt");
        MyFile << t;
        MyFile.close();
    }
    catch (exception e) 
    {
        throw e;
        return false;
    }
    return true;
}

bool IOFile::tableToCSV(Table& table)
{

    string filename = table.getName() + ".csv";
    remove(filename.data());
    try 
    {
        ofstream MyFile(table.getName() + ".csv");
        int dim = table.getNumberOfRecordings();
        vector<pair<string, int>> colNamesAndTypes = table.getColumnNamesAndTypes();
        for (int i = 0; i < table.getSize()-1; i++) 
        {
            MyFile << colNamesAndTypes[i].first << ',';
        }
        MyFile << colNamesAndTypes[table.getSize() - 1].first << endl;
        for (int index = 0; index < dim; index++) 
        {
            for (int i = 0; i < table.getSize() - 1; i++) 
            {
                MyFile <<table[i][index] << ',';
            }
            MyFile << table[table.getSize() - 1][index] << endl;
        }
        MyFile.close();
        return true;
    }
    catch (exception e) 
    {
        throw e;
        return false;
    }
    return true;
}

Table IOFile::tableFromCSV(string filename) 
{
    string line;
    string value;
    vector<pair<string, int>> columns;
    stringstream streamLine;
    ifstream file(filename + ".csv");
    int noRecordings = 0;
    getline(file, line);
    noRecordings = std::count(line.begin(), line.end(), ',') + 1;

    streamLine = stringstream(line);
    while (getline(streamLine, value, ',')) 
    {
        columns.push_back(make_pair(value, 2));
    }
    Table t = Table(filename, columns);
    Recording r;
    while (std::getline(file, line))
    {
        streamLine = stringstream(line);
        int i = 0;
        while (getline(streamLine, value, ',')) 
        {
            r.append(columns[i].first, "2");
            i++;
        }
        t.appendRecording(r);
        r.clear();
    }
    return t;
}

bool IOFile::tableToBinary(Table& t)
{
    string filename = t.getName() + ".tab";
    vector<pair<string, int>> cols = t.getColumnNamesAndTypes();
    remove(filename.data());

    ofstream wf(t.getName()+".tab", ios::out | ios::binary);

    int dim = t.getName().size();
    wf.write((char*)&dim, sizeof(int));
    wf.write( t.getName().data() , sizeof(char) * t.getName().size());
    dim = t.getSize();
    wf.write((char*)&dim, sizeof(int));

    int noChr, noRecordings = t.getNumberOfRecordings();
    for (int i = 0; i < dim; i++) 
    {
        noChr = cols[i].first.size();
        wf.write((char*)&noChr, sizeof(int));
        wf.write(cols[i].first.data(), sizeof(char) * noChr);
        noChr = cols[i].second;
        wf.write( (char*) &noChr, sizeof(int));
    }
    wf.write((char*)&noRecordings, sizeof(int)); 
    for (int i = 0; i < t[0].getSize(); i++) 
    {
        for (int index = 0; index < dim; index++) 
        {
            noChr = t[index][i].size();
            wf.write((char*)&noChr, sizeof(int));
            wf.write(t[index][i].data(), sizeof(char) * noChr);
        }
    }
    wf.close();
    if (!wf.good()) 
    {
        throw exception("Eroare in timpul scrierii in fisierul binar");
        return false;
    }
    return true;
}

Table IOFile::tableFromBinary(string filename)
{
    Table t;
    vector<pair<string, int>> columns;
    string file = filename + ".tab";
    ifstream myFile(file, ios::in | ios::binary);
    cout << "filename " << file << endl;
    int dim = 0; //number of characters in table name
    myFile.read((char*)&dim, sizeof(int));
    cout << "//number of characters in table name " << dim << endl<<"sizeof char "<<sizeof(char)<<endl;

    char* tableName = new char[dim];
    tableName[dim] = '\0';
    myFile.read(tableName, dim);  
    cout << " table name " << tableName << endl;
    string tName = tableName;
    tName[dim] = '\0';
    cout << "table name string " << tName.data() << endl;

    t.setName(tableName);
    myFile.read((char*)&dim, sizeof(int)); 
    cout << "Number of columns " << dim << endl;
    int noChr, noRecordings;
    char* columnName;
    int dimColumnName = 0;
    int columnType = 0;
    for (int i = 0; i < dim; i++) {
        myFile.read((char*)&dimColumnName, sizeof(int));
        cout << "number of characters in column name " << dimColumnName << endl;
        columnName = new char[dimColumnName];
        columnName[dimColumnName] = '\0';
        myFile.read(columnName, sizeof(char) * dimColumnName);
        cout << "column name " << columnName << endl;
        myFile.read((char*)&columnType, sizeof(int));
        cout << "column type " << columnType << endl;
        columns.push_back(make_pair(string(columnName), columnType));
    }
    t = Table(tableName, columns);
    columnName = new char[3];
    myFile.read((char*)&noRecordings, sizeof(int)); 
    cout << "no of recordings: " << noRecordings << endl;
    Recording r;
    for (int i = 0; i < noRecordings; i++) {
        for (int index = 0; index < dim; index++) {
            myFile.read((char*)&dimColumnName, sizeof(int));
            cout << "no char in value " << dimColumnName << endl;
            columnName = new char[dimColumnName];
            columnName[dimColumnName] = '\0';
            myFile.read(columnName, sizeof(char) * dimColumnName);
            cout << "val " << columnName << endl;

            if (i == 0)
                r.append(columns[index].first, string(columnName));
            else
                r.modify(index, string(columnName));
        }
        t.appendRecording(r);
    }
    cout << "inside" << endl << t << "end" << endl;
    return t;
}
