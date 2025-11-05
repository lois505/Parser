#pragma once
#include "TokenCodes.h"
#include "Nonterminals.h"
#include <string>
#include <vector>
#include <iostream>
class TokenInformationNode
{
protected:
	TokenCodes code;
	Nonterminals nonterm;
	std::string id;
	std::vector<TokenInformationNode*> childs;
public:
	/*input의 child에 Nonterminal 달고 child리턴*/
	TokenInformationNode* addNonterminal(Nonterminals type)
	{
		TokenInformationNode* newNode = new TokenInformationNode;
		newNode->code = TokenCodes::NT;
		newNode->nonterm = type;
		childs.push_back(newNode);
		switch (type)
		{
		case Nonterminals::PROGRAM:
			newNode->id = "PROGRAM";
			break;
		case Nonterminals::STATEMENTS:
			newNode->id = "STATEMENTS";
			break;
		case Nonterminals::STATEMENT:
			newNode->id = "STATEMENT";
			break;
		case Nonterminals::EXPRESSION:
			newNode->id = "EXPRESSION";
			break;
		case Nonterminals::TERMTAIL:
			newNode->id = "TERMTAIL";
			break;
		case Nonterminals::TERM:
			newNode->id = "TERM";
			break;
		case Nonterminals::FACTORTAIL:
			newNode->id = "FACTORTAIL";
			break;
		case Nonterminals::FACTOR:
			newNode->id = "FACTOR";
			break;
		case Nonterminals::CONST:
			newNode->id = "CONST";
			break;
		case Nonterminals::IDENT:
			newNode->id = "IDENT";
			break;
		case Nonterminals::ASS_OP:
			newNode->id = "ASS_OP";
			break;
		case Nonterminals::SEMI_COL:
			newNode->id = "SEMI_COL";
			break;
		case Nonterminals::ADD_OP:
			newNode->id = "ADD_OP";
			break;
		case Nonterminals::MULT_OP:
			newNode->id = "MULT_OP";
			break;
		case Nonterminals::LEFT_PAREN:
			newNode->id = "LEFT_PAREN";
			break;
		case Nonterminals::RIGHT_PAREN:
			newNode->id = "RIGHT_PAREN";
			break;
		case Nonterminals::TERMINAL:
			break;
		default:
			newNode->id = "SOMETHING_WENT_WRONG";
			break;
		}
		return newNode;
	}

	/*input의 child에 terminal달기*/
	void addTerminal(TokenCodes type, std::string ident)
	{
		TokenInformationNode* newNode = new TokenInformationNode;
		newNode->code = type;
		newNode->nonterm = Nonterminals::TERMINAL;
		newNode->id = ident;
		childs.push_back(newNode);
		return;
	}

	void printTree(const std::string& prefix = "", bool isLast = true) const {
		// 현재 노드의 id 출력 (들여쓰기 포함)
		std::cout << prefix;

		// └─ 또는 ├─로 가지를 출력
		std::cout << (isLast ? "└─ " : "├─ ") << id << std::endl;

		// 자식 노드들에 대해 재귀적으로 printTree 호출
		for (size_t i = 0; i < childs.size(); ++i) {
			bool lastChild = (i == childs.size() - 1);
			childs[i]->printTree(prefix + (isLast ? "    " : "│   "), lastChild);
		}
	}
};