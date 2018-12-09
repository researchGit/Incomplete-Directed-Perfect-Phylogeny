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

#ifndef IDP_IDP_HDT_H
#define IDP_IDP_HDT_H

#include "idp_treap.h"
#include <math.h>
#include <vector>

using namespace std;

struct cutTreaps{
    treapNode* treap1;
    treapNode* treap2;
    cutTreaps(){};
    cutTreaps(treapNode *t1, treapNode *t2):treap1(t1),treap2(t2){};
};

class idp_hdt {
public:
    idp_hdt(){};
    idp_hdt(int numberOfNode, unordered_set<treapNode*> *treapList,
            unordered_map<string, treapNode*> *activeOccurList, bool enablePromotion,
            unordered_map<graphEdge, int, hashEdge> *allEdge,
            unordered_map<graphEdge, int, hashEdge> *treeEdgeList,
            unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*> *nonTreeEdgeList,
            unordered_map<string, unordered_set<string>*> *adjacenctList,
            unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *> *levelLabelReference,
            int countOfNode);
    void hdtDeleteEdge(string u, string v);
    void adjacencyListDeleteEdge(string u, string v);
    void hdtSplit(string u, string v, int edgeLV);
    unordered_map<int, unordered_set<treapNode*> *> *getLevelTreapMap(){return levelTreapMap;};
    ~idp_hdt();

private:
    treapNode *hdtFindRoot(treapNode *occur);
    void hdtCutTreap(string u, string v, treapNode *&treap, int edgeLV);
    bool isFirstOccurSmallerThanSecondOccur(treapNode *occurA, treapNode *occurB);
    int computeLengthOfPathToRoot(treapNode *currentNode);
    vector<treapNode*> *computePathToRoot(int pathLength, treapNode *currentNode);
    void hdtCutTreapAtRightPosition(treapNode *&currentTreap, treapNode *&cutPosition, treapNode *&treapPiece1, treapNode *&treapPiece2);
    void hdtCutTreapAtLeftPosition(treapNode *&currentTreap, treapNode *&cutPosition, treapNode *&treapPiece1, treapNode *&treapPiece2);
    void leftRotate(treapNode *&root);
    void rightRotate(treapNode *&root);
    void printTreap(treapNode *treapRoot);
    treapNode* findLeftmostNode(treapNode* tree);
    treapNode* findRightmostNode(treapNode* tree);
    treapNode* reLinkTreaps(treapNode* treap1, treapNode* treap2, int edgeLV);
    void promoteTreeEdges(int edgeLV, cutTreaps *currentCutTreaps);
    void rerooting(string u, treapNode *&tree, int edgeLV);
    graphEdge findReplacementEdge(treapNode *smallTreap, treapNode *largeTreap, int edgeLV);


private:
    int logN = 0;
    int nodeCount = 0;
    unordered_map<int, unordered_set<treapNode*> *> *levelTreapMap = new unordered_map<int, unordered_set<treapNode*> *>();
    unordered_map<int, unordered_map<string, treapNode*> *> *levelActiveOccurMap = new unordered_map<int, unordered_map<string, treapNode*> *>();
    bool promotionSwitch = false;
    unordered_map<graphEdge, int, hashEdge> *edgeList = new unordered_map<graphEdge, int, hashEdge>();
    unordered_map<graphEdge, int, hashEdge> *treeEdge = new unordered_map<graphEdge, int, hashEdge>();
    unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*> *nonTreeEdge = new unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*>();
    unordered_map<string, unordered_set<string>*> *adjacencyList = new unordered_map<string, unordered_set<string>*>();
    unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *> *labelReference = new
            unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>();

    unordered_map<int, cutTreaps*> *treapVector = new unordered_map<int, cutTreaps*>();
    treapNode* treapU = nullptr; //treap contains node u
    treapNode* treapV = nullptr; //treap contains node v
};


#endif //IDP_IDP_HDT_H
