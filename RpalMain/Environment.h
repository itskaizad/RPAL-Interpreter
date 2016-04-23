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
	string value;
	int type;

public:

	Environment()
	{ }

	Environment(string var, string mValue, int mType)
	{
		parent = NULL;
		child = NULL;
		variable = var;
		mValue = value;
		type = mType;
	}

};

#endif	// Close ENV_H_INCLUDED