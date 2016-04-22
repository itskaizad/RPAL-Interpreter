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

		//Helper to debug how deltas are made for the control structures.
		//cout << index << ".\t" << mTree->nodeValue << " is a " << mTree->nodeType << endl;

		// Make sure controls[index] exists.
		// If not, youre entering that delta first time, so create it!
		if (controls.size() < index)
		{
			//TODO Check this out! Hopefully * gives the new object from the pointer.
			controls.push_back(*(new vector<CSElement>));
			//PSST: It does!
		}

		CSElement* currentEl;
		if (mTree->nodeValue == "lambda")
		{
			string varName = mTree->childNode->nodeValue;
			int deltaNumber = controls.size();
			currentEl = new CSElement("lambda", deltaNumber, varName, "");
			controls[index - 1].push_back(*currentEl);
			//if (mTree->childNode != NULL)		Commented him out assuming theres always a right child to LAMBDA.
			buildControlStructures(mTree->childNode->siblingNode, deltaNumber + 1);
		}
		else
		{
			currentEl = new CSElement(mTree->nodeValue);
			controls[index - 1].push_back(*currentEl);
			buildControlStructures(mTree->childNode, index);
			if(mTree->childNode != NULL)
				buildControlStructures(mTree->childNode->siblingNode, index);
		}

		
	}


	// To print out every control structure (delta) we have in Vector 'controls'
	void printControlStructures()
	{
		for (int i = 0; i < controls.size(); i++)
		{
			cout << i << ".\t";
			for (int j = 0; j < controls[i].size(); j++)
			{
				if (controls[i].at(j).value == "lambda")
					cout << controls[i].at(j).value << controls[i].at(j).index << " ";
				else
					cout << controls[i].at(j).value << " ";
			}
			cout << endl;
		}
	}

};

#endif	//Close CSEMACHINE_H_INCLUDED