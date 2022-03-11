#include <iostream>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include "Compiler.h"
#include "DataBase.h"
#include "IOFile.h"

using namespace std;


class Menu
{
private:

	vector<string> IF_lines;
	void displayOptions(bool loaded = false, bool debugmode = false)
	{
		cout
			<< "1. Input line commands\n"
			<< "2. Load database from file: " << (loaded==true ? "LOADED\n" : "NOT LOADED\n")
			<< "3. Debug mode: " << (debugmode == true ? "ON\n" : "OFF\n")
			<< "...or type \"0\" to quit. \n";
	}
public:
	Menu() {}
	Menu(int argc, char* argv[])
	{
		this->getInputFiles(argc, argv);
	}
	void getInputFiles(int argc, char* argv[])
	{
		for (int i = 0; i < min(5, argc - 1); ++i)
		{
			ifstream fin(argv[1 + i], ios::in | ios::binary);
			string line;
			while (getline(fin, line))
			{
				if (line.find_first_not_of(" \t\n\v\f\r") != std::string::npos)
				{
					this->IF_lines.push_back(line);
				}
			}
			fin.close();
		}
	}

	void runScripts(DataBase& db)
	{
		Compiler c;

		for (string s : IF_lines)
		{
			cout << s << endl;
			vector<tokenizer::Token> tokens = c.getParser().getTokenizer().tokenize(s);
			/*for (tokenizer::Token t : tokens)
			{
				cout << t.token_type << " ";
			}*/
			cout << endl;
			parser::Node n = c.getParser().parse(tokens);
			cout << "NUME COMANDA :: " << n.name << endl;
			//cout << endl << n.toString(0) << endl;
			statement::Statement* st = c.compile(n);

			//st->print();
			st->run(db);

			cout << "Press enter for the next command.\n";
			getchar();
		}
	}
	void runLoop(DataBase& db, bool debug_mode = false)
	{
		Compiler c;
		string s;

		// CLEARS ISTREAM
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		//
		system("CLS");
		while (1)
		{
			std::getline(cin, s);
			if (s == "quit") break;
			vector<tokenizer::Token> tokens = c.getParser().getTokenizer().tokenize(s);
			if (debug_mode)
			{
				for (tokenizer::Token t : tokens)
				{
					cout << t.token_type << " ";
				}
				cout << "\n\n";
			}
			parser::Node AST = c.getParser().parse(tokens);
			if (debug_mode)
			{
				cout << endl << AST.toString(0) << endl;
				cout << "\n\n";
			}
			if (AST.name == "ERROR_TOKEN")
			{
				cout << "Unrecognized statement!\n";
			}
			else 
			{
				statement::Statement* st = c.compile(AST);
				st->print();
				st->run(db);
			}
		}
	}
	void runMenu(DataBase& db, bool runscripts = false)
	{
		bool load_state = false, debug_state = false;
		if (runscripts)
			runScripts(db);
		while (1)
		{
			system("CLS");
			displayOptions(load_state, debug_state);
			char selection;

			while (!_kbhit())
			{			}
			
			selection = _getch();
			if (selection == '1')
			{
				system("CLS");
				cout << "Type \"help\" for a list of commands.\nType \"quit\" to return to menu.\nPress ENTER to continue... \n";
				runLoop(db, debug_state);
			}
			else if (selection == '2')
			{
				load_state = true;
				vector<string> files;
				try 
				{
					files = IOFile::databaseFilesFromText("DataBase");
				}
				catch (exception e) {
					IOFile::databaseDescriptionToText(db);
				}
				if (!files.empty()) {
					for (int i = 0; i < files.size(); i++) {
						try {
							Table j = IOFile::tableFromBinary(files[i]);
							db.append(j);
						}
						catch (exception e) {
							cout << e.what() << endl;
						}
					}
				}

			}
			else if (selection == '3')
			{
				debug_state = !debug_state;
				statement::Statement::verbose = debug_state;
			}
			else if (selection == '0')
			{
				cout << "Quit selected.\n";
				break;
			}
		}
	}

};

int main(int argc, char* argv[])
{
	DataBase db = DataBase("DataBase");
	Menu(argc, argv).runMenu(db);

	return 0;
}
