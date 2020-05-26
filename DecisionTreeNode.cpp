/**
 * AUTHOR: Abdullah Can Alpay
 * ID: 21702686
 * SECTION: 3
 * ASSIGNMENT: 2 
 */
#include<iostream>
#include"DecisionTreeNode.h"
using namespace std;

DecisionTreeNode::DecisionTreeNode(){
    value = 0;
    left = NULL;
    right = NULL;
}

DecisionTreeNode::~DecisionTreeNode(){

}
bool DecisionTreeNode::nodeDecide(){
    if(value < 0) // It's a class because value is negative
        return true;
    
    return false;
}