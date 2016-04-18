#pragma once
#include <iostream>
#include <string>
#include <ctype.h>
#include <cstdio>
#include <stack>
#include <cstdlib>
#include "ExpTree.h"
using namespace std;


class Parser
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
		DIGIT,
		LETTER,
		IDENT,
		INT_LIT,
		ERROR,
		SPACE,
		STOP,
		OP_CODE,
		OPERATOR_CODE,
		STRING,
		COMMENT,
		OPENING,
		CLOSING,
		SEMICOL,
		COMMA
	};


public:
	stack<ExpTree> treeStack;
	int tokenType;
	string tokenValue;
	bool isDebug;

private:
	LexAnalyzer *mLex;

public:

	Parser(LexAnalyzer *mLexNew)
	{
		isDebug = false;
		mLex = mLexNew;
		tokenType = mLex->processNext();
		tokenValue = mLex->lexeme;
		while (tokenType == COMMENT || tokenType == ERROR)
		{
			tokenType = mLex->processNext();
			tokenValue = mLex->lexeme;
			//cout << "New token: " << tokenValue << endl;
		}
	}

	ExpTree makeTree(string mVal, int popCount)
	{
		/*
		Pop stack, invert and push into temporary stack mechanism
		The first child to be popped from the original stack becomes the LAST sibling!
		*/
		//cout << "\nStack size: " << treeStack.size() << "\t" << mVal << "|";
		//cout << "Name:" << mVal << " count:" << popCount << endl;
		ExpTree *newTree = new ExpTree(mVal, GRAM_RULE, NULL, NULL);
		if (popCount == 0) {
			treeStack.push(*newTree);
			return *newTree;
		}

		stack<ExpTree> tempStack;
		for (int i = 0; i < popCount; i++)
		{
			ExpTree tmp = treeStack.top();
			//cout << "-- " << tmp.nodeValue;
			treeStack.pop();
			tempStack.push(tmp);
		}
		//cout << endl;
		//cout << tempStack.size() << endl;

		ExpTree anotherTemp = tempStack.top();
		ExpTree *tempBigSib = new ExpTree(&anotherTemp);
		newTree->childNode = tempBigSib;
		//cout << "NT-C-V = " << newTree->childNode->nodeValue << endl;
		tempStack.pop();
		for (int i = 1; i < popCount; i++)	// Starts from 1 because first node is taken care of!
		{
			anotherTemp = tempStack.top();
			tempBigSib->siblingNode = new ExpTree(&anotherTemp);
			tempBigSib = tempBigSib->siblingNode;
			tempStack.pop();
		}
		treeStack.push(*newTree);
		//ExpTree::prettyPrint(newTree);
		return *newTree;
	}


	/*
	void test() {
	makeTree("1.3", 0);
	makeTree("1.2.2", 0);
	makeTree("1.2.1", 0);
	makeTree("1.2", 2);
	makeTree("1.1", 0);
	makeTree("1", 3);
	}
	*/

	ExpTree* getTopTree()
	{
		ExpTree *val = &(treeStack.top());
		return val;
	}

	void readToken(string mToken)
	{

		if (tokenType == STOP)
		{
			cout << "EXCEPTION!!!	Input terminated unexpectedly" << endl;
			cin >> mToken;
			exit(0);
		}

		while (tokenType == COMMENT)
		{
			tokenType = mLex->processNext();
			tokenValue = mLex->lexeme;
		}

		//cout << "Reading " << mToken << " against value " << tokenValue << endl;

		if (mToken == "<ID>" || mToken == "<INT>" || mToken == "<STR>")											// If we're reading a variable 
		{
			if (isKeyword(tokenValue) && mToken != "<STR>")			// If our input key is a KEYWORD
			{
				cout << "Expected " << mToken << " found KEYWORD " << tokenValue << endl;
				cin >> mToken;
				exit(0);
			}
			else								// If our input token is NOT a KEYWORD --- IDEALLY this way!
			{
				if (mToken == "<ID>" && tokenType != IDENT)			// if we want an ID and our input is not
				{
					cout << "ERROR:\t Wanted IDENTIFIER, found " << mLex->getCodeString(tokenType) << endl;
					cin >> mToken;
					exit(0);
				}
				if (mToken == "<INT>" && tokenType != INT_LIT)		// if we want an INT and out input is not
				{
					cout << "ERROR:\t Wanted INTEGER, found " << mLex->getCodeString(tokenType) << endl;
					cin >> mToken;
					exit(0);
				}
				if (mToken == "<STR>" && tokenType != STRING)		// if we want a STR and our input is not
				{
					cout << "ERROR:\t Wanted STRING, found " << mLex->getCodeString(tokenType) << endl;
					cin >> mToken;
					exit(0);
				}

				// If you reached here, all's good, so make your customized tree node.
				if (mToken == "<ID>")
					treeStack.push(ExpTree(tokenValue, ID_NAME, nullptr, nullptr));
				else if (mToken == "<INT>")
					treeStack.push(ExpTree(tokenValue, INTVALUE, nullptr, nullptr));
				else if (mToken == "<STR>")
					treeStack.push(ExpTree(tokenValue, STRVALUE, nullptr, nullptr));
				//cout << "Made token: " << mToken << tokenValue << "\tStackSize:" << treeStack.size() << endl;
			}

		}
		else																					// If we are reading a KEYWORD
		{
			if (mToken != tokenValue)
			{
				cout << "KEYWORD MISMATCH:\t Wanted " << mToken << " , found " << tokenValue << endl;
				cin >> mToken;
				exit(0);
			}
		}

		// Read the next token!					THIS IS IMPORTANT!!!
		tokenType = mLex->processNext();
		tokenValue = mLex->lexeme;

		// If the next token is a COMMENT, read one more, and one more, and so on
		// till we get a token that not's a comment!

		// THIS IS VERY IMPORTANT OR SHIT WILL BREAK!
		while (tokenType == COMMENT)
		{
			tokenType = mLex->processNext();
			tokenValue = mLex->lexeme;
		}
		//cout << "Next Token:" << " " << tokenValue << endl;

	}

	//method to help readToken find if an identifier is a keyword
	static bool isKeyword(string mTok)
	{
		string keys[] = { "let", "fn", "in", ".", "where",
			",", "aug", "->", "|",
			"or", "&", "not", "gr", ">", "ge", ">=", "ls", "<", "le", "<=", "eq", "ne",
			"+", "-", "*", "/", "**", "@",
			"true", "false", "nil", "(", ")", "dummy",
			"within", "and", "rec", "=" };

		for (int i = 0; i < 38; i++) {
			if (keys[i] == mTok)
				return true;
		}
		return false;
	}

	void procE()
	{
		if (isDebug) {
			cout << "Entering E()............stack size:" << treeStack.size() << "\n";
		}
		if (tokenValue == "let")
		{
			readToken("let");
			procD();
			readToken("in");
			procE();
			makeTree("let", 2);
		}
		else if (tokenValue == "fn")
		{
			readToken("fn");
			int N = 0;
			do {
				procVb();
				N++;
			} while (tokenType == IDENT || tokenType == OPENING);
			readToken(".");
			procE();
			makeTree("lambda", N + 1);
		}
		else
		{
			procEw();
		}

		if(isDebug)
			cout << ".........Returning E()...stack size:" << treeStack.size() << "\n";
	}

	void procEw()
	{
		if (isDebug) {
			cout << "Entering Ew()............stack size:" << treeStack.size() << "\n";
		}
		procT();
		if (tokenValue == "where")
		{
			readToken("where");
			procDr();
			makeTree("where", 2);
		}

		if (isDebug)
			cout << ".........Returning Ew()...stack size:" << treeStack.size() << "\n";
	}

	void procT()
	{
		if (isDebug) {
			cout << "Entering T()............stack size:" << treeStack.size() << "\n";
		}
		procTa();
		int N = 1;
		while (tokenValue == ",")
		{
			readToken(",");
			procTa();
			N = N + 1;
		}
		if (N > 1)
			makeTree("tau", N);


		if (isDebug)
			cout << ".........Returning T()...stack size:" << treeStack.size() << "\n";
	}

	void procTa()
	{
		if (isDebug) {
			cout << "Entering Ta()............stack size:" << treeStack.size() << "\n";
		}
		procTc();
		while (tokenValue == "aug")
		{
			readToken("aug");
			procTc();
			makeTree("aug", 2);
		}


		if (isDebug)
			cout << ".........Returning Ta()...stack size:" << treeStack.size() << "\n";
	}

	void procTc()
	{
		if (isDebug) {
			cout << "Entering Tc()............stack size:" << treeStack.size() << "\n";
		}
		procB();
		if (tokenValue == "->")
		{
			readToken("->");
			procTc();
			readToken("|");
			procTc();
			makeTree("->", 3);
		}


		if (isDebug)
			cout << ".........Returning Tc()...stack size:" << treeStack.size() << "\n";
	}

	void procB()
	{
		if (isDebug) {
			cout << "Entering B()............stack size:" << treeStack.size() << "\n";
		}
		procBt();
		while (tokenValue == "or")
		{
			readToken("or");
			procBt();
			makeTree("or", 2);
		}


		if (isDebug)
			cout << ".........Returning B()...stack size:" << treeStack.size() << "\n";
	}

	void procBt()
	{
		if (isDebug) {
			cout << "Entering Bt()............stack size:" << treeStack.size() << "\n";
		}
		procBs();
		while (tokenValue == "&")
		{
			readToken("&");
			procBs();
			makeTree("&", 2);
		}


		if (isDebug)
			cout << ".........Returning Bt()...stack size:" << treeStack.size() << "\n";
	}

	void procBs()
	{
		if (isDebug) {
			cout << "Entering Bs()............stack size:" << treeStack.size() << "\n";
		}
		if (tokenValue == "not")
		{
			readToken("not");
			procBp();
			makeTree("not", 1);
		}
		else
			procBp();


		if (isDebug)
			cout << ".........Returning Bs()...stack size:" << treeStack.size() << "\n";
	}

	void procBp()
	{
		if (isDebug) {
			cout << "Entering Bp()............stack size:" << treeStack.size() << "\n";
		}
		procA();
		if (tokenValue == "gr")
		{
			readToken("gr");
			procA();
			makeTree("gr", 2);
		}
		else if (tokenValue == ">")
		{
			readToken(">");
			procA();
			makeTree("gr", 2);
		}
		else if (tokenValue == "ge")
		{
			readToken("ge");
			procA();
			makeTree("ge", 2);
		}
		else if (tokenValue == ">=")
		{
			readToken(">=");
			procA();
			makeTree("ge", 2);
		}
		else if (tokenValue == "ls")
		{
			readToken("ls");
			procA();
			makeTree("ls", 2);
		}
		else if (tokenValue == "<")
		{
			readToken("<");
			procA();
			makeTree("ls", 2);
		}
		else if (tokenValue == "le")
		{
			readToken("le");
			procA();
			makeTree("le", 2);
		}
		else if (tokenValue == "<=")
		{
			readToken("<=");
			procA();
			makeTree("le", 2);
		}
		else if (tokenValue == "eq")
		{
			readToken("eq");
			procA();
			makeTree("eq", 2);
		}
		else if (tokenValue == "ne")
		{
			readToken("ne");
			procA();
			makeTree("ne", 2);
		}


		if (isDebug)
			cout << ".........Returning Bp()...stack size:" << treeStack.size() << "\n";

	}

	void procA()
	{

		if (isDebug) {
			cout << "Entering A()............stack size:" << treeStack.size() << "\n";
		}
		if (tokenValue == "-" && tokenType!= STRING)
		{
			readToken("-");
			procAt();
			makeTree("neg", 1);
		}
		else if (tokenValue == "+" && tokenType!= STRING)
		{
			readToken("+");
			procAt();
		}
		else
			procAt();
		// if-else ends here

		while ((tokenValue == "-" || tokenValue == "+") && tokenType != STRING)
		{
			string tempToken = tokenValue;
			readToken(tokenValue);
			procAt();
			makeTree(tempToken, 2);
		}


		if (isDebug)
			cout << ".........Returning A()...stack size:" << treeStack.size() << "\n";
	}

	void procAt()
	{
		if (isDebug) {
			cout << "Entering At()............stack size:" << treeStack.size() << "\n";
		}
		procAf();
		while (tokenValue == "*" || tokenValue == "/")
		{
			if (tokenValue == "*")
			{
				readToken("*");
				procAf();
				makeTree("*", 2);
			}
			else if (tokenValue == "/")
			{
				readToken("/");
				procAf();
				makeTree("/", 2);
			}
		}


		if (isDebug)
			cout << ".........Returning At()...stack size:" << treeStack.size() << "\n";
	}

	void procAf()
	{
		if (isDebug) {
			cout << "Entering Af()............stack size:" << treeStack.size() << "\n";
		}
		procAp();
		if (tokenValue == "**")
		{
			readToken("**");
			procAf();
			makeTree("**", 2);
		}


		if (isDebug)
			cout << ".........Returning Af()...stack size:" << treeStack.size() << "\n";
	}

	void procAp()
	{
		if (isDebug) {
			cout << "Entering Ap()............stack size:" << treeStack.size() << "\n";
		}
		procR();
		while (tokenValue == "@")
		{
			readToken("@");
			readToken("<ID>");
			procR();
			makeTree("@", 3);
		}


		if (isDebug)
			cout << ".........Returning Ap()...stack size:" << treeStack.size() << "\n";
	}

	void procR()
	{
		if (isDebug) {
			cout << "Entering R()............stack size:" << treeStack.size() << "\n";
		}
		procRn();
		while (tokenValue == "true" ||
			tokenValue == "false" ||
			tokenValue == "nil" ||
			tokenValue == "dummy" ||
			(tokenType == IDENT && !(isKeyword(tokenValue))) ||
			tokenType == OPENING ||
			tokenType == INT_LIT ||
			tokenType == STRING)
		{
			procRn();
			makeTree("gamma", 2);
		}


		if (isDebug)
			cout << ".........Returning R()...stack size:" << treeStack.size() << "\n";
	}

	void procRn()
	{
		if (isDebug) {
			cout << "Entering Rn()............stack size:" << treeStack.size() << "\n";
		}
		if (isKeyword(tokenValue) && tokenType != STRING)		// other than <ID> <INT> and <STR>
		{
			// check for true, false, nil and dummy
			if (tokenValue == "true")
			{
				readToken("true");
				makeTree("<true>", 0);
			}
			else if (tokenValue == "false")
			{
				readToken("false");
				makeTree("<false>", 0);
			}
			else if (tokenValue == "nil")
			{
				readToken("nil");
				makeTree("<nil>", 0);
			}
			else if (tokenValue == "dummy")
			{
				readToken("dummy");
				makeTree("<dummy>", 0);
			}
			else if (tokenValue == "(")
			{
				if (tokenType == OPENING)
				{
					readToken("(");
					procE();
					readToken(")");
				}
				else if (tokenType == STRING)
				{
					readToken("<STR>");
				}
			}
		}
		else							// <ID> <INT> and <STR>
		{
			switch (tokenType)
			{
			case IDENT:
				readToken("<ID>");
				break;
			case INT_LIT:
				readToken("<INT>");
				break;
			case STRING:
				readToken("<STR>");
				break;
			}
		}


		if (isDebug)
			cout << ".........Returning Rn()...stack size:" << treeStack.size() << "\n";
	}

	void procD()
	{
		if (isDebug) {
			cout << "Entering D()............stack size:" << treeStack.size() << "\n";
		}
		procDa();
		if (tokenValue == "within")
		{
			readToken("within");
			procD();
			makeTree("within", 2);
		}


		if (isDebug)
			cout << ".........Returning D()...stack size:" << treeStack.size() << "\n";
	}

	void procDa()
	{
		if (isDebug) {
			cout << "Entering Da()............stack size:" << treeStack.size() << "\n";
		}
		procDr();
		int N = 1;
		while (tokenValue == "and")
		{
			readToken("and");
			procDr();
			N = N + 1;
		}

		if (N > 1)
			makeTree("and", N);


		if (isDebug)
			cout << ".........Returning Da()...stack size:" << treeStack.size() << "\n";
	}

	void procDr()
	{
		if (isDebug) {
			cout << "Entering Dr()............stack size:" << treeStack.size() << "\n";
		}
		if (tokenValue == "rec")
		{
			readToken("rec");
			procDb();
			makeTree("rec", 1);
		}
		else
			procDb();


		if (isDebug)
			cout << ".........Returning Dr()...stack size:" << treeStack.size() << "\n";
	}

	void procDb()
	{
		if (isDebug) {
			cout << "Entering Db()............stack size:" << treeStack.size() << "\n";
		}
		if (tokenValue == "(")
		{
			readToken("(");
			procD();
			readToken(")");
		}
		else if (tokenType == IDENT && !isKeyword(tokenValue))
		{
			readToken("<ID>");
			int childrenCt = 1;
			// Explore the first rule with Vl
			// This can be either <ID> followed by ( , <ID> ) or ( = )
			if (tokenValue == "," || tokenValue == "=")
			{
				while (tokenValue == ",")
				{
					readToken(",");
					readToken("<ID>");
					childrenCt++;
				}
				if (childrenCt > 1)
					makeTree(",", childrenCt);

				readToken("=");
				procE();
				makeTree("=", 2);
			}
			else
			{
				do
				{
					procVb();
					childrenCt++;
				} while (tokenType == OPENING || (tokenType == IDENT && !isKeyword(tokenValue)));
				readToken("=");
				procE();
				makeTree("function_form", childrenCt + 1);
			}
		}


		if (isDebug)
			cout << ".........Returning Db()...stack size:" << treeStack.size() << "\n";
	}

	void procVb()
	{
		if (isDebug) {
			cout << "Entering Vb()............stack size:" << treeStack.size() << "\n";
		}
		if (tokenType == IDENT && !isKeyword(tokenValue))
			readToken("<ID>");
		else if (tokenType == OPENING)
		{
			readToken("(");
			if (tokenType == CLOSING)
			{
				readToken(")");
				makeTree("()", 0);
			}
			else
			{
				procVl();
				readToken(")");
			}
		}


		if (isDebug)
			cout << ".........Returning Vb()...stack size:" << treeStack.size() << "\n";
	}

	void procVl()
	{
		if (isDebug) {
			cout << "Entering Vl()............stack size:" << treeStack.size() << "\n";
		}
		readToken("<ID>");
		int N = 1;
		while (tokenValue == ",")
		{
			readToken(",");
			readToken("<ID>");
			N++;
		}
		if (N > 1)
			makeTree(",", N);


		if (isDebug)
			cout << ".........Returning Vl()...stack size:" << treeStack.size() << "\n";
	}

};