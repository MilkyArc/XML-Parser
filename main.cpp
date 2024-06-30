#include <iostream>
#include "Parser.h"

const string FILE_NAME = "test_multiple.txt";

int main()
{
    Tree tree = Parser::getInstance().parseXmlFile(FILE_NAME);

	char text[100]; // needed for cin.getline()
    string input;
	bool terminate = false;
    while (!terminate)
    {
		cout << ">>>";
		if (tree.getSubTrees().size() == 0) return 0; // Parsing error or empty file
        cin.getline(text, 200);
		input = text;
		//cout << input;
        terminate = Parser::getInstance().parseUserInput(tree, input);
    }

    return 0;
}


