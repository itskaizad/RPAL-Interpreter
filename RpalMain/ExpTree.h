#pragma once
#include <iostream>
#include <string>
#include <ctype.h>
#include <cstdio>
#include <stack>
#include "LexAnalyzer.h"
using namespace std;

#ifndef EXPTREE_H_INCLUDED
#define EXPTREE_H_INCLUDED

class ExpTree
{

	enum ExpNodeType
	{
		STRVALUE,
		INTVALUE,
		ID_NAME,
		GRAM_RULE
	};

public:
	int nodeType;
	string nodeValue;
	ExpTree *childNode;
	ExpTree *siblingNode;

public:
	ExpTree(string mVal, int mType, ExpTree *mChild, ExpTree *mSib)
	{
		nodeValue = mVal;
		nodeType = mType;
		childNode = mChild;
		siblingNode = mSib;
	}


	ExpTree(ExpTree *copyTree)
	{
		nodeValue = copyTree->nodeValue;
		nodeType = copyTree->nodeType;
		childNode = copyTree->childNode;
		siblingNode = copyTree->siblingNode;
	}

	static void prettyPrint(ExpTree* mTree, int count = 0)
	{
		if (mTree != NULL)
		{
			for (int i = 0; i < count; i++)
				cout << ".";
			if (mTree->nodeType == GRAM_RULE)
				cout << mTree->nodeValue << endl;
			else if (mTree->nodeType == INTVALUE)
				cout << "<INT:" << (mTree->nodeValue) << ">" << endl;
			else if (mTree->nodeType == STRVALUE)
				cout << "<STR:'" << mTree->nodeValue << "'>" << endl;
			else if (mTree->nodeType == ID_NAME)
				cout << "<ID:" << mTree->nodeValue << ">" << endl;
			prettyPrint(mTree->childNode, count + 1);
			prettyPrint(mTree->siblingNode, count);
		}

	}

	void operator=(ExpTree t) {
		childNode = t.childNode;
		siblingNode = t.siblingNode;
		nodeType = t.nodeType;
		nodeValue = t.nodeValue;
	}


};


#endif // EXPTREE_H_INCLUDED