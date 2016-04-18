#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include "ExpTree.h"

using namespace std;

#ifndef STANDARDIZER_H
#define STANDARDIZER_H

class Standardizer
{

public:
	static ExpTree* standardizeTree(ExpTree *tree)
	{
		if (tree == NULL)
			return NULL;

		if (tree->childNode != NULL)
			standardizeTree(tree->childNode);

		if (tree->siblingNode != NULL)
			standardizeTree(tree->siblingNode);

		if (tree->nodeValue == "let")
		{
			standardizeLet(tree);
		}
		else if (tree->nodeValue == "within")
		{
			standardizeWithin(tree);
		}
		else if (tree->nodeValue == "function_form")
		{
			standardizeFncForm(tree);
		}
		else if (tree->nodeValue == "lambda")
		{
			standardizeLambda(tree);
		}
		else if (tree->nodeValue == "@")
		{
			standardizeInfix(tree);
		}
		else if (tree->nodeValue == "and")
		{
			standardizeAnd(tree);
		}
		else if (tree->nodeValue == "where")
		{
			standardizeWhere(tree);
		}
		else if (tree->nodeValue == "rec")
		{
			standardizeRec(tree);
		}

		return tree;
	}

private:
	static void standardizeLet(ExpTree *tree)
	{
		//TODO Write implementation here!
		tree->nodeValue = "gamma";
		ExpTree *eq = tree->childNode;
		ExpTree *p = eq->siblingNode;
		ExpTree *x = eq->childNode;
		ExpTree *e = x->siblingNode;
		eq->nodeValue = "lambda";
		x->siblingNode = p;
		eq->siblingNode = e;
	}

	static void standardizeWithin(ExpTree *tree)
	{
		//TODO Write implementation here!
		tree->nodeValue = "=";

		ExpTree *eq1 = tree->childNode;
		ExpTree *eq2 = eq1->siblingNode;

		ExpTree *x1 = eq1->childNode;
		ExpTree *x2 = eq2->childNode;
		eq1->childNode = NULL;
		eq1->siblingNode = NULL;
		eq2->childNode = NULL;

		ExpTree *e1 = x1->siblingNode;
		ExpTree *e2 = x2->siblingNode;
		x1->siblingNode = NULL;
		x2->siblingNode = NULL;

		tree->childNode = x2;
		x2->siblingNode = eq2;
		eq2->nodeValue = "gamma";
		eq2->childNode = eq1;
		eq1->nodeValue = "lambda";
		eq1->siblingNode = e1;
		eq1->childNode = x1;
		x1->siblingNode = e2;
	}

	static void standardizeFncForm(ExpTree *tree)
	{
		//TODO Write implementation here!
		ExpTree *p = tree->childNode;
		ExpTree *v = p->siblingNode;
		tree->nodeValue;

		ExpTree *lam = new ExpTree("lambda", 3, v, NULL);
		p->siblingNode = lam;
		ExpTree *next = v->siblingNode;
		while (next->siblingNode != NULL)
		{
			lam = new ExpTree("lambda", 3, NULL, NULL);
			v->siblingNode = lam;
			lam->childNode = next;
			v = next;
			next = v->siblingNode;
		}
	}

	static void standardizeLambda(ExpTree *tree)
	{
		//TODO Write implementation here!
		ExpTree *v = tree->childNode;
		ExpTree *next = v->siblingNode;
		while (next->siblingNode != NULL)
		{
			ExpTree *lam = new ExpTree("lambda", 3, next, NULL);
			v->siblingNode = lam;
			v = next;
			next = next->siblingNode;
		}
	}

	static void standardizeInfix(ExpTree *tree)
	{
		//TODO Write implementation here!
		tree->nodeValue = "gamma";
		ExpTree *e1 = tree->childNode;
		ExpTree *n = e1->siblingNode;
		ExpTree *e2 = n->siblingNode;
		ExpTree *gamma = new ExpTree("gamma", 3, n, NULL);
		gamma->siblingNode = e2;
		tree->childNode = gamma;
		n->siblingNode = e1;
		e1->siblingNode = NULL;
	}

	static void standardizeAnd(ExpTree *tree)
	{
		//TODO Write implementation here!
		ExpTree *comma = new ExpTree(",", 3, NULL, NULL);
		ExpTree *tau = new ExpTree("tau", 3, NULL, NULL);
		comma->siblingNode = tau;
		ExpTree *eq = tree->childNode;
		ExpTree *x = eq->childNode;
		ExpTree *e = x->siblingNode;
		comma->childNode = x;
		tau->childNode = e;
		eq = eq->siblingNode;
		while (eq != NULL)
		{
			x->siblingNode = eq->childNode;
			x = x->siblingNode;
			e->siblingNode = eq->childNode->siblingNode;
			e = e->siblingNode;
			eq = eq->siblingNode;
		}
		x->siblingNode = NULL;
		tree->nodeValue = "=";
		tree->childNode = comma;
	}

	static void standardizeWhere(ExpTree *tree)
	{
		//TODO Write implementation here!
		tree->nodeValue = "gamma";
		ExpTree *p = tree->childNode;
		ExpTree *eq = p->siblingNode;
		ExpTree *x = eq->childNode;
		ExpTree *e = x->siblingNode;
		eq->nodeValue = "lambda";
		tree->childNode = eq;
		x->siblingNode = p;
		p->siblingNode = NULL;
		eq->siblingNode = e;
	}

	static void standardizeRec(ExpTree *tree)
	{
		//TODO Write implementation here!
		tree->nodeValue = "=";
		ExpTree *x = tree->childNode->childNode;
		delete tree->childNode;
		tree->childNode = new ExpTree(x);
		ExpTree *ystar = new ExpTree("<Y*>", 3, NULL, NULL);
		ExpTree *gamma = new ExpTree("gamma", 3, ystar, NULL);
		tree->childNode->siblingNode = gamma;
		ExpTree *lambda = new ExpTree("lambda", 3, x, NULL);
		gamma->childNode->siblingNode = lambda;
	}


};

#endif // !STANDARDIZER_H
