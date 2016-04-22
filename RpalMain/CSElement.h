#pragma once

using namespace std;

#ifndef CSELEMENT_H_INCLUDED
#define CSELEMENT_H_INCLUDED

class CSElement
{

public:
	string value;
	int index;
	string param1;
	string param2;
	bool isEnvMarker;

	CSElement(string mVal)
	{
		value = mVal;
		index = 0;
		param1 = "";
		param2 = "";
		isEnvMarker = false;
	}

	CSElement(string mVal, int mIndex, string mPar1, string mPar2)
	{
		value = mVal;
		index = mIndex;
		param1 = mPar1;
		param2 = mPar2;
		isEnvMarker = false;
	}


};

#endif	//Close CSELEMENT_H_INCLUDED