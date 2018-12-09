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

#ifndef IDP_IDP_TREAP_H
#define IDP_IDP_TREAP_H

#include "idp_depthFirstTraversal.h"

using namespace std;

struct treapNode{
    int number;
    string nodeLabel;
    int priority;
    bool actChild; //if any child is true, make it true
    bool active; //if this occurrence is the first one encountered and has non-tree edges, make it true
    bool activeOccur;
    int weight;
    treapNode *lChild;
    treapNode *rChild;
    treapNode *parent;
    treapNode(){};
    treapNode(int a, string b, bool c):number(a),nodeLabel(b),active(c)
    {priority = ((rand()<<15)+rand())%200000000; actChild = false; weight = 1; activeOccur = false;};
};

class idp_treap {
public:
    void insertNode(unordered_map<int, list<string>*> *eulerTourSequenceLists,
                    unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*> *nonTreeEdgesList);
    unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *> *getLevelLabelReferenceMap(){return levelLabelReferenceMap;};
    void printLevelLabelReferenceMap();
    void printActiveOccur();
    unordered_set<treapNode*> *getTreapList(){return treapList;};
    unordered_map<string, treapNode*> *getActiveOccur(){return activeOccur;};
    int getCountOfNode(){return countOfNode;};
    ~idp_treap();

private:
    void insert(int number, string label, treapNode *&root, bool active, bool occur, treapNode *parentNode);
    void printTreap(treapNode *treapRoot);
    void leftRotate(treapNode *&root);
    void rightRotate(treapNode *&root);

private:
    unordered_set<treapNode*> *treapList = new unordered_set<treapNode*>();
    unordered_map<string, treapNode*> *activeOccur = new unordered_map<string, treapNode*>();
    treapNode *lastVisitedPointer;
    unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *> *levelLabelReferenceMap =
            new unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>();
    int countOfNode = INT_MIN;
};


#endif //IDP_IDP_TREAP_H
