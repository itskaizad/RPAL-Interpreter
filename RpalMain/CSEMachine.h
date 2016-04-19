#pragma once
#include <iostream>
#include <string>
#include <ctype.h>
#include <cstdio>
#include <stack>
#include <vector>
#include "ExpTree.h"

using namespace std;


#ifndef CSEMACHINE_H_INCLUDED
#define CSEMACHINE_H_INCLUDED

class CSEMachine
{
private:
	ExpTree* stdTree;
	int envCounter;


public:
	CSEMachine(ExpTree* mTree)
	{
		stdTree = mTree;
	}



};

#endif	//Close CSEMACHINE_H_INCLUDED