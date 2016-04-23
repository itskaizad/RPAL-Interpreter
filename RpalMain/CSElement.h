#pragma once

using namespace std;

#ifndef CSELEMENT_H_INCLUDED
#define CSELEMENT_H_INCLUDED

class CSElement
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
	string value;
	int index;
	string param1;
	int parEnv;
	bool isEnvMarker;
	int type;

	CSElement(string mVal)
	{
		value = mVal;
		index = 0;
		param1 = "";
		parEnv = -1;
		isEnvMarker = false;
		type = GRAM_RULE;
	}

	CSElement(string mVal, int mIndex, string mPar1, int mPar2)
	{
		value = mVal;
		index = mIndex;
		param1 = mPar1;
		parEnv = mPar2;
		isEnvMarker = false;
		type = GRAM_RULE;
	}


};

#endif	//Close CSELEMENT_H_INCLUDED