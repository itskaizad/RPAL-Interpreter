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
	vector<CSElement*> taus;
	string value;
	int index;
	string param1;
	int parEnv;
	bool isEnvMarker;
	bool isTau;
	int type;

	CSElement(string mVal)
	{
		value = mVal;
		index = 0;
		param1 = "";
		parEnv = -1;
		isEnvMarker = false;
		isTau = false;
		type = GRAM_RULE;
	}

	CSElement(string mVal, int mIndex, string mPar1, int parEnv)
	{
		value = mVal;
		index = mIndex;
		param1 = mPar1;
		parEnv = parEnv;
		isEnvMarker = false;
		isTau = false;
		type = GRAM_RULE;
	}


};

#endif	//Close CSELEMENT_H_INCLUDED