#pragma once
#include "Tokenizer.h"
#include <vector>

namespace parser
{
	using namespace std;
	using tokenizer::Token;
	/* ------------------------------------------------------------------------------------------------------------ Node --- */
	class Node
	{
	public:
		string name; // uppercase identifier
		vector<Node> children;
		string value; // actual code part
		int subPatternIndx;

		Node();
		Node(string _rule_name);
		Node(Token tok);

		string toString(int indent);
	};
	class GrammarRule
	{
	public:
		string name;
		vector<vector<string>> patterns;

		GrammarRule();
		GrammarRule(string, vector<vector<string>>);

		pair<vector<Node>, int> matches(vector<Node>);
	};
	class Parser
	{
		tokenizer::Tokenizer *tokenizer;
	public:
		static vector<GrammarRule> RULES;
		Parser();
		Parser(tokenizer::Tokenizer*);
		Node parse(vector<Token>);
		tokenizer::Tokenizer getTokenizer();
	};
	
}
