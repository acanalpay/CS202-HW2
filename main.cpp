/**
 * AUTHOR: Abdullah Can Alpay
 * ID: 21702686
 * SECTION: 3
 * ASSIGNMENT: 2 
 */
#include <iostream>
#include<math.h>
#include<fstream>
#include"DecisionTree.h"
#include"DecisionTreeNode.h"
using namespace std;

int main(){
    DecisionTree ml;
    int trainFeatures = 10;
    int trainSamples = 31;
    int testSamples = 473;
    string x = "train_data_g.txt";
    string y = "test_data.txt";

    ml.train(x,trainSamples, trainFeatures);
    cout<<"TRAIN FINISH"<<endl;
    double per = ml.test(y, testSamples);
    cout << per << endl;
    cout<<"TEST FINISH"<<endl;
    ml.print();
    return 0;
}