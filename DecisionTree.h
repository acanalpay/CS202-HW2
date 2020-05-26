/**
 * AUTHOR: Abdullah Can Alpay
 * ID: 21702686
 * SECTION: 3
 * ASSIGNMENT: 2 
 */
#ifndef __DECISION_TREE
#define __DECISION_TREE
#include<iostream>
#include"DecisionTreeNode.h"
using namespace std;

class DecisionTree{
    public:
        DecisionTree();
        ~DecisionTree();
        
        void train(const bool**, const int*, const int, const int);

        void train(const string, const int, const int);

        int predict(const bool*);

        double test(const bool**, const int*, const int);

        double test(const string, const int);

        void print();
    private:
        DecisionTreeNode* root;
        // void trainTree(const bool**, const int*, const int, const int, const int, const bool*, bool*, DecisionTreeNode*&);
        void deleteTree(DecisionTreeNode*& node);
        void print(DecisionTreeNode*&, int);
};
#endif