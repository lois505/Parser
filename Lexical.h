#pragma once
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "charClass.h"
#include "CharType.h"
#include "TokenInformationNode.h"
#include "TokenCodes.h"
#include "MyInt.h"
class Lexical
{
private:
	TokenCodes nextToken;
	std::string lexeme;
	std::ifstream inputFile;
	std::map<std::string, MyInt> identifiers;
	std::string currentLine;
	TokenInformationNode* tree;

	std::string errorMessage;
	bool isError = false;
	bool isWarning = false;
	int idCnt = 0;
	int constCnt = 0;
	int opCnt = 0;
public:
	void readInFile(std::string fileName);
	void addChar(CharType nc);
	CharType getNonBlank();
	CharType getChar();
	TokenCodes lex();//���� ��ū���� nextToken�� ����
	TokenCodes lookup(char nextChar);
	CharClass classifyChar(char nextChar);

	void program();
	void statements(TokenInformationNode* node);
	void statement(TokenInformationNode* node);
	MyInt expression(TokenInformationNode* node);
	MyInt termTail(TokenInformationNode* node,bool& add);
	MyInt term(TokenInformationNode* node);
	MyInt factorTail(TokenInformationNode* node,bool& mult);
	MyInt factor(TokenInformationNode* node);

	void printToken(TokenCodes token);
	void printTree();
	void printAllIDs();
};