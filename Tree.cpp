#include "Tree.h"

void Tree::addTree(Tree tree)
{
    this->subTrees.push_back(tree);
}

void Tree::getSubRootsWithValue(Tree tree, string value, vector<Tree>& vec)
{
    for (int i = 0; i < tree.getSubTrees().size(); i++)
    {
        if (tree.getSubTrees()[i].getData() == value)
        {
            vec.push_back(tree.getSubTrees()[i]);
        }
        else
        {
            getSubRootsWithValue(tree.getSubTrees()[i], value, vec);
        }

    }
}

void Tree::addSubTrees(vector<Tree> vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        addTree(vec[i]);
    }
}
