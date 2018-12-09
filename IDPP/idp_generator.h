//
// MIT License
//
//Copyright (c) 2018 Lei Liu
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//

#ifndef IDP_IDP_GENERATER_H
#define IDP_IDP_GENERATER_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <vector>
#include <stack>
#include <fstream>

using namespace std;

struct coordinate{
    int x;
    int y;
    coordinate(){};
    coordinate(int a, int b):x(a),y(b){};
};

struct inputTree{
    int label;
    int sublabel;
    inputTree *parent;
    unordered_set<inputTree*> *childList;
    list<int> *coverSpecies;
    inputTree(){label = 0; sublabel = -1; parent = nullptr; childList = new unordered_set<inputTree*>(); coverSpecies = new list<int>();};
};

struct treeCom{
    string label;
    unordered_set<treeCom*> *treeChild;
    treeCom(){label = ""; treeChild = new unordered_set<treeCom*>();};
    treeCom(string a){label = a; treeChild = new unordered_set<treeCom*>();};
};

class idp_generator {
public:
    char** generateNumberSequence(int m, int n, int N);
    char** generateNumberSequenceForIntermediateRatio(int m, int n, int N);
    char** generateNumberSequenceForHighRatio(int m, int n, int N);
    char** generateNumberSequenceForTreeCompatibilityLowDensity(int m, int n, int N);
    char** generateNumberSequenceForTreeCompatibilityHighDensity(int m, int n, int N);
    char** randomConvertE1ToEQ(int percentageRemain, char** matrix);
    char** randomConvertE1E0ToEQ(int percentageRemain, char** matrix);
    char** randomConvertE1E0ToEQ(int percentageRemain, char** matrix, int row, int col);
    char** randomConvertE0toEq(int percentageRemain, char** matrix);
    char** randomConvertE0toEq(int percentageRemain, char** matrix, int row, int col);
    char** randomConvertE0toEqBasedOnE1(int percentageRemain, char** matrix, int row, int col);
    void idpp2TreeCompatibility(char** matrix, int row, int col, int count);
    void idpp2TreeCompatibilityHighDensity(char** matrix, int row, int col, int count);
    int getColumnSize(){return columnSize;};
    int getRowSize(){return rowSize;};
    int getZeros(){return sizeOfE0;};
    void writeMatrix2file(char** seedMatrix, int row, int col);
    ~idp_generator();

private:
    char** convert2Tree(int n, int N);
    char** convert2TreeWithHighRatio(int n, int N);
    char** convert2TreeWithIntermediateRatio(int n, int N);
    char** convert2Tree4TreeCompaibilityLowDensity(int n, int N);
    char** convert2Tree4TreeCompaibilityHighDensity(int n, int N);
    string convert2newick(treeCom *tree, string newick);
    void writeInputTrees2File(string filePath, string ioStr);
    void writeMatrix2file(string filePath, char** matrix, int row, int col);


private:
    vector<int> *pruferCode = new vector<int>();
    unordered_set<int> *speciesList = new unordered_set<int>();
    vector<int> *characterList = nullptr;
    int columnSize = 0;
    int rowSize = 0;
    int sizeOfE1 = 0;
    int sizeOfE0 = 0;
};


#endif //IDP_IDP_GENERATER_H
