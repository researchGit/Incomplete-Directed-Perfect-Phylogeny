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

#ifndef IDP_IDP_A_FAST_H
#define IDP_IDP_A_FAST_H

#include "idp_matrixTransform.h"
#include <fstream>

using namespace std;

class idp_A_fast {
public:
    idp_A_fast(){};
    idp_A_fast(unordered_map<int, fancyKStructure*> *fancyKList,
               unordered_set<graphEdge, hashEdge> *E0,
               unordered_set<graphEdge, hashEdge> *E1,
               unordered_set<graphEdge, hashEdge> *Eq);
    void idpProcessing(char** matrix, int columnSize, int rowSize, bool enablePromotion);
    void write2file(int fileCount);
    string convert2newick(outputTree *tree, string newick);
    ~idp_A_fast();

private:
    void printDcOne();
    void printDcQuestion();
    void printFancyKLists(unordered_map<int, fancyKStructure*> *fancyKList);
    void printOutputTree();

private:
    unordered_map<int, fancyKStructure*> *fancyK = new unordered_map<int, fancyKStructure*>();
    unordered_set<graphEdge, hashEdge> *eZero = new unordered_set<graphEdge, hashEdge>();
    unordered_set<graphEdge, hashEdge> *eOne = new unordered_set<graphEdge, hashEdge>();
    unordered_set<graphEdge, hashEdge> *eQuestion = new unordered_set<graphEdge, hashEdge>();
    int t = 0;

    unordered_map<string, int> *dcOne = new unordered_map<string, int>();
    unordered_map<string, int> *dcQuestion = new unordered_map<string, int>();

    outputTree *outputRoot = new outputTree();
    unordered_set<outputTree*> *outputSet = new unordered_set<outputTree*>();

    unordered_map<string, outputTree*> *labelToNode = new unordered_map<string, outputTree*>();
};


#endif //IDP_IDP_A_FAST_H
