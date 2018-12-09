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

#ifndef IDP_IDP_DEPTHFIRSTTRAVERSAL_H
#define IDP_IDP_DEPTHFIRSTTRAVERSAL_H

#include <iostream>
#include <list>
#include <unordered_map>
#include <stack>
#include <unordered_set>
#include "idp_matrixTransform.h"

using namespace std;

//define status for DFS algorithm
#define NONCOLORED 0
#define WHITE 1
#define GRAY 2
#define BLACK 3

class idp_depthFirstTraversal {
public:
    void iterativeDepthFirstSearch(unordered_map<string, unordered_set<string>*> *adjList);
    void printEulerTourSequence();
    void printEdgeLists();
    unordered_map<int,list<string> *> *getEulerTourSequenceLists(){return eulerTourSequenceLists;};
    unordered_map<graphEdge, int, hashEdge> *getTreeEdges(){return treeEdges;};
    unordered_map<graphEdge, int, hashEdge> *getAllEdges(){return allEdge;};
    unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*> *getNonTreeEdgesList(){return nonTreeEdgesList;};
    ~idp_depthFirstTraversal();

private:
    //create the Euler Tour sequence
    unordered_map<int,list<string> *> *eulerTourSequenceLists = new unordered_map<int,list<string> *>();
    //initialize tree edge lists(map from edge to level)
    unordered_map<graphEdge, int, hashEdge> *treeEdges = new unordered_map<graphEdge, int, hashEdge>();
    //initialize non-tree edge lists(map from edge to level)
    unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*> *nonTreeEdgesList =
            new unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*>();
    //initialize edge lists
    unordered_map<graphEdge, int, hashEdge> *allEdge = new unordered_map<graphEdge, int, hashEdge>();

};


#endif //IDP_IDP_DEPTHFIRSTTRAVERSAL_H
