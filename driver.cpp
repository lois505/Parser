// Date: 2021/04/17
#include "Lexical.h"
#include <iostream>

using namespace std;

int main(int argv, char* argc[])
{
	std::string input = argc[1];
	Lexical lex;
	lex.readInFile(input);
	lex.program();
	lex.printAllIDs();
	//cout << "\n\n";
	//lex.printTree();
	return 0;
}