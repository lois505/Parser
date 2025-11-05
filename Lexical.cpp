#include "Lexical.h"
#include <iostream>
using namespace std;
void Lexical::readInFile(std::string fileName)
{
	this->inputFile.open(fileName);
}

void Lexical::addChar(CharType nc)
{
	if (lexeme.length() <= 98)
		lexeme += nc.c;
	else
		std::cout << "Error - lexeme is too long" << std::endl;
}

CharType Lexical::getNonBlank()
{
	CharType result;
	while ( (result = getChar()).charClass!=CharClass::EF && isspace(result.c));//EOF ==-2?
	return result;
}

 CharType Lexical::getChar()
{
	CharType nextChar;
	if (inputFile.get(nextChar.c)) {
		nextChar.charClass = classifyChar(nextChar.c);
	}
	else
		nextChar.charClass = CharClass::EF;
	return nextChar;
}
 /*
 Lexical�� nextToken(TokenCodes)�� ������Ʈ�ϰ�
 �װ� return
 */
TokenCodes Lexical::lex()
{
	bool inWhile = false;
	lexeme.clear();
	CharType nextChar = getNonBlank();
	if (nextChar.charClass == CharClass::EF) return TokenCodes::UK;
	switch (nextChar.charClass)
	{
	case CharClass::LETTER:
		addChar(nextChar);
		nextChar = getChar();
		while (nextChar.charClass == CharClass::LETTER || nextChar.charClass == CharClass::DIGIT)
		{
			addChar(nextChar);
			nextChar = getChar();
		}
		inputFile.putback(nextChar.c);
		nextToken = TokenCodes::ID;
		idCnt++;
		break;
	case CharClass::DIGIT:
		addChar(nextChar);
		nextChar = getChar();
		while (nextChar.charClass == CharClass::DIGIT)
		{
			addChar(nextChar);
			nextChar = getChar();
		}
		inputFile.putback(nextChar.c);
		nextToken = TokenCodes::CONST;
		constCnt++;
		break;
	case CharClass::UNKNOWN:
		addChar(nextChar);
		nextToken = lookup(nextChar.c);
		break;
	}
	//printToken(nextToken);
	currentLine += lexeme + ' ';
	return nextToken;
}

CharClass Lexical::classifyChar(char nextChar)
{
	if (isalpha(nextChar))
		return CharClass::LETTER;
	else if (isdigit(nextChar))
		return CharClass::DIGIT;
	else
		return CharClass::UNKNOWN;
}


void Lexical::program()
{
	tree = new TokenInformationNode();
	TokenInformationNode* newNode = tree->addNonterminal(Nonterminals::PROGRAM);
	lex();
	statements(newNode);
	cout << currentLine << endl;
	cout << "ID : " << idCnt << " CONST : " << constCnt << " OP : " << opCnt << endl;
	if (!isError && !isWarning)
		cout << "(OK)\n";
}

void Lexical::statements(TokenInformationNode* node)
{
	TokenInformationNode* newNode = node->addNonterminal(Nonterminals::STATEMENTS);
	statement(newNode);
	if (nextToken == TokenCodes::SEMI_COLON) {
		newNode->addTerminal(TokenCodes::SEMI_COLON, "SEMI_COLON");
		cout << currentLine << endl;
		cout << "ID : " << idCnt << " CONST : " << constCnt << " OP : " << opCnt << endl;
		if (!isError && !isWarning)
		{
			cout << "(OK)\n";
		}
		else
		{
			cout << errorMessage << endl;;
		}
		cout << "\n";
		idCnt = 0; constCnt = 0; opCnt = 0;
		isError = false; isWarning = false;
		currentLine.clear();
		lex();
		statements(newNode);
	}
}

void Lexical::statement(TokenInformationNode* node)
{
	TokenInformationNode* newNode = node->addNonterminal(Nonterminals::STATEMENT);
	if (nextToken == TokenCodes::ID)
	{
		newNode->addTerminal(TokenCodes::ID, lexeme);
		std::string id = lexeme;
		lex();
		if (nextToken == TokenCodes::ASS_OP)
		{
			newNode->addTerminal(TokenCodes::ASS_OP, "ASS_OP");
			lex();
			identifiers[id] = expression(newNode);
		}
	}
}

MyInt Lexical::expression(TokenInformationNode* node)
{
	bool add = true;
	TokenInformationNode* newNode = node->addNonterminal(Nonterminals::EXPRESSION);
	MyInt a= term(newNode);
	MyInt b=termTail(newNode,add);
	if(add)
	return a + b;
	return a - b;
}

MyInt Lexical::termTail(TokenInformationNode* node,bool& add)
{
	TokenInformationNode* newNode = node->addNonterminal(Nonterminals::TERMTAIL);
	MyInt a(true,0);
	MyInt b(true,0);
	bool ad = true;
	while (nextToken == TokenCodes::ADD_OP)
	{
		char temp = lexeme[0];
		add = (lexeme[0] == '+');
		newNode->addTerminal(TokenCodes::ADD_OP, "ADD_OP");
		opCnt++;
		lex();
		if (nextToken == TokenCodes::ADD_OP || nextToken == TokenCodes::ASS_OP || nextToken == TokenCodes::MULT_OP)
		{
			lexeme = temp;
			nextToken = TokenCodes::ADD_OP;
			currentLine.erase(currentLine.end() - 2, currentLine.end());
			errorMessage.clear();
			isWarning = true;
			errorMessage = "(Warning) 중복연산자 제거";
			opCnt--;
			continue;
		}
		a = term(newNode);
		b = termTail(newNode,ad);
	}
	if(ad)
	return a + b;
	return a - b;
}

MyInt Lexical::term(TokenInformationNode* node)
{
	bool mult = true;
	TokenInformationNode* newNode = node->addNonterminal(Nonterminals::TERM);
	MyInt a = factor(newNode);
	MyInt b = factorTail(newNode,mult);
	if(mult)
	return a * b;
	return a / b;
}

MyInt Lexical::factorTail(TokenInformationNode* node,bool& mult)
{
	TokenInformationNode* newNode = node->addNonterminal(Nonterminals::FACTORTAIL);
	bool m = true;
	while (nextToken == TokenCodes::MULT_OP)
	{
		char temp = lexeme[0];
		mult = (lexeme[0] == '*');
		newNode->addTerminal(TokenCodes::MULT_OP, "MULT_OP");
		opCnt++;
		lex();
		if (nextToken == TokenCodes::ADD_OP || nextToken == TokenCodes::ASS_OP || nextToken == TokenCodes::MULT_OP)
		{
			lexeme = temp;
			nextToken = TokenCodes::MULT_OP;
			currentLine.erase(currentLine.end() - 2, currentLine.end());
			errorMessage.clear();
			isWarning = true;
			errorMessage = "(Warning) 중복연산자 제거";
			opCnt--;
			continue;
		}
		MyInt a = factor(newNode);
		MyInt b = factorTail(newNode,m);
		if(m)
		return a * b;
		return a / b;
	}
	return MyInt(true,1);
}

MyInt Lexical::factor(TokenInformationNode* node)
{
	MyInt result;
	TokenInformationNode* newNode = node->addNonterminal(Nonterminals::FACTOR);
	if (nextToken == TokenCodes::LEFT_PAREN)
	{
		newNode->addTerminal(TokenCodes::LEFT_PAREN,"LEFT_PAREN");
		lex();
		result = expression(newNode);
		if (nextToken == TokenCodes::RIGHT_PAREN)
		{
			newNode->addTerminal(TokenCodes::RIGHT_PAREN, "RIGHT_PAREN");
			lex();
			return result;
		}
	}
	else if (nextToken == TokenCodes::ID)
	{
		newNode->addTerminal(TokenCodes::ID, lexeme);
		if (identifiers.find(lexeme) != identifiers.end())
		{
			result.revise(identifiers[lexeme]);
		}
		else
		{
			isError = true;
			errorMessage =  "(Error) 정의되지 않은 식별자 " + lexeme + " 참조됨.";
			result.revise(false, 0);
			identifiers[lexeme] = result;
		}
		lex();
		return result;
	}
	else if (nextToken == TokenCodes::CONST)
	{
		newNode->addTerminal(TokenCodes::CONST, lexeme);
		result.revise(true, stoi(lexeme));
		lex();
		return result;
	}
}

void Lexical::printToken(TokenCodes token)
{
	/*
	switch (token)
	{
	case TokenCodes::CONST:
		cout << "CONST" << endl;
		break;
	case TokenCodes::ID:
		cout << "ID" << endl;
		break;
	case TokenCodes::ASS_OP:
		cout << "ASS_OP" << endl;
		break;
	case TokenCodes::SEMI_COLON:
		cout << "SEMI_COLON" << endl;
		break;
	case TokenCodes::ADD_OP:
		cout << "ADD_OP" << endl;
		break;
	case TokenCodes::MULT_OP:
		cout << "MULT_OP" << endl;
		break;
	case TokenCodes::LEFT_PAREN:
		cout << "LEFT_PAREN" << endl;
		break;
	case TokenCodes::RIGHT_PAREN:
		cout << "RIGHT_PAREN" << endl;
		break;
	case TokenCodes::UK:
		break;
	default:
		break;
	}*/
}

TokenCodes Lexical::lookup(char nextChar)
{
	switch (nextChar)
	{
	case ':':
		if (inputFile.peek() == '=') {
			addChar(getChar());
			return TokenCodes::ASS_OP;
		}
	case ';':
		return TokenCodes::SEMI_COLON;
	case '+':
	case '-':
		return TokenCodes::ADD_OP;
	case '*':
	case '/' :
		return TokenCodes::MULT_OP;
	case '(':
		return TokenCodes::LEFT_PAREN;
	case ')':
		return TokenCodes::RIGHT_PAREN;
	default :
		return TokenCodes::UK;
	}

}
void Lexical::printTree()
{
	tree->printTree();
}

void Lexical::printAllIDs()
{
	for (auto ids : identifiers)
	{
		cout << ids.first << " : " << ids.second <<' ';
	}
}