/**
 * AUTHOR: Abdullah Can Alpay
 * ID: 21702686
 * SECTION: 3
 * ASSIGNMENT: 2 
 */
#ifndef __DECISION_TREE_NODE
#define __DECISION_TREE_NODE
#include<iostream>
using namespace std;

class DecisionTreeNode{
    public:
        DecisionTreeNode();
        ~DecisionTreeNode();
        bool nodeDecide();

        int value; // If value is negative that means it's leaf otherwise it posses the featureID
        DecisionTreeNode* left;
        DecisionTreeNode* right;

};

#endif