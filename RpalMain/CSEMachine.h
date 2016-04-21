#pragma once
#include <iostream>
#include <string>
#include <ctype.h>
#include <cstdio>
#include <stack>
#include <vector>
#include "ExpTree.h"
#include "CSElement.h"


using namespace std;


#ifndef CSEMACHINE_H_INCLUDED
#define CSEMACHINE_H_INCLUDED

class CSEMachine
{
private:
	ExpTree* stdTree;
	int envCounter;
	vector<vector<CSElement>> controls;

public:
	CSEMachine(ExpTree* mTree)
	{
		stdTree = mTree;
		buildControlStructures(mTree, 1);
		cout << "Size: " << controls.size() << endl;
		printControlStructures();
	}

	void buildControlStructures(ExpTree* mTree, int index)
	{
		if (mTree == NULL)
			return;
		// Make sure controls[index] exists.
		// If not, youre entering that delta first time, so create it!
		if (controls.size() < index)
		{
			//TODO Check this out! Hopefully * gives the new object from the pointer.
			controls.push_back(*(new vector<CSElement>));
		}

		CSElement* currentEl;
		if (mTree->nodeValue == "lambda")
		{
			string varName = mTree->childNode->nodeValue;
			currentEl = new CSElement("delta", index + 1, varName, "");
			buildControlStructures(mTree->childNode->siblingNode, index + 1);
		}
		else
		{
			currentEl = new CSElement(mTree->nodeValue);
			buildControlStructures(mTree->childNode, index);
			buildControlStructures(mTree->siblingNode, index);
		}

		controls[index-1].push_back(*currentEl);
		
	}

	void printControlStructures()
	{
		for (int i = 0; i < controls.size(); i++)
		{
			for (int j = 0; j < controls[i].size(); j++)
			{
				cout << controls[i].at(j).value << " ";
			}
			cout << endl;
		}
	}

};

#endif	//Close CSEMACHINE_H_INCLUDED