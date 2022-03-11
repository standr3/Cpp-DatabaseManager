#include "Parser.h"
#include <iostream>
#include <string>
#include <utility>

using namespace parser;
using std::string;
using std::vector;
using std::pair;

/********************************************************************************************************** PARSER CLASS **/
std::vector<parser::GrammarRule> parser::Parser::RULES = {
		GrammarRule("INT_COL_DEF", {
			{"OPEN_BRACKET","IDENTIFIER", "COMMA","INT_WORD", "COMMA","INT_LITERAL", "COMMA","INT_LITERAL","CLOSE_BRACKET"}
		}),
		GrammarRule("FLOAT_COL_DEF", {
			{"OPEN_BRACKET","IDENTIFIER", "COMMA","FLOAT_WORD", "COMMA","INT_LITERAL", "COMMA","FLOAT_LITERAL","CLOSE_BRACKET"}
		}),
		GrammarRule("TEXT_COL_DEF", {
			{"OPEN_BRACKET","IDENTIFIER", "COMMA","TEXT_WORD", "COMMA","INT_LITERAL", "COMMA","STR_LITERAL","CLOSE_BRACKET"}
		}),
		GrammarRule("COL_DEF", {
			{"INT_COL_DEF"},
			{"FLOAT_COL_DEF"},
			{"TEXT_COL_DEF"}
		}),
		GrammarRule("COL_DEF_LIST", {
			{"COL_DEF_LIST", "COMMA", "COL_DEF"},
			{"OPEN_BRACKET", "COL_DEF"}
		}),
		GrammarRule("CREATE_TABLE", {
			//{"CREATE_WORD", "TABLE_WORD", "TABLE_WORD", "IDENTIFIER", "OPEN_BRACKET", "COL_DEF_LIST", "CLOSE_BRACKET", "SEMICOLON"},//for testing
			{"CREATE_WORD", "TABLE_WORD", "IDENTIFIER", "COL_DEF_LIST", "CLOSE_BRACKET", "SEMICOLON"}
		}),
		GrammarRule("DROP_TABLE", {
			{"DROP_WORD", "TABLE_WORD", "IDENTIFIER", "SEMICOLON"}
		}),
		GrammarRule("DISPLAY_TABLE", {
			{"DISPLAY_WORD", "TABLE_WORD", "IDENTIFIER", "SEMICOLON"}
		}),
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
		GrammarRule("DELETE_FROM", {
			{"DELETE_WORD", "FROM_WORD", "IDENTIFIER", "WHERE_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "SEMICOLON"},
			{"DELETE_WORD", "FROM_WORD", "IDENTIFIER", "WHERE_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "SEMICOLON"},
			{"DELETE_WORD", "FROM_WORD", "IDENTIFIER", "WHERE_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "SEMICOLON"}
		}),
		GrammarRule("IDENTIFIER_LIST", {
			{"IDENTIFIER_LIST", "COMMA", "IDENTIFIER"},
			{"IDENTIFIER", "COMMA", "IDENTIFIER" }
		}),
		GrammarRule("SELECT", {
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
		}),
		GrammarRule("UPDATE", {
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "INT_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "FLOAT_LITERAL", "SEMICOLON"},
			{"UPDATE_WORD", "IDENTIFIER", "SET_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "WHERE_WORD", "IDENTIFIER", "EQUALS", "STR_LITERAL", "SEMICOLON"},
		}),
		GrammarRule("HELP", {{"HELP_WORD"}} )
};

Parser::Parser() : tokenizer(NULL)
{
	this->tokenizer = new tokenizer::Tokenizer;
}
Parser::Parser(tokenizer::Tokenizer* tokenizer ) : tokenizer(NULL)
{
	this->tokenizer = tokenizer;
}
Node Parser::parse(vector<Token> tokens)
{
	vector<Node> stack;

	/*Tokenizer::Token firsttoken = tokens[0];
	cout << firsttoken.token_type << endl;*/

	for (Token token : tokens)
	{
		stack.push_back(Node(token));
		bool should_continue = true;
		while (should_continue)
		{
			should_continue = false;
			for (GrammarRule gRule : Parser::RULES)
			{
				auto matchResult = gRule.matches(stack);

				if (matchResult.first.size() != 0)
				{
					//cout << "\n---------" << gRule.name << endl;
					should_continue = true;
					Node n(gRule.name);
					n.children = matchResult.first;
					n.subPatternIndx = matchResult.second;
					for (int i = 0; i < matchResult.first.size(); ++i)
					{
						stack.pop_back();
					}
					stack.push_back(n);
				}
			}
		}
	}

	bool ruleFound = false;
	vector<string> allGoodRules = { "HELP", "CREATE_TABLE", "DROP_TABLE","DISPLAY_TABLE","INSERT_INTO","DELETE_FROM","SELECT","UPDATE" };
	if (!stack.empty() && find(allGoodRules.begin(), allGoodRules.end(), stack.back().name) != allGoodRules.end())
		return stack.back();
	else
		return Node(Token(tokenizer::ERROR_TOKEN, TOKEN_TYPE_NAMES[tokenizer::ERROR_TOKEN]));
}

/***************************************************************************************************** GRAMMARRULE CLASS **/

GrammarRule::GrammarRule() 
{}
GrammarRule::GrammarRule(string _name, vector<vector<string>> _patterns)
{
	name = _name;
	patterns = _patterns;
}

pair<vector<Node>, int> GrammarRule::matches(vector<Node> stack)
{

	int count = -1;
	for (vector<string>& subpattern : patterns)
	{
		count++;
		if (stack.size() >= subpattern.size())
		{
			vector<Node> subStack(stack.end() - subpattern.size(), stack.end());
			vector<string> subNames;
			for (Node node : subStack)
			{
				subNames.push_back(node.name);
			}

			if (subpattern == subNames)
			{
				/*for (string s : subNames)
					cout << s << " ";
				cout << endl;*/
				return make_pair(subStack, count);
			}
		}
	}
	return make_pair(vector<Node>(), -1);
}

/************************************************************************************************************ NODE CLASS **/

Node::Node() {}
Node::Node(string _rule_name)
{
	name = _rule_name;
}
Node::Node(Token tok)
{

	name = TOKEN_TYPE_NAMES[tok.token_type];
	value = tok.token_string;
}
string Node::toString(int indent = 0)
{
	string childrenStr;
	for (Node n : children)
	{
		childrenStr += "\n" + n.toString(indent + 4);
	}
	string indentation(indent, ' ');

	return indentation + name + ":" + value + childrenStr;
}
tokenizer::Tokenizer Parser::getTokenizer()
{
	return *(this->tokenizer);
}