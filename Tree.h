#ifndef XML_HW_TREE_H
#define XML_HW_TREE_H
#include <string>
#include <vector>

using namespace std;

class Tree
{
private:
    vector<Tree> subTrees;
    string data;

public:
    Tree() {}
    Tree(vector<Tree> _subTrees, string _data) : subTrees(_subTrees), data(_data) {}

    void addTree(Tree tree);

    string getData () const {return this->data;}
    vector<Tree>& getSubTrees() {return this->subTrees;}
    static void getSubRootsWithValue(Tree tree, string value, vector<Tree>& vec);
    void addSubTrees(vector<Tree> vec);

};


#endif //XML_HW_TREE_H