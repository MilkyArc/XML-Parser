#include "Parser.h"
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

int backTrackIndex = 0;

Tree Parser::parseXmlFile(string filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error opening file!" << endl;
        return Tree();
    }

    Tree masterTree({}, "Root");
    vector<string> fileLines;

    while (!file.eof())
    {
        char buffer[100];
        file.getline(buffer, 100);
        fileLines.push_back(buffer);
    }

    masterTree.getSubTrees() = constructTreeFromLines(0, fileLines);

    file.close();

    cout << "File " << filename << " parsed successfully!" << endl << "You can now type your XPath queries or type 'exit' to quit the program..." << endl;

    return masterTree;
}

vector<Tree> Parser::constructTreeFromLines(int tabCount, vector<string> strVec)
{
    unsigned short innerTabCount;
    vector<Tree> trees;
    Tree lastTree({}, "");

    while (backTrackIndex < strVec.size())
    {
        innerTabCount = getTabCount(strVec[backTrackIndex]);
		backTrackIndex++;

        string buff = strVec[backTrackIndex - 1];

        if (innerTabCount == tabCount)
        {
            if (getBracketsCount(strVec[backTrackIndex - 1]) == 1)
                lastTree = readIncompleteLine(strVec[backTrackIndex - 1], innerTabCount);
            else
                lastTree = readCompleteLine(strVec[backTrackIndex - 1], innerTabCount);

            trees.push_back(lastTree);
        }
        if (innerTabCount < tabCount)
        {
            return trees;
        }
        else if (innerTabCount > tabCount)
        {
			backTrackIndex--;
            lastTree.addSubTrees(constructTreeFromLines(innerTabCount,strVec));
            trees.pop_back();
            trees.push_back(lastTree);
        }
    }
    return trees;
}

unsigned short Parser::getTabCount(string line)
{
    unsigned short tabCount = 0;
    int i = 0;

    while (line[i] == '\t')
    {
        i++;
        tabCount++;
    }
    return tabCount;
}

unsigned short Parser::getBracketsCount(string line)
{
    unsigned short count = 0;

    for (int i = 0; i < line.length(); i++)
    {
        if (line[i] == '<') count++;
    }
    return count;
}

Tree Parser::readIncompleteLine(string line, unsigned int tabCount)
{
    int offset = tabCount + 1;

    if (hasQuotes(line))
    {
        string cat;

        for (int i = offset; i < line.length(); i++)
        {
            if (line[i] == ' ') break;
            cat += line[i];
        }

        offset += cat.length() + 1;
        string attribute;

        for (int i = offset; i < line.length(); i++)
        {
            if (line[i] == '=') break;
            attribute += line[i];
        }

        offset += attribute.length() + 2;
        string attributeValue;

        for (int i = offset; i < line.length(); i++)
        {
            if (line[i] == '"') break;
            attributeValue += line[i];
        }

        Tree attributeTree({ Tree({}, attributeValue) }, attribute);
        return Tree({attributeTree}, cat);
    }
    else
    {
        string cat;

        for (int i = offset; i < line.length(); i++)
        {
            if (line[i] == '>') break;
            cat += line[i];
        }
        return Tree({}, cat);
    }
}

Tree Parser::readCompleteLine(string line, unsigned int tabCount)
{
    int offset = tabCount + 1;

    if (hasQuotes(line))
    {
        string cat;

        for (int i = offset; i < line.length(); i++)
        {
            if (line[i] == ' ') break;
            cat += line[i];
        }

        offset += cat.length() + 1;
        string attribute;

        for (int i = offset; i < line.length(); i++)
        {
            if (line[i] == '=') break;
            attribute += line[i];
        }

        offset += attribute.length() + 2;
        string attributeValue;

        for (int i = offset; i < line.length(); i++)
        {
            if (line[i] == '"') break;
            attributeValue += line[i];
        }

        offset += attributeValue.length() + 2;

        string catValue;

        for (int i = offset; i < line.length(); i++)
        {
            if (line[i] == '<') break;
            catValue += line[i];
        }

        Tree attributeTree({ Tree({}, attributeValue) }, attribute);
        return Tree({ Tree({}, catValue), attributeTree }, cat);
    }
    else
    {
        string cat;

        for (int i = offset; i < line.length(); i++)
        {
            if (line[i] == '>') break;
            cat += line[i];
        }

        offset += cat.length() + 1;

        string catValue;

        for (int i = offset; i < line.length(); i++)
        {
            if (line[i] == '<') break;
            catValue += line[i];
        }

        return Tree({ Tree({}, catValue) }, cat);
    }
}

bool Parser::hasQuotes(string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == '"') return true;
    }
    return false;
}

Parser& Parser::getInstance()
{
         static Parser parser;
       	 return parser;
}

vector<string> Parser::pop_front(vector<string> vec)
{
    vector<string> vecResult;

    if(vec.size() < 1) return vec;

    for (int i = 1; i < vec.size(); i++)
    {
        vecResult.push_back(vec[i]);
    }
    return vecResult;
}

vector<Tree> Parser::getRelevantTrees(vector<Tree> trees, vector<string> args)
{
    vector<Tree> treesResult;

    if(args.size() == 1) return trees;

    for (int i = 0; i < trees.size(); i++)
    {
        for (int j = 0; j < trees[i].getSubTrees().size(); ++j)
        {
            if(trees[i].getSubTrees()[j].getData() == args[1])
            {
                treesResult.push_back(trees[i].getSubTrees()[j]);
            }
        }
    }

    return getRelevantTrees(treesResult, pop_front(args));
}

vector<string> Parser::slashOperator(Tree tree, vector<string> args)
{
    vector<string> vec;
    vector<Tree> trees = getRelevantTrees({tree}, args);

    for (int i = 0; i < trees.size(); i++)
    {
        vec.push_back(trees[i].getSubTrees()[0].getData());
    }
    
    return vec;
}

vector<string> Parser::indexedSlashOperator(Tree tree, vector<string> args, short index)
{
    string lastArg = args[args.size() - 1];
    args.pop_back();
    vector<Tree> trees = getRelevantTrees({tree}, args);
    vector<string> vec;
    vector<vector<string>> orderedVec;

    for (int i = 0; i < trees.size(); i++)
    {
        vector<string> innerVec;
        for (int j = 0; j < trees[i].getSubTrees().size(); ++j)
        {
            if(trees[i].getSubTrees()[j].getData() == lastArg)
            {
                innerVec.push_back(trees[i].getSubTrees()[j].getSubTrees()[0].getData());

            }
        }
        orderedVec.push_back(innerVec);
    }

    for (int i = 0; i < orderedVec.size(); i++)
    {
        if(orderedVec[i].size() > index)
        vec.push_back(orderedVec[i][index]);
    }

    return vec;
}

vector<string> Parser::atOperator(Tree tree, vector<string> args, string attribute)
{
    vector<string> vec;
    vector<Tree> trees = getRelevantTrees({tree}, args);

    for (int i = 0; i < trees.size(); i++)
    {
        for (int j = 0; j < trees[i].getSubTrees().size(); ++j)
        {
            if(trees[i].getSubTrees()[j].getData() == attribute)
                vec.push_back(trees[i].getSubTrees()[j].getSubTrees()[0].getData());
        }
    }

    return vec;
}

vector<string> Parser::equalsOperator(Tree tree, vector<string> args, string element, string val, string wantedVal)
{
    vector<string> vec;
    vector<Tree> trees = getRelevantTrees({tree}, args);

    for (int i = 0; i < trees.size(); i++)
    {
        bool isCondMet = false;

        for (int j = 0; j < trees[i].getSubTrees().size(); j++)
        {
            if(trees[i].getSubTrees()[j].getData() == element)
            {
                if(trees[i].getSubTrees()[j].getSubTrees()[0].getData() == val)
                {
                    isCondMet = true;
                    break;
                }
            }

        }

        if(isCondMet)
        {
            for (int j = 0; j < trees[i].getSubTrees().size(); j++)
            {
                if(trees[i].getSubTrees()[j].getData() == wantedVal)
                {
                    vec.push_back(trees[i].getSubTrees()[j].getSubTrees()[0].getData());
                }
            }
        }
    }

    return vec;
}

bool Parser::parseUserInput(Tree masterTree, string input)
{
    if (containsSlash(input) && !containsSquareBrackets(input) && !containsAt(input) && !containsEquals(input))
    {
        // slash opeartor: person/address

        vector<string> args;
        args.push_back("root"); // always start from root

        short depth = getCharCountInStr(input, '/') + 1;

        short lastIndex = 0;
        for (int i = 0; i < depth; i++)
        {
            string arg;
            for (int j = lastIndex; input[j] != '/' && input[j] != '\0'; j++)
            {
                arg += input[j];
            }
            lastIndex += arg.size() + 1; // skip the slash (+1) and start from where we left off in the string
            args.push_back(arg);
        }

        printVec(slashOperator(masterTree, args));
    }

    else if (containsSlash(input) && containsSquareBrackets(input) && !containsAt(input) && !containsEquals(input))
    {
        // indexed slash operator: person/address[0]

		vector<string> args;
		args.push_back("root"); // always start from root

		short depth = getCharCountInStr(input, '/') + 1;

		short lastIndex = 0;
		for (int i = 0; i < depth; i++)
		{
			string arg;
			for (int j = lastIndex; input[j] != '/' && input[j] != '['; j++)
			{
				arg += input[j];
			}
			lastIndex += arg.size() + 1; // skip the slash (+1) and start from where we left off in the string
			args.push_back(arg);
		}

		string index;
		for (int i = lastIndex; input[i] != ']'; i++)
		{
			index += input[i];
		}


		printVec(indexedSlashOperator(masterTree, args, toInt(index)));
    }

    else if (containsSquareBrackets(input) && containsAt(input) && !containsEquals(input))
    {
        // at operator: person[@id]

		vector<string> args;
		args.push_back("root"); // always start from root

		short depth = getCharCountInStr(input, '/') + 1;

		short lastIndex = 0;
		for (int i = 0; i < depth; i++)
		{
			string arg;
			for (int j = lastIndex; input[j] != '/' && input[j] != '['; j++)
			{
				arg += input[j];
			}
			lastIndex += arg.size() + 1; // skip the slash (+1) and start from where we left off in the string
			args.push_back(arg);
		}

		string attribute;
		for (int i = lastIndex + 1; input[i] != ']'; i++) // +1 for skipping the @
		{
			attribute += input[i];
		}

		printVec(atOperator(masterTree, args, attribute));
    }

    else if (containsSlash(input) && containsSquareBrackets(input) && !containsAt(input) && containsEquals(input))
    {
        // equals operator: person[address="Bulgaria"]/name

		//first get args
		vector<string> args;
		args.push_back("root"); // always start from root

		short depth = getCharCountInStr(input, '/'); // no +1 here because we have one extra / at the end

		short lastIndex = 0;
		for (int i = 0; i < depth; i++)
		{
			string arg;
			for (int j = lastIndex; input[j] != '/' && input[j] != '['; j++)
			{
				arg += input[j];
			}
			lastIndex += arg.size() + 1; // skip the slash (+1) and start from where we left off in the string
			args.push_back(arg);
		}

		//then get element (child element)

		string element;
		for (int i = lastIndex; input[i] != '='; i++)
		{
			element += input[i];
		}
		lastIndex += element.size();

		//then get the value of the element
		string val;
		for (int i = lastIndex + 2; input[i] != '"'; i++) // +2 to skip "" and start from first char of val
		{
			val += input[i];
		}
		lastIndex += val.size();

		//finally get the wanted value; aka the child element to be returned
		string wantedVal;
		for (int i = lastIndex + 5; input[i] != '\0'; i++) // +5 to skip "]/ and start from first char of wantedVal
		{
			wantedVal += input[i];
		}

		printVec(equalsOperator(masterTree, args, element, val, wantedVal));
    }

	else if (input == "exit") return true;

    else cout << "Unrecognized XPath query Type." << endl;

}

bool Parser::containsSlash(string input)
{
    if (input.find('/') != input.npos) return true;
   else return false;
}

bool Parser::containsSquareBrackets(string input)
{
    if (input.find('[') != input.npos && input.find(']') != input.npos) return true;
    else return false;
}

bool Parser::containsAt(string input)
{
    if (input.find('@') != input.npos) return true;
    else return false;
}

bool Parser::containsEquals(string input)
{
    if (input.find('=') != input.npos) return true;
    else return false;
}

short Parser::getCharCountInStr(string str, char ch)
{
    short counter = 0;

    for (char c:str)
    {
        if (c == ch) counter++;
    }

    return counter;
}

void Parser::printVec(vector<string> vec)
{
    for (string str:vec)
    {
        cout << str << endl;
    }
}

int Parser::toInt(string str)
{
	int number = 0;
	for (int i = 0; i < str.length(); i++)
	{
		number += (int)(str[i] - '0') * pow(10, str.length() - i - 1);
	}

	return number;
}
