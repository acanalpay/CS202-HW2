/**
 * AUTHOR: Abdullah Can Alpay
 * ID: 21702686
 * SECTION: 3
 * ASSIGNMENT: 2 
 */
#include<iostream>
#include"DecisionTree.h"
#include"DecisionTreeNode.h"
#include<math.h>
#include<fstream>
#include<string>
using namespace std;
 
DecisionTree::DecisionTree(){
   root = new DecisionTreeNode;
}
void DecisionTree::deleteTree(DecisionTreeNode*& node){
   if(node == NULL)
       return;
  
   if(node->left != NULL)
       deleteTree(node->left);
   if(node->right != NULL)
       deleteTree(node->right);
   delete node;
}
DecisionTree::~DecisionTree(){
    deleteTree(root);
}

double calculateEntropy(const int* classCounts, const int numClasses){
   int sum = 0;
   double ans = 0;
 
   for (int i = 0; i < numClasses; i++)
       sum  += classCounts[i];
    for(int i = 0; i < numClasses; i++){
       if(classCounts[i] > 0){
           double c = double(classCounts[i]) / double(sum);
           c = log2(c);
           ans += (double(classCounts[i]) / double(sum) ) *  c;
       }
   }
   ans *= -1; // from the formula
  
   return ans;
}
 
double calculateInformationGain(const bool ** data, const int* labels, const int numSamples,
const int numFeatures, const bool* usedSamples, const int featureId){
    double hParent = 0;
    double hSplit = 0;
    double hRight = 0;
    double hLeft = 0;

    int* classCounts = new int[numSamples];

    for(int i = 0; i < numSamples; i++)
        classCounts[i] = 0;
    
    for(int i = 0; i < numSamples; i++)
        if(usedSamples[i])
            classCounts[labels[i] - 1]++;
    
    hParent = calculateEntropy(classCounts, numSamples);

    delete[] classCounts;
    // H(P) finished

    // H(S)
    int* leftChild = new int[numSamples];
    int* rightChild = new int[numSamples];

    int rightCount = 0;
    int leftCount = 0;

    for(int i = 0; i < numSamples; i++){
        rightChild[i] = 0;
        leftChild[i] = 0;
    }

    for(int i = 0; i < numSamples; i++){
        if(usedSamples[i]){
            if(data[i][featureId]){
                rightChild[labels[i] - 1]++;
                rightCount++;
            }
            else{
                leftChild[labels[i] - 1]++;
                leftCount++;
            }
        }
    }

    hRight = calculateEntropy(rightChild, numSamples);
    hLeft = calculateEntropy(leftChild, numSamples);

    delete[] rightChild;
    delete[] leftChild;

    double pRight = (rightCount)/(double)(rightCount + leftCount);
    double pLeft = (double)leftCount/(double)(rightCount + leftCount);

    hSplit = pRight * hRight + pLeft * hLeft;

    return hParent - hSplit;
}
 
void trainTree(const bool** data, const int* labels,
const int numSamples, const int numFeatures, int current, bool* used,
bool* featureUse, DecisionTreeNode*& node){
    //BASE STEPS
    if(current <= 0){
        // No Available Feature Left
        // Maximum Repeated label will determined the class
 
        int* maximum = new int[numSamples]; // Maximum numSamples different class can be existed
        int curMax = -1;
        int max;
        for(int i = 0; i < numSamples; i++){
            if(used[i]){
                maximum[labels[i] - 1]++;
                if(maximum[labels[i] - 1] > curMax){
                    curMax = maximum[labels[i] - 1];
                    max = labels[i];
                }
            }
        }
 
        node->value = -1 * max;
        delete [] maximum;
        return;
        // END OF THE FIRST BASE STEP
    }
 
    // SECOND BASE STEP
    // All the labels are same
    
    bool same = false;
    int prev = -1;
    for(int i = 0; i < numSamples; i++){
        if(used[i]){
            if(prev == -1)
                prev = labels[i];
          
            if(prev == labels[i])
                same = true;
            else{
                same = false;
                break;
            }
        }
    }
 
    if(same){
        // prev posses the labels id
  
        node->value = -1 * prev;
        return;
        // END OF THE SECOND BASE STEP
    }
    // END OF THE BASE STEPS

    // Find maximum Information Gain for available feature
  
    double maxIG = -1;
    int featureID = -1;
    for(int i = 0; i < numFeatures; i++){
        if(!featureUse[i]){
            double curIG = calculateInformationGain(data, labels, numSamples, numFeatures, used, i);
            if(maxIG < 0 || curIG > maxIG){
                maxIG = curIG;
                featureID = i;
            }
        }
    }

    featureUse[featureID] = true;
    node->value = featureID;
 
    // Always split to two child
    // Let's find the split of the data respect to featureID
  
    bool* rightChild = new bool[numSamples];
    bool* leftChild = new bool[numSamples];
  
    int rightCount = 0;
    int leftCount = 0;

    for(int i = 0; i < numSamples; i++){
        rightChild[i] = false;
        leftChild[i] = false;
        if(used[i]){
            if(data[i][featureID]){
                // data's feature is 1 => right child
                rightChild[i] = true;
                rightCount++;
            }
            else{
                // data's feature is 0 => left child
                leftChild[i] = true;
                leftCount++;
            }
        }
    }
    if(leftCount == 0 || rightCount == 0){
        int* maximum = new int[numSamples]; // Maximum numSamples different class can be existed
        int curMax = -1;
        int max;
        for(int i = 0; i < numSamples; i++){
            if(used[i]){
                maximum[labels[i] - 1]++;
                if(maximum[labels[i] - 1] > curMax){
                    curMax = maximum[labels[i] - 1];
                    max = labels[i];
                }
            }
        }
 
        node->value = -1 * max;
        delete [] rightChild;
        delete [] leftChild;
        delete [] maximum;
        return;
    }
    // Let's create the nodes and call the function recursiely
    node->right = new DecisionTreeNode;
    node->left = new DecisionTreeNode;
  
    trainTree (data, labels, numSamples, numFeatures, current - 1, rightChild, featureUse, node->right);
    trainTree (data, labels, numSamples, numFeatures, current - 1, leftChild, featureUse, node->left);
  
    // Delete the arrays
    delete [] rightChild;
    delete [] leftChild;
}
 
void DecisionTree::train(const bool** data, const int* labels,
const int numSamples, const int numFeatures){
   bool* used = new bool[numSamples];
   bool* featureUse = new bool[numFeatures];
 
   for(int i = 0; i < numSamples; i++)
       used[i] = true;
 
   for(int i = 0; i < numFeatures; i++)
       featureUse[i] = false;
  
   trainTree((const bool**)data,(const int*)labels, numSamples, numFeatures, numFeatures, used, featureUse, root);
  
   delete [] used;
   delete [] featureUse;   
}
 
void DecisionTree::train(const string fileName, const int numSamples, const int numFeatures){
   ifstream myFile;
   myFile.open(fileName);
 
   bool** data = new bool*[numSamples];
   for(int i = 0; i < numSamples; i++)
       data[i] = new bool[numFeatures];
  
   int* labels = new int[numSamples];
 
   int x;
   int i = 0;
   int j = 0;
   while(i < numSamples){
       myFile>>x;
       if(j == numFeatures){
           labels[i++] = x;
           j = 0;
       }
       else{
           data[i][j] = x;
           j++;
       }
  
   }
  
   myFile.close();
  
   train((const bool**)data, labels, numSamples, numFeatures);
 
   delete [] labels;
   for(int i = 0 ; i < numSamples; i++)
       delete [] data[i];
   delete [] data;
}
 
int DecisionTree::predict(const bool* data){
   DecisionTreeNode* cur = root;
 
   // cout<<"PREDICT ENTER"<<endl;
   while(cur->value >= 0){
 
       if(data[cur->value])
           cur = cur->right;
       else
           cur = cur->left;
       // cout<<"PREDICT CONT"<<endl;
   }
   // cout<<"RETURN PREDICT"<<endl;
   return -1 * cur->value;
}
 
double DecisionTree::test(const bool** data, const int* labels, const int numSamples){
   int trueSoFar = 0;
  
   for(int i = 0; i < numSamples; i++){
 
       int x = predict((const bool*)data[i]);
       if(x == labels[i])
           trueSoFar++;
       else{
        //    cout<<i<<'\t'<<x<<'\t'<<labels[i]<<endl;
       }
   }
  
   double percentage = (double)trueSoFar / (double)numSamples;
  
   return percentage * 100;
}
 
double DecisionTree::test(const string fileName, const int numSamples){
    fstream myFile;
 
    myFile.open(fileName);
    string line;

    getline(myFile, line);
  
    bool** data = new bool*[numSamples];
 
    int* labels = new int[numSamples];

    int numFeatures = line.length() - 1;

    while(line[numFeatures] != ' ')
        numFeatures--;
    
    for(int i = 0; i < numSamples; i++)
        data[i] = new bool [numFeatures / 2];
    
    int x = 0;
    while(x < numSamples){
        for(int i = 0; i < numFeatures; i+= 2){
            if(line[i] == '1')
                data[x][i / 2] = true;
            else if(line[i] == '0')
                data[x][i / 2] = false;
        }
        int decimal = 1;
        int label = 0;
        
        for(int i = line.length() - 2; i > numFeatures; i--){
            label += (line[i] - '0') * decimal;
            // cout<<line[i]<<endl;
            decimal *= 10;
        }
        labels[x] = label;
        x++;
        getline(myFile, line);
    }

    myFile.close();
//    // cout<<"TEST READ SUCCESS!"<<endl;
    double answer = test((const bool**)data, labels, numSamples);
 
    for(int i = 0; i < numSamples; i++)
        delete [] data[i];
    delete [] data;
    delete [] labels;

    return answer;
}
// send root and 0. Preorder => Parent - Left - Right
void DecisionTree::print(DecisionTreeNode*& node, int tabs){
   if(node == NULL)
       return;
   if(node->nodeDecide()){//It's a class so no leaf
       for(int i = 0 ; i < tabs; i++)
           cout<<"\t";
       cout<< "class="<<(-1 * node->value)<<endl;
       return;
   }
   //Node is a split node, i.e. node is a parent for some nodes
   for(int i = 0; i < tabs; i++)
       cout<<"\t";
   cout<< node->value << endl;
 
   tabs++;
   // First call the left subtree
    print(node->left, tabs);
 
    // Then call the right subtree
    print(node->right, tabs);
}
 
void DecisionTree::print(){
    print(root, 0);
}

