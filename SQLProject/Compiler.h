#pragma once
#include "Statement.h"
#include "Parser.h"

class Compiler
{
	parser::Parser *parser;
public:
	Compiler();
	Compiler(parser::Parser*);
	statement::Statement* compile(parser::Node AST);
	parser::Parser getParser();
};



