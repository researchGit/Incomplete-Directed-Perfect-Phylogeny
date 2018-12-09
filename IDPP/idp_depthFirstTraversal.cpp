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

#include "idp_depthFirstTraversal.h"

using namespace std;

//iterative depth first search running on adjacency list
//compute the lists of euler tour sequences
//note that there might be more than 1 connected components being taken as inputs
void idp_depthFirstTraversal::iterativeDepthFirstSearch(unordered_map<string, unordered_set<string> *> *adjList) {
    unordered_map<string, bool> *visitMap = new unordered_map<string, bool>();
    unordered_set<string> *elementSet = new unordered_set<string>();
    unordered_map<string, string> *fromMap = new unordered_map<string, string>();

    for(unordered_map<string, unordered_set<string>*>::iterator adjIter = adjList->begin(); adjIter != adjList->end();
            adjIter++){
        visitMap->insert(unordered_map<string, bool>::value_type(adjIter->first, false));
        elementSet->insert(adjIter->first);
    }

    //initialize a stack
    stack<string> *dfsStack = new stack<string>();

    int counter = 0;
    while(!elementSet->empty()){
        list<string> *eulerTourSeq = new list<string>();
        dfsStack->push(*(elementSet->begin()));
        while(!dfsStack->empty()){
            string tmpStr = dfsStack->top();
            dfsStack->pop();

            unordered_map<string, bool>::iterator visIter = visitMap->find(tmpStr);
            if(visIter != visitMap->end()){
                visIter->second = true;
                eulerTourSeq->push_back(visIter->first);
                unordered_set<string>::iterator eleIter = elementSet->find(visIter->first);
                if(eleIter != elementSet->end()){
                    elementSet->erase(eleIter);
                }

                bool all_empty = false;
                unordered_map<string, unordered_set<string>*>::iterator adjIter = adjList->find(tmpStr);
                if(adjIter != adjList->end()){
                    for(unordered_set<string>::iterator listIter = adjIter->second->begin(); listIter != adjIter->second->end();
                            listIter++){
                        visIter = visitMap->find(*listIter);
                        if(visIter != visitMap->end()){
                            if(!visIter->second){
                                all_empty = true;
                                dfsStack->push(*listIter);
                                fromMap->insert(unordered_map<string, string>::value_type(*listIter, tmpStr));

                                //put this edge in the tree edge list
                                graphEdge edge("","");
                                if(*listIter < tmpStr){
                                    edge.endPointer1 = *listIter;
                                    edge.endPointer2 = tmpStr;
                                }
                                else{
                                    edge.endPointer1 = tmpStr;
                                    edge.endPointer2 = *listIter;
                                }
                                treeEdges->insert(unordered_map<graphEdge, int, hashEdge>::value_type(edge,0));

                                allEdge->insert(unordered_map<graphEdge,int,hashEdge>::value_type(edge,0));

                                break;
                            }
                            else{
                                //put this edge in the non-tree edge list
                                graphEdge edge("","");
                                if(*listIter < tmpStr){
                                    edge.endPointer1 = *listIter;
                                    edge.endPointer2 = tmpStr;
                                }
                                else{
                                    edge.endPointer1 = tmpStr;
                                    edge.endPointer2 = *listIter;
                                }
                                unordered_map<graphEdge,int,hashEdge>::iterator tEdgeIter = treeEdges->find(edge);
                                if(tEdgeIter == treeEdges->end()){
                                    //push into list
                                    allEdge->insert(unordered_map<graphEdge,int,hashEdge>::value_type(edge,0));

                                    unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*>::iterator edgeIter =
                                            nonTreeEdgesList->find(edge.endPointer1);
                                    if(edgeIter != nonTreeEdgesList->end()){
                                        unordered_map<int, unordered_set<graphEdge, hashEdge>*>::iterator levelIter = edgeIter->second->find(0);
                                        if(levelIter != edgeIter->second->end()){
                                            levelIter->second->insert(edge);
                                        }
                                        else{
                                            unordered_set<graphEdge, hashEdge> *tempSet = new unordered_set<graphEdge,hashEdge>();
                                            tempSet->insert(edge);
                                            edgeIter->second->insert(unordered_map<int, unordered_set<graphEdge,hashEdge>*>::value_type(0,tempSet));
                                        }
                                    }
                                    else{
                                        unordered_set<graphEdge, hashEdge> *tempSet = new unordered_set<graphEdge,hashEdge>();
                                        tempSet->insert(edge);
                                        unordered_map<int, unordered_set<graphEdge,hashEdge>*> *tempMap = new unordered_map<int, unordered_set<graphEdge,hashEdge>*>();
                                        tempMap->insert(unordered_map<int, unordered_set<graphEdge,hashEdge>*>::value_type(0,tempSet));
                                        nonTreeEdgesList->insert(unordered_map<string, unordered_map<int, unordered_set<graphEdge,hashEdge>*>*>::value_type(edge.endPointer1,tempMap));
                                    }

                                    edgeIter = nonTreeEdgesList->find(edge.endPointer2);
                                    if(edgeIter != nonTreeEdgesList->end()){
                                        unordered_map<int, unordered_set<graphEdge, hashEdge>*>::iterator levelIter = edgeIter->second->find(0);
                                        if(levelIter != edgeIter->second->end()){
                                            levelIter->second->insert(edge);
                                        }
                                        else{
                                            unordered_set<graphEdge, hashEdge> *tempSet = new unordered_set<graphEdge,hashEdge>();
                                            tempSet->insert(edge);
                                            edgeIter->second->insert(unordered_map<int, unordered_set<graphEdge,hashEdge>*>::value_type(0,tempSet));
                                        }
                                    }
                                    else{
                                        unordered_set<graphEdge, hashEdge> *tempSet = new unordered_set<graphEdge,hashEdge>();
                                        tempSet->insert(edge);
                                        unordered_map<int, unordered_set<graphEdge,hashEdge>*> *tempMap = new unordered_map<int, unordered_set<graphEdge,hashEdge>*>();
                                        tempMap->insert(unordered_map<int, unordered_set<graphEdge,hashEdge>*>::value_type(0,tempSet));
                                        nonTreeEdgesList->insert(unordered_map<string, unordered_map<int, unordered_set<graphEdge,hashEdge>*>*>::value_type(edge.endPointer2,tempMap));
                                    }
                                }
                            }
                        }
                    }
                }

                if(!all_empty){
                    unordered_map<string, string>::iterator fromIter = fromMap->find(tmpStr);
                    if(fromIter != fromMap->end()){
                        dfsStack->push(fromIter->second);
                    }
                }
            }
        }

        eulerTourSequenceLists->insert(unordered_map<int, list<string>*>::value_type(++counter, eulerTourSeq));
    }

    delete visitMap;
    delete elementSet;
    delete fromMap;
    delete dfsStack;
}

//print out the euler tour sequence
void idp_depthFirstTraversal::printEulerTourSequence() {
    cout << "Euler Tour Sequence lists: " << eulerTourSequenceLists->size() << endl;
    for(unordered_map<int, list<string>*>::iterator lists_iter = eulerTourSequenceLists->begin();
            lists_iter != eulerTourSequenceLists->end(); lists_iter++){
        cout << "\t" << lists_iter->first << "(" << lists_iter->second->size() << ")" << ": ";
        for(list<string>::iterator list_iter = lists_iter->second->begin(); list_iter != lists_iter->second->end();
                list_iter++){
            cout << *list_iter << " ";
        }
        cout << endl;
    }
}

//print out the tree edges and non-tree edges
void idp_depthFirstTraversal::printEdgeLists() {
    cout << "edge list:" << endl;
    for(unordered_map<graphEdge, int, hashEdge>::iterator edgeIter = allEdge->begin(); edgeIter != allEdge->end(); edgeIter++){
        cout << "\t(" << edgeIter->first.endPointer1 << "," << edgeIter->first.endPointer2 << ") at level " << edgeIter->second << endl;
    }

    cout << "tree edge list:" << endl;
    for(unordered_map<graphEdge, int, hashEdge>::iterator edgeIter = treeEdges->begin(); edgeIter != treeEdges->end(); edgeIter++){
        cout << "\t(" << edgeIter->first.endPointer1 << "," << edgeIter->first.endPointer2 << ") at level " << edgeIter->second << endl;
    }

    cout << "non-tree edge list:" << endl;
    for(unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*>::iterator subIter = nonTreeEdgesList->begin();
        subIter != nonTreeEdgesList->end(); subIter++){
        cout << "\tendpoint:" << subIter->first << " size of " << subIter->second->size() << endl;
        for(unordered_map<int, unordered_set<graphEdge, hashEdge>*>::iterator treeIter = subIter->second->begin();
            treeIter != subIter->second->end(); treeIter++){
            cout << "\tlevel:" << treeIter->first;
            for(unordered_set<graphEdge,hashEdge>::iterator setIter = treeIter->second->begin(); setIter != treeIter->second->end(); setIter++){
                cout << "(" << (*setIter).endPointer1 << "," << (*setIter).endPointer2 << ")";
            }
            cout << endl;
        }
    }
}

//destructor
idp_depthFirstTraversal::~idp_depthFirstTraversal() {
    for(unordered_map<int,list<string> *>::iterator eulerIter = eulerTourSequenceLists->begin();
            eulerIter != eulerTourSequenceLists->end(); eulerIter++){
        delete eulerIter->second;
    }
    delete eulerTourSequenceLists;
}