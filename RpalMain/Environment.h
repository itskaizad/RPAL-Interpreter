#pragma once
#include <iostream>
#include <string>

using namespace std;

#ifndef ENV_H_INCLUDED
#define ENV_H_INCLUDED

class Environment
{

	enum ExpNodeType
	{
		STRVALUE,
		INTVALUE,
		ID_NAME,
		GRAM_RULE,
		TUPLE,
		COMMA_LIST,
		OPERATOR
	};

public:
	Environment *parent;
	Environment *child;
	string variable;
	CSElement *value;

public:

	Environment()
	{ }

	Environment(string var, CSElement *mValue, int mType)
	{
		parent = NULL;
		child = NULL;
		variable = var;
		value = mValue;
	}

};

#endif	// Close ENV_H_INCLUDED