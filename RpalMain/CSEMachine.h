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
	stack<CSElement> leftStack;
	stack<CSElement> rightStack;

	bool printControlCreation, printControls;

public:
	CSEMachine(ExpTree* mTree)
	{
		stdTree = mTree;

		printControlCreation = false;				//Print switch
		buildControlStructures(mTree, 1);

		printControls = false;						//Print switch
		if(printControls)
			printControlStructures();
		initializeCSEMachine();
		printCSE();
		printCSE();
	}

	void buildControlStructures(ExpTree* mTree, int index)
	{
		if (mTree == NULL)
			return;

		//Helper to debug how deltas are made for the control structures.
		if(printControlCreation)
			cout << index << ".\t" << mTree->nodeValue << " is a " << mTree->nodeType << endl;

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
		cout << "Size: " << controls.size() << endl;

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

	void pushDeltaOnLeftStack(int delta)
	{
		// Create an environment marker for that delta
		CSElement *freshEnv = new CSElement("env", delta, "", "");
		freshEnv->isEnvMarker = true;

		//Push the environment marker on both stacks first
		leftStack.push(*freshEnv);
		rightStack.push(*freshEnv);

		//Then push the entire delta onto the left stack.
		for (int i = 0; i < controls[delta].size(); i++)
		{
			leftStack.push( controls[delta].at(i) );
		}
	}

	void initializeCSEMachine()
	{
		//Push delta 0 on the stacks
		pushDeltaOnLeftStack(0);

		//Set current environment to zero
		envCounter = 0;
	}

	void printCSE()
	{
		//Print left stack
		cout << "LEFT:\t";
		stack<CSElement> tempLeft;
		while (!leftStack.empty())
		{
			tempLeft.push(leftStack.top());
			leftStack.pop();
		}
		while (!tempLeft.empty())
		{
			leftStack.push(tempLeft.top());
			if(leftStack.top().value == "lambda")
				cout << leftStack.top().value << leftStack.top().index << " ";
			else if(leftStack.top().value == "env")
				cout << leftStack.top().value << leftStack.top().index << " ";
			else
				cout << leftStack.top().value << " ";
			tempLeft.pop();
		}


		//Print right stack
		cout << endl << "RIGHT:\t";
		stack<CSElement> tempRight;
		while (!rightStack.empty())
		{
			tempRight.push(rightStack.top());
			if (rightStack.top().value == "lambda")
				cout << rightStack.top().value << rightStack.top().index << " ";
			else if (rightStack.top().value == "env")
				cout << rightStack.top().value << rightStack.top().index << " ";
			else
				cout << rightStack.top().value << " ";
			rightStack.pop();
		}
		while (!tempRight.empty())
		{
			rightStack.push(tempRight.top());
			tempRight.pop();
		}
		cout << endl << endl;
	}

	void runCSEMachine()		//Assumes there is atleast one delta/environment on the machine
	{

	}

	
};

#endif	//Close CSEMACHINE_H_INCLUDED