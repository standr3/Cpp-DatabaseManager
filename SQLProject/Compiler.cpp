#include "Compiler.h"
//#include "Statement.h"
//#include "Parser.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace statement;
using parser::Node;
using std::string;
using std::pair;
using std::vector;


Compiler::Compiler() : parser(NULL)
{
	this->parser = new parser::Parser;
}
Compiler::Compiler(parser::Parser *parser) : parser(NULL)
{
	this->parser = parser;
}
Statement* Compiler::compile(Node AST)
{
	Statement* res = NULL;
	if (AST.name == "HELP")
	{
		if (AST.subPatternIndx == 0)
		{
			res = new Help();
		}
	}
	else if (AST.name == "CREATE_TABLE")
	{
		if (AST.subPatternIndx == 0)
		{
			string table_name = AST.children[2].value;
			vector<ColumnDef> col_defs;
			Node col_def_list = AST.children[3];

			while (col_def_list.subPatternIndx == 0)
			{
				Node next = col_def_list.children[0];
				Node col_def = col_def_list.children[2].children[0];
				ColumnDef cd;
				cd.name = col_def.children[1].value;
				cd.type = col_def.children[3].value;
				cd.dim = col_def.children[5].value;
				cd.default_value = col_def.children[7].value;
				col_defs.push_back(cd);
				col_def_list = next;
			}
			Node col_def = col_def_list.children[1].children[0];
			ColumnDef cd;
			cd.name = col_def.children[1].value;
			cd.type = col_def.children[3].value;
			cd.dim = col_def.children[5].value;
			cd.default_value = col_def.children[7].value;
			col_defs.push_back(cd);

			std::reverse(col_defs.begin(), col_defs.end());

			res = new CreateTable(table_name, col_defs);
		}
	}
	else if (AST.name == "DROP_TABLE")
	{
		string table_name = AST.children[2].value;
		res = new DropTable(table_name);
	}
	else if (AST.name == "DISPLAY_TABLE")
	{
		string table_name = AST.children[2].value;
		res = new DisplayTable(table_name);
	}
	else if (AST.name == "INSERT_INTO")
	{
		/*
		GrammarRule("VALUES_LIST", {
			{"VALUES_LIST", "COMMA", "INT_LITERAL"},
			{"VALUES_LIST", "COMMA", "FLOAT_LITERAL"},
			{"VALUES_LIST", "COMMA", "STR_LITERAL"},
			{"OPEN_BRACKET", "INT_LITERAL"},
			{"OPEN_BRACKET", "FLOAT_LITERAL"},
			{"OPEN_BRACKET", "STR_LITERAL"},
		}),
		GrammarRule("INSERT_INTO", {
			{"INSERT_WORD", "INTO_WORD", "IDENTIFIER", "VALUES_LIST", "CLOSE_BRACKET", "SEMICOLON"}
		}),
		*/
		string table_name = AST.children[2].value;
		vector<pair<string, string>> values;
		Node values_list = AST.children[3];
		while (values_list.subPatternIndx == 0 || values_list.subPatternIndx == 1 || values_list.subPatternIndx == 2)
		{
			Node next = values_list.children[0];
			Node literalNode = values_list.children[2];
			string value, type;

			if (literalNode.name == "INT_LITERAL")
			{
				// literal is of type INT
				value = literalNode.value;
				type = "INT";
			}
			else if (literalNode.name == "FLOAT_LITERAL")
			{
				// literal is of type FLOAT
				value = literalNode.value;
				type = "FLOAT";
			}
			else if (literalNode.name == "STR_LITERAL")
			{
				// literal is of type TEXT
				value = literalNode.value;
				type = "TEXT";
			}
			values.push_back(make_pair(value, type));
			values_list = next;
		}
		Node valueNode = values_list.children[1];
		string value, type;
		if (valueNode.name == "INT_LITERAL")
		{
			// literal is of type INT
			value = valueNode.value;
			type = "INT";
		}
		else if (valueNode.name == "FLOAT_LITERAL")
		{
			// literal is of type FLOAT
			value = valueNode.value;
			type = "FLOAT";
		}
		else if (valueNode.name == "STR_LITERAL")
		{
			// literal is of type TEXT
			value = valueNode.value;
			type = "TEXT";
		}
		values.push_back(make_pair(value, type));
		reverse(values.begin(), values.end());
		res = new InsertInto(table_name, values);
	}
	else if (AST.name == "DELETE_FROM")
	{
		/*GrammarRule("DELETE_FROM", {
			{"DELETE_WORD", "FROM_WORD", "IDENTIFIER", "WHERE_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "SEMICOLON"},
			{"DELETE_WORD", "FROM_WORD", "IDENTIFIER", "WHERE_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "SEMICOLON"},
			{"DELETE_WORD", "FROM_WORD", "IDENTIFIER", "WHERE_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "SEMICOLON"}
		})*/
		string table_name = AST.children[2].value;
		string col_name = AST.children[4].value;
		string value = AST.children[6].value;
		res = new DeleteFrom(table_name, col_name, value);

	}
	else if (AST.name == "SELECT")
	{
		/*GrammarRule("SELECT", {
			{"SELECT_WORD", "ALL_WORD", "FROM_WORD", "IDENTIFIER", "SEMICOLON"},
			{"SELECT_WORD", "ALL_WORD", "FROM_WORD", "IDENTIFIER", "WHERE_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "SEMICOLON"},
			{"SELECT_WORD", "ALL_WORD", "FROM_WORD", "IDENTIFIER", "WHERE_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "SEMICOLON"},
			{"SELECT_WORD", "ALL_WORD", "FROM_WORD", "IDENTIFIER", "WHERE_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "SEMICOLON"},
			{"SELECT_WORD", "OPEN_BRACKET", "IDENTIFIER_LIST", "CLOSE_BRACKET", "FROM_WORD", "IDENTIFIER", "WHERE_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "SEMICOLON"},
			{"SELECT_WORD", "OPEN_BRACKET", "IDENTIFIER_LIST", "CLOSE_BRACKET", "FROM_WORD", "IDENTIFIER", "WHERE_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "SEMICOLON"},
			{"SELECT_WORD", "OPEN_BRACKET", "IDENTIFIER_LIST", "CLOSE_BRACKET", "FROM_WORD", "IDENTIFIER", "WHERE_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "SEMICOLON"},
			{"SELECT_WORD", "OPEN_BRACKET", "IDENTIFIER_LIST", "CLOSE_BRACKET", "FROM_WORD", "IDENTIFIER", "SEMICOLON"},
			{"SELECT_WORD", "OPEN_BRACKET", "IDENTIFIER", "CLOSE_BRACKET", "FROM_WORD", "IDENTIFIER", "WHERE_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "SEMICOLON"},
			{"SELECT_WORD", "OPEN_BRACKET", "IDENTIFIER", "CLOSE_BRACKET", "FROM_WORD", "IDENTIFIER", "WHERE_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "SEMICOLON"},
			{"SELECT_WORD", "OPEN_BRACKET", "IDENTIFIER", "CLOSE_BRACKET", "FROM_WORD", "IDENTIFIER", "WHERE_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "SEMICOLON"},
			{"SELECT_WORD", "OPEN_BRACKET", "IDENTIFIER", "CLOSE_BRACKET", "FROM_WORD", "IDENTIFIER", "SEMICOLON"}
		})*/

		if (AST.children[1].name == "ALL_WORD")
		{

			string table_name = AST.children[3].value;
			if (AST.children[4].name == "WHERE_WORD")
			{
				string where_col_name = AST.children[5].value;
				string where_col_value = AST.children[7].value;
				res = new  Select("ALL", table_name, where_col_name, where_col_value);
			}
			else
			{
				res = new  Select("ALL", table_name);
			}
		}
		else if (AST.children[2].name == "IDENTIFIER_LIST")
		{
			string table_name = AST.children[5].value;
			if (AST.children[6].name == "WHERE_WORD")
			{
				string where_col_name = AST.children[7].value;
				string where_col_value = AST.children[9].value;

				Node col_list = AST.children[2];
				vector<string> cols;
				while (col_list.subPatternIndx == 0)
				{
					cols.push_back(col_list.children[2].value);
					col_list = col_list.children[0];
				}
				cols.push_back(col_list.children[0].value);
				cols.push_back(col_list.children[2].value);
				res = new  Select(cols, table_name, where_col_name, where_col_value);
			}
			else
			{
				Node col_list = AST.children[2];
				vector<string> cols;
				while (col_list.subPatternIndx == 0)
				{
					cols.push_back(col_list.children[2].value);
					col_list = col_list.children[0];
				}
				cols.push_back(col_list.children[0].value);
				cols.push_back(col_list.children[2].value);
				res = new  Select(cols, table_name);
			}
		}
		else
		{
			string col_name = AST.children[2].value;
			string table_name = AST.children[5].value;
			if (AST.children[6].name == "WHERE_WORD")
			{
				string where_col_name = AST.children[7].value;
				string where_col_value = AST.children[9].value;
				res = new Select(col_name, table_name, where_col_name, where_col_value);
			}
			else
			{
				res = new  Select(col_name, table_name);
			}
		}
	}
	else if (AST.name == "UPDATE")
	{
		/*GrammarRule("UPDATE", {
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "SEMICOLON"},
		})*/

		string table_name = AST.children[1].value;
		string set_col_name = AST.children[3].value;
		string set_col_val = AST.children[5].value;
		string where_col_name = AST.children[7].value;
		string where_col_val = AST.children[9].value;
		res = new Update(table_name, set_col_name, set_col_val, where_col_name, where_col_val);
	}
	return res;
}
parser::Parser Compiler::getParser()
{
	return *(this->parser);
}