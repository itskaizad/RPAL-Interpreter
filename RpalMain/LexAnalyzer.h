#pragma once
#include <iostream>
#include <string>
#include <ctype.h>
#include <cstdio>

using namespace std;


#ifndef LEXANALYZER_H_INCLUDED
#define LEXANALYZER_H_INCLUDED

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

class LexAnalyzer {
public:
	char nextChar;
	int charClass;
	string lexeme;

private:
	string input;

public:
	LexAnalyzer(string inputNew) {
		input = inputNew;
		nextChar = ' ';
		charClass = ERROR;
		lexeme = "";
	}

	~LexAnalyzer() {

	}

	void getChar() {
		if (input.size() > 0) {
			nextChar = input[0];
			input.erase(0, 1);
		}
		else
			nextChar = EOF;

		charClass = getCharClass(nextChar);
	}

	static int getCharClass(char msymbol) {

		int mcharClass = ERROR;

		//if( (msymbol > 64 && msymbol < 91) || (msymbol > 96 && msymbol < 123))
		if (isalpha(msymbol))
			mcharClass = LETTER;
		if (isalnum(msymbol) && !isalpha(msymbol))
			mcharClass = DIGIT;
		if (isspace(msymbol) || msymbol == ' ')
			mcharClass = SPACE;
		if (msymbol == EOF)
			mcharClass = STOP;
		if (msymbol == '+' || msymbol == '-' || msymbol == '*' || msymbol == '<'
			|| msymbol == '>' || msymbol == '&' || msymbol == '.'
			|| msymbol == '@' || msymbol == '/' || msymbol == ':'
			|| msymbol == '=' || msymbol == '~' || msymbol == '|'
			|| msymbol == '!' || msymbol == '#' || msymbol == '%'
			|| msymbol == '^' || msymbol == '_' || msymbol == '['
			|| msymbol == ']' || msymbol == '{' || msymbol == '}'
			|| msymbol == '"' || msymbol == '`' || msymbol == '?'
			|| msymbol == '\'' || msymbol == '$')
			mcharClass = OP_CODE;
		if (msymbol == '(')
			mcharClass = OPENING;
		if (msymbol == ')')
			mcharClass = CLOSING;
		if (msymbol == ',')
			mcharClass = COMMA;
		if (msymbol == ';')
			mcharClass = SEMICOL;

		return mcharClass;
	}

	void addCharLexeme() {
		lexeme += nextChar;
	}

	int processNext() {
		lexeme = "";

		while (charClass == SPACE)
		{
			//cout << "Got spaceSymbol";
			getChar();
		}

		if (charClass == ERROR) {
			addCharLexeme();
			getChar();
			return ERROR;
		}

		if (charClass == STOP)
			return STOP;
		if (charClass == OPENING) {
			addCharLexeme();
			getChar();
			return OPENING;
		}
		if (charClass == CLOSING) {
			addCharLexeme();
			getChar();
			return CLOSING;
		}
		if (charClass == SEMICOL) {
			addCharLexeme();
			getChar();
			return SEMICOL;
		}
		if (charClass == COMMA) {
			addCharLexeme();
			getChar();
			return COMMA;
		}

		switch (charClass) {
		case LETTER:
			addCharLexeme();
			getChar();
			while (charClass == LETTER || charClass == DIGIT || nextChar == '_') {
				addCharLexeme();
				getChar();
			}
			return IDENT;
			break;

		case DIGIT:
			addCharLexeme();
			getChar();
			while (charClass == DIGIT) {
				addCharLexeme();
				getChar();
			}
			return INT_LIT;
			break;

		case OP_CODE:
			if (nextChar == '\'') {
				getChar();
				while (nextChar != '\'') {
					addCharLexeme();
					getChar();
				}
				getChar();
				return STRING;
			}
			else if (nextChar == '/') {
				//cout << "Here" << endl;
				addCharLexeme();
				getChar();
				// If the next symbol is / its a comment
				if (nextChar == '/') {											//SECOND   /   if opens here
					//cout << "Here too" << endl;
					addCharLexeme();
					getChar();
					while (nextChar != '\n' && nextChar != '\r'
						&& nextChar != EOF && nextChar != '!') {
						//cout << "Inside" << endl;
						addCharLexeme();
						getChar();
					}
					//cout << "Returning" << endl;
					return COMMENT;
				}
				else if(getCharClass(nextChar) == OP_CODE && nextChar != '/'){															// SECOND   /  else opens here
					addCharLexeme();
					getChar();
					while (getCharClass(nextChar) == OP_CODE && nextChar != '/') {
						addCharLexeme();
						getChar();
					}
					return OPERATOR_CODE;
				}							// Close final / symbol if.

			}
			else {
				addCharLexeme();
				getChar();
				while (getCharClass(nextChar) == OP_CODE && nextChar != '\'') {
					addCharLexeme();
					getChar();
				}
				return OPERATOR_CODE;
			}
			break;
		default:
			return ERROR;

		} // Ends the switch case
		return ERROR;
	}	// Ends processNext();

	string getCodeString(int code) {
		switch (code) {
		case DIGIT:
			return "DIGIT";
			break;
		case LETTER:
			return "LETTER";
			break;
		case IDENT:
			return "IDENT";
			break;
		case INT_LIT:
			return "INT_LIT";
			break;
		case ERROR:
			return "ERROR";
			break;
		case SPACE:
			return "SPACE";
			break;
		case STOP:
			return "STOP";
			break;
		case OP_CODE:
			return "OP_CODE";
			break;
		case OPERATOR_CODE:
			return "OPERATOR_CODE";
			break;
		case STRING:
			return "STRING";
			break;
		case COMMENT:
			return "COMMENT";
			break;
		case OPENING:
			return "OPENING";
		case CLOSING:
			return "CLOSING";
			break;
		case SEMICOL:
			return "SEMICOL";
			break;
		case COMMA:
			return "COMMA";
			break;
		}
		return "ERROR";
	}

};

#endif // LEXANALYZER_H_INCLUDED
