#pragma once
#include <regex>
#include <list>
#include <iostream>

namespace tokenizer 
{
	using namespace std;

	enum TokenType {
		ERROR_TOKEN,
		HELP_WORD,
		CREATE_WORD,
		TABLE_WORD,
		DROP_WORD,
		DISPLAY_WORD,
		INSERT_WORD,
		INTO_WORD,
		DELETE_WORD,
		FROM_WORD,
		WHERE_WORD,
		SELECT_WORD,
		ALL_WORD,
		SET_WORD,
		UPDATE_WORD,
		OPEN_BRACKET,
		CLOSE_BRACKET,
		EQUALS,
		COMMA,
		SEMICOLON,
		IDENTIFIER,
		INT_WORD,
		FLOAT_WORD,
		TEXT_WORD,
		INT_LITERAL,
		STR_LITERAL,
		FLOAT_LITERAL
	};


	/* -------------------------------------------------------------------------------------------------------- Token --- */
	class Token
	{
	public:
		TokenType token_type;
		string token_string;
		Token();
		Token(TokenType, string);
	};
	/* ----------------------------------------------------------------------------------------------------- Tokenizer --- */
	class Tokenizer
	{
	private:
		/* ---------------------------------------------------------------------------------- Tokenizer :: LexicalRule --- */
		class LexicalRule
		{

		public:
			TokenType token_type;
			regex regex_string;
			LexicalRule();
			LexicalRule(TokenType, regex);
		};
	public:
		static vector<LexicalRule> RULES;
		void replaceAll(string&, const string&, const string&);
		void prep_cmd(string&);
		list<string> split(string);
		TokenType classify_token(string);
		vector<Token> tokenize(string&);

	};


}

extern const std::vector<std::string> TOKEN_TYPE_NAMES;
extern std::ostream& operator<<(std::ostream& out, tokenizer::TokenType& tt);