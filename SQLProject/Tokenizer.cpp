#define CASE_INSENSITIVE std::regex_constants::icase
#include "Tokenizer.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <list>

using namespace tokenizer;
using std::vector;
using std::list;
using std::string;
using std::regex;
using std::stringstream;

const vector<string> TOKEN_TYPE_NAMES = {
	"ERROR_TOKEN",
	"HELP_WORD",
	"CREATE_WORD",
	"TABLE_WORD",
	"DROP_WORD",
	"DISPLAY_WORD",
	"INSERT_WORD",
	"INTO_WORD",
	"DELETE_WORD",
	"FROM_WORD",
	"WHERE_WORD",
	"SELECT_WORD",
	"ALL_WORD",
	"SET_WORD",
	"UPDATE_WORD",
	"OPEN_BRACKET",
	"CLOSE_BRACKET",
	"EQUALS",
	"COMMA",
	"SEMICOLON",
	"IDENTIFIER",
	"INT_WORD",
	"FLOAT_WORD",
	"TEXT_WORD",
	"INT_LITERAL",
	"STR_LITERAL",
	"FLOAT_LITERAL"
};
vector<Tokenizer::LexicalRule> Tokenizer::RULES = {
		LexicalRule(HELP_WORD, regex("HELP", CASE_INSENSITIVE)),
		LexicalRule(CREATE_WORD, regex("CREATE", CASE_INSENSITIVE)),
		LexicalRule(TABLE_WORD, regex("TABLE", CASE_INSENSITIVE)),
		LexicalRule(DROP_WORD, regex("DROP", CASE_INSENSITIVE)),
		LexicalRule(DISPLAY_WORD, regex("DISPLAY", CASE_INSENSITIVE)),
		LexicalRule(INSERT_WORD, regex("INSERT", CASE_INSENSITIVE)),
		LexicalRule(INTO_WORD, regex("INTO", CASE_INSENSITIVE)),
		LexicalRule(DELETE_WORD, regex("DELETE", CASE_INSENSITIVE)),
		LexicalRule(FROM_WORD, regex("FROM", CASE_INSENSITIVE)),
		LexicalRule(WHERE_WORD, regex("WHERE", CASE_INSENSITIVE)),
		LexicalRule(SELECT_WORD, regex("SELECT", CASE_INSENSITIVE)),
		LexicalRule(SET_WORD, regex("SET", CASE_INSENSITIVE)),
		LexicalRule(ALL_WORD, regex("ALL", CASE_INSENSITIVE)),
		LexicalRule(UPDATE_WORD, regex("UPDATE", CASE_INSENSITIVE)),
		LexicalRule(OPEN_BRACKET, regex("\\(")),
		LexicalRule(CLOSE_BRACKET, regex("\\)")),
		LexicalRule(EQUALS, regex("\\=")),
		LexicalRule(COMMA, regex("\\,")),
		LexicalRule(SEMICOLON, regex("\\;")),
		LexicalRule(INT_WORD, regex("INT", CASE_INSENSITIVE)),
		LexicalRule(FLOAT_WORD, regex("FLOAT", CASE_INSENSITIVE)),
		LexicalRule(TEXT_WORD, regex("TEXT", CASE_INSENSITIVE)),
		LexicalRule(INT_LITERAL, regex("\\d+")),
		LexicalRule(FLOAT_LITERAL, regex("\\d+\\.\\d+")),
		LexicalRule(STR_LITERAL, regex("\'(\\w+)\'")),
		LexicalRule(IDENTIFIER, regex("[a-zA-Z_$][a-zA-Z_$0-9]*"))
};

ostream& operator<<(ostream& out, tokenizer::TokenType& tt)
{
	out << TOKEN_TYPE_NAMES[(int)tt];
	return out;
}

/*********************************************************************************************************** TOKEN CLASS **/
Token::Token()
{}
Token::Token(TokenType _tType, string _tStr)
	: token_type(_tType), token_string(_tStr) 
{}

/******************************************************************************************************* TOKENIZER CLASS **/
void Tokenizer::replaceAll(string& str, const string& from, const string& to)
{
	if (from.empty())
		return;
	int start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}
void Tokenizer::prep_cmd(string& cmd)
{
	replaceAll(cmd, "(", " ( ");
	replaceAll(cmd, ")", " ) ");
	replaceAll(cmd, ",", " , ");
	replaceAll(cmd, ";", " ; ");
}
list<string> Tokenizer::split(string s)
{
	list<string> token_strings;
	stringstream ss(s);
	string word;
	while (ss >> word) {
		token_strings.push_back(word);
	}
	return token_strings;
}
TokenType Tokenizer::classify_token(string token)
{
	for (const auto& rule : Tokenizer::RULES)
	{
		if (regex_match(token, rule.regex_string))
			return rule.token_type;
	}
	return ERROR_TOKEN;
}
vector<Token> Tokenizer::tokenize(string& cmd)
{
	vector<Token> result;
	/* STRING PREPING */
	prep_cmd(cmd);
	/* TOKENIZATION */
	list<string> token_strings = split(cmd);
	/* LEXING */
	for (string token_string : token_strings)
	{
		TokenType token_type = classify_token(token_string);
		Token token = Token(token_type, token_string);
		result.push_back(token);
	}
	return result;
}

/***************************************************************************************************** LEXICALRULE CLASS **/
Tokenizer::LexicalRule::LexicalRule()
	: token_type(ERROR_TOKEN)
{}
Tokenizer::LexicalRule::LexicalRule(TokenType _tType, regex _rStr)
	: token_type(_tType), regex_string(_rStr)
{}