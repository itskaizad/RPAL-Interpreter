#pragma once
#include <iostream>
#include <string>
#include <ctype.h>
#include <cstdio>
#include <stack>
#include <vector>
#include <cmath> 
#include "ExpTree.h"
#include "CSElement.h"
#include "Environment.h"

using namespace std;

#ifndef CSEMACHINE_H_INCLUDED
#define CSEMACHINE_H_INCLUDED

//TODO
//Fix Environment generation (currently linear)
//Fix attaching multiple variables
//Fix variable lookup function
//
//Write all rules(!!)

//NOTE TO SELF:
//When the (var) attached to lambda is a comma,
//attach a string that is a comma separated list of variable names.
//On the Environment tree, attach a list of CSElements (same size as tau)
//Whenever lookup gets called, first check if the variable name has a comma in it!
//If it does, check the entire list (vector) or just check the first element.
//This should work.
//We eliminated standardizing ,[COMMA] and stored multiple variables at a time.
//We did not standardize tau, and will make ...[DEVELOP]-Glitch found (below)
//We cannot store two CSElements in one. (Dayum!)
//

class CSEMachine
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


private:
	ExpTree* stdTree;
	int envCounter;
	Environment *env;
	vector<vector<CSElement>> controls;
	stack<CSElement> leftStack;
	stack<CSElement> rightStack;
	bool printControlCreation, printControls;
	CSElement *lookupVal;
	int lookupType;

private:

	string getTypeString(int type)
	{
		if (type == 0)
			return "STRING";
		else if (type == 1)
			return "INTEGER";
		else if (type == 2)
			return "IDENT";
		else if (type == 3)
			return "RULE";
		else
			return "UNKNOWN!";
	}

public:
	CSEMachine(ExpTree* mTree)
	{
		stdTree = mTree;

		printControlCreation = true;				//Print switch
		buildControlStructures(mTree, 1);

		printControls = true;						//Print switch
		if(printControls)
			printControlStructures();
		initializeCSEMachine();
		runCSEMachine();
	}

	void buildControlStructures(ExpTree* mTree, int index)
	{
		if (mTree == NULL)
			return;

		//Helper to debug how deltas are made for the control structures.
		if(printControlCreation)
			cout << index << ".\t" << mTree->nodeValue << " is a " << getTypeString(mTree->nodeType) << endl;

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
			currentEl = new CSElement("lambda", deltaNumber, varName, -1);
			controls[index - 1].push_back(*currentEl);
			//if (mTree->childNode != NULL)		Commented him out assuming theres always a right child to LAMBDA.
			buildControlStructures(mTree->childNode->siblingNode, deltaNumber + 1);
		}
		else if (mTree->nodeValue == "tau")
		{
			currentEl = new CSElement(mTree->nodeValue);	//tau
			int tauCnt = 0;
			ExpTree *runner = mTree->childNode;
			while (runner != NULL)
			{
				tauCnt++;
				runner = runner->siblingNode;
			}
			currentEl->index = tauCnt;
			controls[index - 1].push_back(*currentEl);		//push tau on CS
			ExpTree *child = mTree->childNode;
			while (child != NULL)
			{
				buildControlStructures(child, index);
				// We dont push children on stack because they'll go 
				// to the else clause below. Hurrah!
				child = child->siblingNode;
			}
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

	void pushDeltaOnStack(int delta)
	{

		// Create an environment marker for that delta
		CSElement *freshEnv = new CSElement("env", delta, "", -1);
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
		pushDeltaOnStack(0);

		env = new Environment("", new CSElement("none"), GRAM_RULE);


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

	bool lookupVar(string name)
	{
		
		for (int i = 0; i < env->variable.size(); i++)
		{
			if (env->variable[i] == name)
			{
				lookupVal = env->value[i];
				lookupType = env->value[i]->type;
				return true;
			}
		}
		Environment *temp = env->parent;
		while (temp != NULL)
		{
			if (temp->variable[0] == name)
			{
				lookupVal = temp->value;
				lookupType = temp->value->type;
				return true;
			}
			temp = temp->parent;
		}
		return false;
	}

	void runCSEMachine()		//Assumes there is atleast one delta/environment on the machine
	{
		while (!leftStack.empty())
		{
			printCSE();
			leftStack.pop();
			//processCSEMachine();
		}
	}

	void processCSEMachine()
	{
		//leftStack.pop();

		// RULE 3
		if (leftStack.top().value == "not")
		{
			leftStack.pop();
			CSElement node = rightStack.top();
			rightStack.pop();
			if (node.value == "true")
				node.value = "false";
			else if (node.value == "false")
				node.value = "true";
			else
			{
				cout << "WTH! Not needs a boolean!" << endl;
				cin >> lookupType;
				exit(0);
			}
		}
		else if (leftStack.top().value == "+")
		{
			CSElement op1 = rightStack.top();
			rightStack.pop();
			CSElement op2 = rightStack.top();
			rightStack.pop();
			int in1 = stoi(op1.value);
			int in2 = stoi(op2.value);
			op1.value = to_string(in1 + in2);
			rightStack.push(op1);
		}
		else if (leftStack.top().value == "-")
		{
			CSElement op1 = rightStack.top();
			rightStack.pop();
			CSElement op2 = rightStack.top();
			rightStack.pop();
			int in1 = stoi(op1.value);
			int in2 = stoi(op2.value);
			op1.value = to_string(in1 - in2);
			rightStack.push(op1);
		}
		else if (leftStack.top().value == "/")
		{
			CSElement op1 = rightStack.top();
			rightStack.pop();
			CSElement op2 = rightStack.top();
			rightStack.pop();
			int in1 = stoi(op1.value);
			int in2 = stoi(op2.value);
			op1.value = to_string(in1 / in2);
			rightStack.push(op1);
		}
		else if (leftStack.top().value == "*")
		{
			CSElement op1 = rightStack.top();
			rightStack.pop();
			CSElement op2 = rightStack.top();
			rightStack.pop();
			int in1 = stoi(op1.value);
			int in2 = stoi(op2.value);
			op1.value = to_string(in1 * in2);
			rightStack.push(op1);
		}
		else if (leftStack.top().value == "**")
		{
			CSElement op1 = rightStack.top();
			rightStack.pop();
			CSElement op2 = rightStack.top();
			rightStack.pop();
			int in1 = stoi(op1.value);
			int in2 = stoi(op2.value);
			float x = pow(in1, in2);
			op1.value = to_string(x);
			rightStack.push(op1);
		}



	}


	
};

#endif	//Close CSEMACHINE_H_INCLUDED