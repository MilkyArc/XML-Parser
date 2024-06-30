#ifndef XML_HW_PARSER_H
#define XML_HW_PARSER_H
#include "Tree.h"
#include <string>


class Parser
{
private:
    Parser() {}

	// for parseXmlFile()
     bool hasQuotes(string str);
     vector<Tree> constructTreeFromLines(int tabCount, vector<string> strVec);

     unsigned short getTabCount(string line);
     unsigned short getBracketsCount(string line);

     Tree readIncompleteLine(string line, unsigned int tabCount);
     Tree readCompleteLine(string line, unsigned int tabCount);

     vector<Tree> getRelevantTrees(vector<Tree> trees, vector<string> args);
	 vector<string> pop_front(vector<string> vec);

	 // for parseUserInput()
     bool containsSlash(string input);
     bool containsSquareBrackets(string input);
     bool containsAt(string input);
     bool containsEquals(string input);

     short getCharCountInStr(string str, char ch);
     void printVec(vector<string> vec);

	 int toInt(string str);

	vector<string> slashOperator(Tree tree, vector<string> args);
	vector<string> indexedSlashOperator(Tree tree, vector<string> args, short index);
	vector<string> atOperator(Tree tree, vector<string> args, string attribute);
	vector<string> equalsOperator(Tree tree, vector<string> args, string element, string val, string wantedVal);

public:
    static Parser& getInstance(); // singleton
    Parser(const Parser& p) = delete;

	Tree parseXmlFile(string filename);
	bool parseUserInput(Tree masterTree, string input);
};


#endif //XML_HW_PARSER_H