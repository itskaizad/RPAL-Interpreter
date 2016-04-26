#pragma once
#include <iostream>
#include <string>
#include <vector>

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
	vector<CSElement*> taus; 
	string value;
	int index;
	string boundVar;
	int parEnv;
	bool isEnvMarker;
	bool isTau;
	int type;

	CSElement(string mVal)
	{
		value = mVal;
		index = 0;
		boundVar = "";
		parEnv = -1;
		isEnvMarker = false;
		isTau = false;
		type = GRAM_RULE;
	}

	CSElement(string mVal, int mIndex, string mPar1, int mparEnv)
	{
		value = mVal;
		index = mIndex;
		boundVar = mPar1;
		parEnv = mparEnv;
		isEnvMarker = false;
		isTau = false;
		type = GRAM_RULE;
	}


};

#endif	//Close CSELEMENT_H_INCLUDED