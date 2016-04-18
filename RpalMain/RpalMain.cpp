// RpalMain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include "LexAnalyzer.h"
#include "ExpTree.h"
#include "Parser.h"
#include "Standardizer.h"
using namespace std;

int main(int argc, char* argv[])
{

	enum ExpNodeType
	{
		STRVALUE,
		INTVALUE,
		ID_NAME,
		GRAM_RULE
	};

	enum Token
	{
		DIGIT, LETTER, IDENT, INT_LIT, ERROR, SPACE, STOP, OP_CODE, OPERATOR_CODE,
		STRING, COMMENT, OPENING, CLOSING, SEMICOL, COMMA
	};

	
	ifstream infs("inputCode.txt");
	string content((istreambuf_iterator<char>(infs)), (istreambuf_iterator<char>()));
	//content = "let X=3 and Y=5 in Print(X+Y)";
	LexAnalyzer myLex(content);
	int tokType = myLex.processNext();
	
	
	Parser mParser(&myLex);
	//mParser.test();
	mParser.procE();
	ExpTree::prettyPrint(mParser.getTopTree());
	cout << "\n\n STD TREE\n\n" << endl;
	ExpTree::prettyPrint(Standardizer::standardizeTree(mParser.getTopTree()));
	cout << "Treminated succesfully! Press any key to continue..." << endl;
	cin >> content;
	

    return 0;
}

