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

#include "idp_hdt.h"

using namespace std;

//constructor
idp_hdt::idp_hdt(int numberOfNode, unordered_set<treapNode *> *treapList,
                 unordered_map<string, treapNode *> *activeOccurList, bool enablePromotion,
                 unordered_map<graphEdge, int, hashEdge> *allEdge,
                 unordered_map<graphEdge, int, hashEdge> *treeEdgeList,
                 unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge> *> *> *nonTreeEdgeList,
                 unordered_map<string, unordered_set<string> *> *adjacenctList,
                 unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *> *levelLabelReference,
                 int countOfNode) {
    //maximum level of spanning forests
    logN = (int)log2((double)numberOfNode) + 1;
    if(!enablePromotion){
        logN = 0;
    }

    //initialize level 0 spanning tree by using treaps
    levelTreapMap->insert(unordered_map<int, unordered_set<treapNode*>*>::value_type(0,treapList));

    //initialize level 0 active occurrence list
    levelActiveOccurMap->insert(unordered_map<int, unordered_map<string, treapNode*>*>::value_type(0, activeOccurList));

    //initialize tree edge list and non-tree edge list
    edgeList = allEdge;
    treeEdge = treeEdgeList;
    nonTreeEdge = nonTreeEdgeList;

    //initialize adjacency list
    adjacencyList = adjacenctList;

    //initialize promotion switch
    promotionSwitch = enablePromotion;

    //initialize label reference
    labelReference = levelLabelReference;

    //initialize nodeCount
    nodeCount = countOfNode;
}


//delete method of hdt
void idp_hdt::hdtDeleteEdge(string u, string v) {

    bool isNonTreeEdge = false;
    int edgeLevel = 0;

    graphEdge edgeDelete;
    if(u < v){
        edgeDelete.endPointer1 = u;
        edgeDelete.endPointer2 = v;
    }
    else{
        edgeDelete.endPointer1 = v;
        edgeDelete.endPointer2 = u;
    }

    //query level of this edge
    unordered_map<graphEdge,int,hashEdge>::iterator edgeIter = edgeList->find(edgeDelete);
    if(edgeIter != edgeList->end()){
        edgeLevel = edgeIter->second;

        //firstly query if it is non-tree edge
        unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*>::iterator nonEdgeIter = nonTreeEdge->find(u);
        if(nonEdgeIter != nonTreeEdge->end()){
            unordered_map<int, unordered_set<graphEdge,hashEdge>*>::iterator levelIter = nonEdgeIter->second->find(edgeLevel);
            if(levelIter != nonEdgeIter->second->end()){
                unordered_set<graphEdge,hashEdge>::iterator eIter = levelIter->second->find(edgeDelete);
                if(eIter != levelIter->second->end()){
                    //find it, delete it directly
                    levelIter->second->erase(eIter);

                    //delete another piece of info
                    nonEdgeIter = nonTreeEdge->find(v);
                    if(nonEdgeIter != nonTreeEdge->end()){
                        levelIter = nonEdgeIter->second->find(edgeLevel);
                        if(levelIter != nonEdgeIter->second->end()){
                            eIter = levelIter->second->find(edgeDelete);
                            if(eIter != levelIter->second->end()){
                                levelIter->second->erase(eIter);

                                //delete this non-tree edge from edgeList
                                edgeList->erase(edgeIter);

                                //delete info from adjacency list
                                adjacencyListDeleteEdge(u,v);
                                adjacencyListDeleteEdge(v,u);

                                isNonTreeEdge = true;

                                return;
                            }
                        }
                    }
                }
            }
        }

        //when this edge is tree edge
        if(!isNonTreeEdge){
            //firstly delete it from edgeList
            edgeList->erase(edgeIter);

            //remove edge from treeEdge list
            edgeIter = treeEdge->find(edgeDelete);
            if(edgeIter != treeEdge->end() && edgeLevel == edgeIter->second){
                treeEdge->erase(edgeIter);

                //delete the edge from adjacency list
                adjacencyListDeleteEdge(u,v);
                adjacencyListDeleteEdge(v,u);

                //delete the edge from spanning trees from level edgeLevel
                for(int i = edgeLevel; i >= 0; i--){
                    //split the spanning tree
                    hdtSplit(u,v,i);

                    unordered_map<int, cutTreaps*>::iterator lvIter = treapVector->find(i);
                    if(lvIter != treapVector->end()){
                        treapVector->erase(lvIter);
                    }
                    cutTreaps *uvTreaps = new cutTreaps(treapU, treapV);
                    treapVector->insert(unordered_map<int, cutTreaps*>::value_type(i,uvTreaps));
                }

                //after delete the edge at each level
                //move on to find a replacement edge
                graphEdge replacementEdge(" ", " ");
                int foundAtLevel = -1;
                for(int i = edgeLevel; i >= 0; i--){
                    unordered_map<int, cutTreaps*>::iterator treapsIter = treapVector->find(i);
                    if(treapsIter != treapVector->end()){
                        cutTreaps *tempCutTreaps = treapsIter->second;

                        if(promotionSwitch){
                            //enable edge promotions
                            //promote tree edges from the smaller spanning tree at level i to i + 1
                            if(i <= logN - 1){
                                //print out info
                                promoteTreeEdges(i,tempCutTreaps);
                            }
                        }

                        if(tempCutTreaps->treap1->weight < tempCutTreaps->treap2->weight){
                            replacementEdge = findReplacementEdge(tempCutTreaps->treap1, tempCutTreaps->treap2, i);
                        }
                        else{
                            replacementEdge = findReplacementEdge(tempCutTreaps->treap2, tempCutTreaps->treap1, i);
                        }

                        if(replacementEdge.endPointer1 != " " && replacementEdge.endPointer2 != " "){
                            foundAtLevel = i;
                            break;
                        }
                    }
                }

                if(replacementEdge.endPointer1 != " " && replacementEdge.endPointer2 != " "){
                    //if there exist a replacement edge
                    for(int i = edgeLevel; i >= 0; i--){
                        if(i > foundAtLevel){
                            continue;
                        }

                        unordered_map<int, unordered_map<string, treapNode*> *>::iterator lvMapIter = levelActiveOccurMap->find(i);
                        if(lvMapIter != levelActiveOccurMap->end()){
                            treapNode *root1 = nullptr;
                            treapNode *root2 = nullptr;
                            unordered_map<string, treapNode*>::iterator mapIter = lvMapIter->second->find(replacementEdge.endPointer1);
                            if(mapIter != lvMapIter->second->end()){
                                root1 = hdtFindRoot(mapIter->second);
                            }

                            mapIter = lvMapIter->second->find(replacementEdge.endPointer2);
                            if(mapIter != lvMapIter->second->end()){
                                root2 = hdtFindRoot(mapIter->second);
                            }

                            unordered_map<int, unordered_set<treapNode*> *>::iterator lvSetIter = levelTreapMap->find(i);
                            if(lvSetIter != levelTreapMap->end()){
                                unordered_set<treapNode*>::iterator setIter = lvSetIter->second->find(root1);
                                if(setIter != lvSetIter->second->end()){
                                    lvSetIter->second->erase(setIter);
                                }

                                setIter = lvSetIter->second->find(root2);
                                if(setIter != lvSetIter->second->end()){
                                    lvSetIter->second->erase(setIter);
                                }

                                rerooting(replacementEdge.endPointer1,root1,i);
                                rerooting(replacementEdge.endPointer2,root2,i);

                                treapNode *newNode = new treapNode();
                                newNode->nodeLabel = replacementEdge.endPointer1;
                                newNode->active = false;
                                newNode->actChild = false;
                                newNode->rChild = nullptr;
                                newNode->lChild = nullptr;
                                newNode->weight = 1;
                                newNode->priority = ((rand()<<15)+rand())%200000000;
                                newNode->number = nodeCount++;
                                newNode->parent = nullptr;
                                newNode->activeOccur = false;

                                treapNode *piece1 = reLinkTreaps(root1,root2,i);
                                treapNode *piece2 = reLinkTreaps(piece1,newNode,i);

                                //piece2 is the final treap
                                lvSetIter->second->insert(piece2);
                            }
                        }
                    }
                }

                treapU = nullptr;
                treapV = nullptr;

                treapVector->clear();
            }
        }
    }
    else{
        cout << "cannot find the edge in the edge list!" << endl;
    }
}

//promote tree edges from level i to level i + 1
void idp_hdt::promoteTreeEdges(int edgeLV, cutTreaps *currentCutTreaps) {
    treapNode *smallTreap = currentCutTreaps->treap1->weight < currentCutTreaps->treap2->weight?currentCutTreaps->treap1:currentCutTreaps->treap2;
    unordered_set<string> *existNode = new unordered_set<string>();
    unordered_set<graphEdge,hashEdge> *visitedEdge = new unordered_set<graphEdge,hashEdge>();
    stack<treapNode*> *tempStack = new stack<treapNode*>();

    //traverse the entire small treap
    //for each node encountered, create a copy of the node
    //when finish, add the new one to level i + 1, and update corresponding info
    treapNode *p = smallTreap;

    while(!tempStack->empty() || p != nullptr){
        while(p != nullptr){
            tempStack->push(p);
            p = p->lChild;
        }

        if(!tempStack->empty()){
            p = tempStack->top();
            tempStack->pop();

            //handle the node
            unordered_set<string>::iterator existIter = existNode->find(p->nodeLabel);
            if(existIter == existNode->end()){
                existNode->insert(p->nodeLabel);

                unordered_map<string, unordered_set<string>*>::iterator adjIter = adjacencyList->find(p->nodeLabel);
                if(adjIter != adjacencyList->end()){
                    for(unordered_set<string>::iterator adjEdgeIter = adjIter->second->begin(); adjEdgeIter != adjIter->second->end();
                        adjEdgeIter++){
                        graphEdge insertEdge;
                        if(p->nodeLabel < *adjEdgeIter){
                            insertEdge.endPointer1 = p->nodeLabel;
                            insertEdge.endPointer2 = *adjEdgeIter;
                        }
                        else{
                            insertEdge.endPointer1 = *adjEdgeIter;
                            insertEdge.endPointer2 = p->nodeLabel;
                        }
                        unordered_set<graphEdge,hashEdge>::iterator visitIter = visitedEdge->find(insertEdge);
                        if(visitIter == visitedEdge->end()){
                            visitedEdge->insert(insertEdge);

                            //update edge info in edgeList and treeEdge
                            unordered_map<graphEdge, int, hashEdge>::iterator treeEdgeIter = treeEdge->find(insertEdge);
                            if(treeEdgeIter != treeEdge->end()){
                                if(treeEdgeIter->second == edgeLV){
                                    treeEdgeIter->second += 1;

                                    treeEdgeIter = edgeList->find(insertEdge);
                                    if(treeEdgeIter != edgeList->end()){
                                        if(treeEdgeIter->second == edgeLV){
                                            treeEdgeIter->second += 1;
                                        }
                                    }

                                    unordered_map<int, unordered_map<string, treapNode*> *>::iterator occurLvIter = levelActiveOccurMap->find(edgeLV+1);
                                    if(occurLvIter != levelActiveOccurMap->end()){
                                        unordered_map<string, treapNode*>::iterator occurIter1 = occurLvIter->second->find(insertEdge.endPointer1);
                                        unordered_map<string, treapNode*>::iterator occurIter2 = occurLvIter->second->find(insertEdge.endPointer2);

                                        if(occurIter1 != occurLvIter->second->end() && occurIter2 != occurLvIter->second->end()){
                                            //both nodes have already been in the level i+1
                                            treapNode *rootA = hdtFindRoot(occurIter1->second);
                                            treapNode *rootB = hdtFindRoot(occurIter2->second);
                                            unordered_map<int, unordered_set<treapNode*> *>::iterator lvTreapIter = levelTreapMap->find(edgeLV+1);
                                            if(lvTreapIter != levelTreapMap->end()){
                                                unordered_set<treapNode*>::iterator treapIter = lvTreapIter->second->find(rootA);
                                                if(treapIter != lvTreapIter->second->end()){
                                                    lvTreapIter->second->erase(treapIter);
                                                }

                                                treapIter = lvTreapIter->second->find(rootB);
                                                if(treapIter != lvTreapIter->second->end()){
                                                    lvTreapIter->second->erase(treapIter);
                                                }

                                                rerooting(insertEdge.endPointer1,rootA,edgeLV+1);
                                                rerooting(insertEdge.endPointer2,rootB,edgeLV+1);

                                                treapNode *piece1 = reLinkTreaps(rootA, rootB, edgeLV+1);

                                                treapNode *newCopy = new treapNode();
                                                newCopy->nodeLabel = insertEdge.endPointer1;
                                                newCopy->rChild = nullptr;
                                                newCopy->lChild = nullptr;
                                                newCopy->weight = 1;
                                                newCopy->priority = ((rand()<<15)+rand())%200000000;
                                                newCopy->number = nodeCount++;
                                                newCopy->parent = nullptr;
                                                newCopy->actChild = false;
                                                newCopy->activeOccur = false;
                                                newCopy->active = false;

                                                treapNode *piece2 = reLinkTreaps(piece1,newCopy, edgeLV+1);

                                                lvTreapIter->second->insert(piece2);
                                            }
                                        }
                                        else if(occurIter1 != occurLvIter->second->end() && occurIter2 == occurLvIter->second->end()){
                                            //only one of two nodes in the level i+!
                                            treapNode *rootA = hdtFindRoot(occurIter1->second);
                                            unordered_map<int, unordered_set<treapNode*> *>::iterator lvTreapIter = levelTreapMap->find(edgeLV+1);
                                            if(lvTreapIter != levelTreapMap->end()){
                                                unordered_set<treapNode*>::iterator treapIter = lvTreapIter->second->find(rootA);
                                                if(treapIter != lvTreapIter->second->end()){
                                                    lvTreapIter->second->erase(treapIter);
                                                }

                                                treapNode *newNode = new treapNode();
                                                newNode->nodeLabel = insertEdge.endPointer2;
                                                newNode->rChild = nullptr;
                                                newNode->lChild = nullptr;
                                                newNode->weight = 1;
                                                newNode->priority = ((rand()<<15)+rand())%200000000;
                                                newNode->number = nodeCount++;
                                                newNode->parent = nullptr;
                                                newNode->actChild = false;
                                                newNode->activeOccur = true;
                                                newNode->active = false;

                                                unordered_map<int, unordered_map<string, treapNode*> *>::iterator lvfinder = levelActiveOccurMap->find(edgeLV);
                                                if(lvfinder != levelActiveOccurMap->end()){
                                                    unordered_map<string, treapNode*>::iterator strfinder = lvfinder->second->find(insertEdge.endPointer2);
                                                    if(strfinder != lvfinder->second->end()){
                                                        newNode->active = strfinder->second->active;
                                                    }
                                                }

                                                occurLvIter->second->insert(unordered_map<string, treapNode*>::value_type(newNode->nodeLabel,newNode));

                                                rerooting(insertEdge.endPointer1,rootA,edgeLV+1);
                                                treapNode *piece1 = reLinkTreaps(rootA,newNode,edgeLV+1);

                                                treapNode *newCopy = new treapNode();
                                                newCopy->nodeLabel = insertEdge.endPointer1;
                                                newCopy->rChild = nullptr;
                                                newCopy->lChild = nullptr;
                                                newCopy->weight = 1;
                                                newCopy->priority = ((rand()<<15)+rand())%200000000;
                                                newCopy->number = nodeCount++;
                                                newCopy->parent = nullptr;
                                                newCopy->actChild = false;
                                                newCopy->activeOccur = false;
                                                newCopy->active = false;

                                                treapNode *piece2 = reLinkTreaps(piece1,newCopy, edgeLV+1);

                                                lvTreapIter->second->insert(piece2);
                                            }
                                        }
                                        else if(occurIter1 == occurLvIter->second->end() && occurIter2 != occurLvIter->second->end()){
                                            //only one of two nodes in the level i+!
                                            treapNode *rootA = hdtFindRoot(occurIter2->second);
                                            unordered_map<int, unordered_set<treapNode*> *>::iterator lvTreapIter = levelTreapMap->find(edgeLV+1);
                                            if(lvTreapIter != levelTreapMap->end()){
                                                unordered_set<treapNode*>::iterator treapIter = lvTreapIter->second->find(rootA);
                                                if(treapIter != lvTreapIter->second->end()){
                                                    lvTreapIter->second->erase(treapIter);
                                                }

                                                treapNode *newNode = new treapNode();
                                                newNode->nodeLabel = insertEdge.endPointer1;
                                                newNode->rChild = nullptr;
                                                newNode->lChild = nullptr;
                                                newNode->weight = 1;
                                                newNode->priority = ((rand()<<15)+rand())%200000000;
                                                newNode->number = nodeCount++;
                                                newNode->parent = nullptr;
                                                newNode->actChild = false;
                                                newNode->activeOccur = true;
                                                newNode->active = false;

                                                unordered_map<int, unordered_map<string, treapNode*> *>::iterator lvfinder = levelActiveOccurMap->find(edgeLV);
                                                if(lvfinder != levelActiveOccurMap->end()){
                                                    unordered_map<string, treapNode*>::iterator strfinder = lvfinder->second->find(insertEdge.endPointer1);
                                                    if(strfinder != lvfinder->second->end()){
                                                        newNode->active = strfinder->second->active;
                                                    }
                                                }

                                                occurLvIter->second->insert(unordered_map<string, treapNode*>::value_type(newNode->nodeLabel,newNode));

                                                rerooting(insertEdge.endPointer2,rootA,edgeLV+1);
                                                treapNode *piece1 = reLinkTreaps(rootA,newNode,edgeLV+1);

                                                treapNode *newCopy = new treapNode();
                                                newCopy->nodeLabel = insertEdge.endPointer2;
                                                newCopy->rChild = nullptr;
                                                newCopy->lChild = nullptr;
                                                newCopy->weight = 1;
                                                newCopy->priority = ((rand()<<15)+rand())%200000000;
                                                newCopy->number = nodeCount++;
                                                newCopy->parent = nullptr;
                                                newCopy->actChild = false;
                                                newCopy->activeOccur = false;
                                                newCopy->active = false;

                                                treapNode *piece2 = reLinkTreaps(piece1,newCopy, edgeLV+1);

                                                lvTreapIter->second->insert(piece2);
                                            }
                                        }
                                        else{
                                            //both of them are not in the level i+1
                                            unordered_map<int, unordered_set<treapNode*> *>::iterator lvTreapIter = levelTreapMap->find(edgeLV+1);
                                            if(lvTreapIter != levelTreapMap->end()){

                                                treapNode *newNodeU = new treapNode();
                                                newNodeU->nodeLabel = insertEdge.endPointer1;
                                                newNodeU->rChild = nullptr;
                                                newNodeU->lChild = nullptr;
                                                newNodeU->weight = 1;
                                                newNodeU->priority = ((rand()<<15)+rand())%200000000;
                                                newNodeU->number = nodeCount++;
                                                newNodeU->parent = nullptr;
                                                newNodeU->actChild = false;
                                                newNodeU->activeOccur = true;
                                                newNodeU->active = false;

                                                unordered_map<int, unordered_map<string, treapNode*> *>::iterator lvfinder = levelActiveOccurMap->find(edgeLV);
                                                if(lvfinder != levelActiveOccurMap->end()){
                                                    unordered_map<string, treapNode*>::iterator strfinder = lvfinder->second->find(insertEdge.endPointer1);
                                                    if(strfinder != lvfinder->second->end()){
                                                        newNodeU->active = strfinder->second->active;
                                                    }
                                                }
                                                occurLvIter->second->insert(unordered_map<string, treapNode*>::value_type(newNodeU->nodeLabel,newNodeU));

                                                treapNode *newNodeV = new treapNode();
                                                newNodeV->nodeLabel = insertEdge.endPointer2;
                                                newNodeV->rChild = nullptr;
                                                newNodeV->lChild = nullptr;
                                                newNodeV->weight = 1;
                                                newNodeV->priority = ((rand()<<15)+rand())%200000000;
                                                newNodeV->number = nodeCount++;
                                                newNodeV->parent = nullptr;
                                                newNodeV->actChild = false;
                                                newNodeV->activeOccur = true;
                                                newNodeV->active = false;

                                                lvfinder = levelActiveOccurMap->find(edgeLV);
                                                if(lvfinder != levelActiveOccurMap->end()){
                                                    unordered_map<string, treapNode*>::iterator strfinder = lvfinder->second->find(insertEdge.endPointer2);
                                                    if(strfinder != lvfinder->second->end()){
                                                        newNodeV->active = strfinder->second->active;
                                                    }
                                                }
                                                occurLvIter->second->insert(unordered_map<string, treapNode*>::value_type(newNodeV->nodeLabel,newNodeV));

                                                treapNode *piece1 = reLinkTreaps(newNodeU,newNodeV,edgeLV+1);

                                                treapNode *newCopy = new treapNode();
                                                newCopy->nodeLabel = insertEdge.endPointer1;
                                                newCopy->rChild = nullptr;
                                                newCopy->lChild = nullptr;
                                                newCopy->weight = 1;
                                                newCopy->priority = ((rand()<<15)+rand())%200000000;
                                                newCopy->number = nodeCount++;
                                                newCopy->parent = nullptr;
                                                newCopy->actChild = false;
                                                newCopy->activeOccur = false;
                                                newCopy->active = false;

                                                treapNode *piece2 = reLinkTreaps(piece1,newCopy, edgeLV+1);

                                                lvTreapIter->second->insert(piece2);
                                            }
                                            else{

                                                treapNode *newNodeU = new treapNode();
                                                newNodeU->nodeLabel = insertEdge.endPointer1;
                                                newNodeU->rChild = nullptr;
                                                newNodeU->lChild = nullptr;
                                                newNodeU->weight = 1;
                                                newNodeU->priority = ((rand()<<15)+rand())%200000000;
                                                newNodeU->number = nodeCount++;
                                                newNodeU->parent = nullptr;
                                                newNodeU->actChild = false;
                                                newNodeU->activeOccur = true;
                                                newNodeU->active = false;

                                                unordered_map<int, unordered_map<string, treapNode*> *>::iterator lvfinder = levelActiveOccurMap->find(edgeLV);
                                                if(lvfinder != levelActiveOccurMap->end()){
                                                    unordered_map<string, treapNode*>::iterator strfinder = lvfinder->second->find(insertEdge.endPointer1);
                                                    if(strfinder != lvfinder->second->end()){
                                                        newNodeU->active = strfinder->second->active;
                                                    }
                                                }
                                                occurLvIter->second->insert(unordered_map<string, treapNode*>::value_type(newNodeU->nodeLabel,newNodeU));

                                                treapNode *newNodeV = new treapNode();
                                                newNodeV->nodeLabel = insertEdge.endPointer2;
                                                newNodeV->rChild = nullptr;
                                                newNodeV->lChild = nullptr;
                                                newNodeV->weight = 1;
                                                newNodeV->priority = ((rand()<<15)+rand())%200000000;
                                                newNodeV->number = nodeCount++;
                                                newNodeV->parent = nullptr;
                                                newNodeV->actChild = false;
                                                newNodeV->activeOccur = true;
                                                newNodeV->active = false;

                                                lvfinder = levelActiveOccurMap->find(edgeLV);
                                                if(lvfinder != levelActiveOccurMap->end()){
                                                    unordered_map<string, treapNode*>::iterator strfinder = lvfinder->second->find(insertEdge.endPointer2);
                                                    if(strfinder != lvfinder->second->end()){
                                                        newNodeV->active = strfinder->second->active;
                                                    }
                                                }
                                                occurLvIter->second->insert(unordered_map<string, treapNode*>::value_type(newNodeV->nodeLabel,newNodeV));

                                                treapNode *piece1 = reLinkTreaps(newNodeU,newNodeV,edgeLV+1);

                                                treapNode *newCopy = new treapNode();
                                                newCopy->nodeLabel = insertEdge.endPointer1;
                                                newCopy->rChild = nullptr;
                                                newCopy->lChild = nullptr;
                                                newCopy->weight = 1;
                                                newCopy->priority = ((rand()<<15)+rand())%200000000;
                                                newCopy->number = nodeCount++;
                                                newCopy->parent = nullptr;
                                                newCopy->actChild = false;
                                                newCopy->activeOccur = false;
                                                newCopy->active = false;

                                                treapNode *piece2 = reLinkTreaps(piece1,newCopy, edgeLV+1);

                                                unordered_set<treapNode*> *tempSet = new unordered_set<treapNode*>();
                                                tempSet->insert(piece2);
                                                levelTreapMap->insert(unordered_map<int, unordered_set<treapNode*>*>::value_type(edgeLV+1,tempSet));
                                            }
                                        }
                                    }
                                    else{
                                        //this level i+1 does not exist
                                        unordered_map<int, unordered_set<treapNode*> *>::iterator lvTreapIter = levelTreapMap->find(edgeLV+1);
                                        if(lvTreapIter != levelTreapMap->end()){

                                            treapNode *newNodeU = new treapNode();
                                            newNodeU->nodeLabel = insertEdge.endPointer1;
                                            newNodeU->rChild = nullptr;
                                            newNodeU->lChild = nullptr;
                                            newNodeU->weight = 1;
                                            newNodeU->priority = ((rand()<<15)+rand())%200000000;
                                            newNodeU->number = nodeCount++;
                                            newNodeU->parent = nullptr;
                                            newNodeU->actChild = false;
                                            newNodeU->activeOccur = true;
                                            newNodeU->active = false;

                                            unordered_map<int, unordered_map<string, treapNode*> *>::iterator lvfinder = levelActiveOccurMap->find(edgeLV);
                                            if(lvfinder != levelActiveOccurMap->end()){
                                                unordered_map<string, treapNode*>::iterator strfinder = lvfinder->second->find(insertEdge.endPointer1);
                                                if(strfinder != lvfinder->second->end()){
                                                    newNodeU->active = strfinder->second->active;
                                                }
                                            }
                                            unordered_map<string, treapNode*> *tempOccur = new unordered_map<string, treapNode*>();
                                            tempOccur->insert(unordered_map<string, treapNode*>::value_type(newNodeU->nodeLabel,newNodeU));

                                            treapNode *newNodeV = new treapNode();
                                            newNodeV->nodeLabel = insertEdge.endPointer2;
                                            newNodeV->rChild = nullptr;
                                            newNodeV->lChild = nullptr;
                                            newNodeV->weight = 1;
                                            newNodeV->priority = ((rand()<<15)+rand())%200000000;
                                            newNodeV->number = nodeCount++;
                                            newNodeV->parent = nullptr;
                                            newNodeV->actChild = false;
                                            newNodeV->activeOccur = true;
                                            newNodeV->active = false;

                                            lvfinder = levelActiveOccurMap->find(edgeLV);
                                            if(lvfinder != levelActiveOccurMap->end()){
                                                unordered_map<string, treapNode*>::iterator strfinder = lvfinder->second->find(insertEdge.endPointer2);
                                                if(strfinder != lvfinder->second->end()){
                                                    newNodeV->active = strfinder->second->active;
                                                }
                                            }
                                            tempOccur->insert(unordered_map<string, treapNode*>::value_type(newNodeV->nodeLabel,newNodeV));
                                            levelActiveOccurMap->insert(unordered_map<int, unordered_map<string, treapNode*> *>::value_type(edgeLV+1,tempOccur));

                                            treapNode *piece1 = reLinkTreaps(newNodeU,newNodeV,edgeLV+1);

                                            treapNode *newCopy = new treapNode();
                                            newCopy->nodeLabel = insertEdge.endPointer1;
                                            newCopy->rChild = nullptr;
                                            newCopy->lChild = nullptr;
                                            newCopy->weight = 1;
                                            newCopy->priority = ((rand()<<15)+rand())%200000000;
                                            newCopy->number = nodeCount++;
                                            newCopy->parent = nullptr;
                                            newCopy->actChild = false;
                                            newCopy->activeOccur = false;
                                            newCopy->active = false;

                                            treapNode *piece2 = reLinkTreaps(piece1,newCopy, edgeLV+1);

                                            lvTreapIter->second->insert(piece2);
                                        }
                                        else{
                                            treapNode *newNodeU = new treapNode();
                                            newNodeU->nodeLabel = insertEdge.endPointer1;
                                            newNodeU->rChild = nullptr;
                                            newNodeU->lChild = nullptr;
                                            newNodeU->weight = 1;
                                            newNodeU->priority = ((rand()<<15)+rand())%200000000;
                                            newNodeU->number = nodeCount++;
                                            newNodeU->parent = nullptr;
                                            newNodeU->actChild = false;
                                            newNodeU->activeOccur = true;
                                            newNodeU->active = false;

                                            unordered_map<int, unordered_map<string, treapNode*> *>::iterator lvfinder = levelActiveOccurMap->find(edgeLV);
                                            if(lvfinder != levelActiveOccurMap->end()){
                                                unordered_map<string, treapNode*>::iterator strfinder = lvfinder->second->find(insertEdge.endPointer1);
                                                if(strfinder != lvfinder->second->end()){
                                                    newNodeU->active = strfinder->second->active;
                                                }
                                            }
                                            unordered_map<string, treapNode*> *tempOccur = new unordered_map<string, treapNode*>();
                                            tempOccur->insert(unordered_map<string, treapNode*>::value_type(newNodeU->nodeLabel,newNodeU));

                                            treapNode *newNodeV = new treapNode();
                                            newNodeV->nodeLabel = insertEdge.endPointer2;
                                            newNodeV->rChild = nullptr;
                                            newNodeV->lChild = nullptr;
                                            newNodeV->weight = 1;
                                            newNodeV->priority = ((rand()<<15)+rand())%200000000;
                                            newNodeV->number = nodeCount++;
                                            newNodeV->parent = nullptr;
                                            newNodeV->actChild = false;
                                            newNodeV->activeOccur = true;
                                            newNodeV->active = false;

                                            lvfinder = levelActiveOccurMap->find(edgeLV);
                                            if(lvfinder != levelActiveOccurMap->end()){
                                                unordered_map<string, treapNode*>::iterator strfinder = lvfinder->second->find(insertEdge.endPointer2);
                                                if(strfinder != lvfinder->second->end()){
                                                    newNodeV->active = strfinder->second->active;
                                                }
                                            }
                                            tempOccur->insert(unordered_map<string, treapNode*>::value_type(newNodeV->nodeLabel,newNodeV));
                                            levelActiveOccurMap->insert(unordered_map<int, unordered_map<string, treapNode*> *>::value_type(edgeLV+1,tempOccur));

                                            treapNode *piece1 = reLinkTreaps(newNodeU,newNodeV,edgeLV+1);

                                            treapNode *newCopy = new treapNode();
                                            newCopy->nodeLabel = insertEdge.endPointer1;
                                            newCopy->rChild = nullptr;
                                            newCopy->lChild = nullptr;
                                            newCopy->weight = 1;
                                            newCopy->priority = ((rand()<<15)+rand())%200000000;
                                            newCopy->number = nodeCount++;
                                            newCopy->parent = nullptr;
                                            newCopy->actChild = false;
                                            newCopy->activeOccur = false;
                                            newCopy->active = false;

                                            treapNode *piece2 = reLinkTreaps(piece1,newCopy, edgeLV+1);

                                            unordered_set<treapNode*> *tempSet = new unordered_set<treapNode*>();
                                            tempSet->insert(piece2);
                                            levelTreapMap->insert(unordered_map<int, unordered_set<treapNode*>*>::value_type(edgeLV+1,tempSet));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            p = p->rChild;
        }
    }

    delete existNode;
    delete visitedEdge;
    delete tempStack;
}

//delete corresponding edge from adjacency list
void idp_hdt::adjacencyListDeleteEdge(string u, string v) {
    unordered_map<string, unordered_set<string>*>::iterator adjIter = adjacencyList->find(u);
    if(adjIter != adjacencyList->end()){
        unordered_set<string>::iterator setIter = adjIter->second->find(v);
        if(setIter != adjIter->second->end()){
            adjIter->second->erase(setIter);
        }
    }
}

//delete tree edges from spanning trees
void idp_hdt::hdtSplit(string u, string v, int edgeLV) {
    unordered_map<int, unordered_map<string, treapNode*> *>::iterator levelIter = levelActiveOccurMap->find(edgeLV);
    if(levelIter != levelActiveOccurMap->end()){
        unordered_map<string, treapNode*>::iterator uIter = levelIter->second->find(u);
        unordered_map<string, treapNode*>::iterator vIter = levelIter->second->find(v);
        if(uIter != levelIter->second->end() && vIter != levelIter->second->end()){
            treapNode* activeOccurU = uIter->second;
            treapNode* activeOccurV = vIter->second;

            treapNode* root1 = hdtFindRoot(activeOccurU);
            treapNode* root2 = hdtFindRoot(activeOccurV);

            if(root1 == root2){
                unordered_map<int, unordered_set<treapNode*> *>::iterator listIter = levelTreapMap->find(edgeLV);
                if(listIter != levelTreapMap->end()){
                    unordered_set<treapNode*>::iterator setIter = listIter->second->find(root1);
                    if(setIter != listIter->second->end()){
                        listIter->second->erase(setIter);
                    }

                    hdtCutTreap(u, v, root1, edgeLV);

                    listIter->second->insert(treapU);
                    listIter->second->insert(treapV);
                }
            }
        }
    }
}

//find the root of treap that contains the active node
treapNode* idp_hdt::hdtFindRoot(treapNode *occur) {
    if(occur == nullptr){
        return nullptr;
    }
    treapNode* temp = nullptr;
    for(treapNode* p = occur; p; p = p->parent){
        temp = p;
    }
    return temp;
}

//split the treap at corresponding position by using rotation of treaps
void idp_hdt::hdtCutTreap(string u, string v, treapNode *&treap, int edgeLV) {
    treapNode* occurPointer1 = nullptr;
    treapNode* occurPointer2 = nullptr;
    treapNode* occurPointer3 = nullptr;
    treapNode* occurPointer4 = nullptr;

    //since a pair of edges has 3 or 4 occurrences, we shall get it
    unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>::iterator edgeIter =
            labelReference->find(u);
    if(edgeIter != labelReference->end()){
        unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator lvIter = edgeIter->second->find(edgeLV);
        if(lvIter != edgeIter->second->end()){
            unordered_map<string, unordered_set<treapNode*> *>::iterator endIter = lvIter->second->find(v);
            if(endIter != lvIter->second->end()){
                unordered_set<treapNode*>::iterator setIter = endIter->second->begin();
                if(setIter != endIter->second->end()){
                    if(endIter->second->size() == 1){
                        occurPointer1 = *(setIter);
                        occurPointer2 = nullptr;
                    }
                    else if(endIter->second->size() == 2){
                        occurPointer1 = *(setIter++);
                        occurPointer2 = *(setIter);
                    }
                }
            }
        }
    }

    edgeIter = labelReference->find(v);
    if(edgeIter != labelReference->end()){
        unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator lvIter = edgeIter->second->find(edgeLV);
        if(lvIter != edgeIter->second->end()){
            unordered_map<string, unordered_set<treapNode*> *>::iterator endIter = lvIter->second->find(u);
            if(endIter != lvIter->second->end()){
                unordered_set<treapNode*>::iterator setIter = endIter->second->begin();
                if(setIter != endIter->second->end()){
                    if(endIter->second->size() == 1){
                        occurPointer3 = *(setIter);
                        occurPointer4 = nullptr;
                    }
                    else if(endIter->second->size() == 2){
                        occurPointer3 = *(setIter++);
                        occurPointer4 = *(setIter);
                    }
                }
            }
        }
    }

    treapNode *tempForCompare;
    //make sure u1 < u2
    if(occurPointer1 != nullptr && occurPointer2 != nullptr){
        if(isFirstOccurSmallerThanSecondOccur(occurPointer2, occurPointer1)){
            tempForCompare = occurPointer2;
            occurPointer2 = occurPointer1;
            occurPointer1 = tempForCompare;
        }
    }

    //make sure v1 < v2
    if(occurPointer3 != nullptr && occurPointer4 != nullptr){
        if(isFirstOccurSmallerThanSecondOccur(occurPointer4, occurPointer3)){
            tempForCompare = occurPointer4;
            occurPointer4 = occurPointer3;
            occurPointer3 = tempForCompare;
        }
    }

    //u1 < u2, v1 < v2
    if(occurPointer1 != nullptr && occurPointer3 != nullptr){
        if(isFirstOccurSmallerThanSecondOccur(occurPointer3, occurPointer1)){
            tempForCompare = occurPointer3;
            occurPointer3 = occurPointer1;
            occurPointer1 = tempForCompare;

            tempForCompare = occurPointer3;
            occurPointer3 = occurPointer2;
            occurPointer2 = tempForCompare;
        }
        else{
            tempForCompare = occurPointer3;
            occurPointer3 = occurPointer2;
            occurPointer2 = tempForCompare;

            tempForCompare = occurPointer4;
            occurPointer4 = occurPointer3;
            occurPointer3 = tempForCompare;
        }
    }

    treapNode *piece1 = nullptr;
    treapNode *piece2 = nullptr;
    treapNode *piece3 = nullptr;
    treapNode *piece4 = nullptr;

    //delete the adjacent pointers
    unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>::iterator referenceIter
            = labelReference->find(occurPointer1->nodeLabel);
    if(referenceIter != labelReference->end()){
        unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = referenceIter->second->find(edgeLV);
        if(levelIter != referenceIter->second->end()){
            unordered_map<string, unordered_set<treapNode*> *>::iterator eIter = levelIter->second->find(occurPointer2->nodeLabel);
            if(eIter != levelIter->second->end()){
                unordered_set<treapNode*>::iterator setIter = eIter->second->find(occurPointer2);
                if(setIter != eIter->second->end()){
                    eIter->second->erase(setIter);
                }
            }
        }
    }

    referenceIter = labelReference->find(occurPointer2->nodeLabel);
    if(referenceIter != labelReference->end()){
        unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = referenceIter->second->find(edgeLV);
        if(levelIter != referenceIter->second->end()){
            unordered_map<string, unordered_set<treapNode*> *>::iterator eIter = levelIter->second->find(occurPointer1->nodeLabel);
            if(eIter != levelIter->second->end()){
                unordered_set<treapNode*>::iterator setIter = eIter->second->find(occurPointer1);
                if(setIter != eIter->second->end()){
                    eIter->second->erase(setIter);
                }
            }
        }
    }

    hdtCutTreapAtRightPosition(treap,occurPointer1,piece1,piece2);

    //delete the adjacent pointers
    if(occurPointer3 != nullptr){
        unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>::iterator refIter
                = labelReference->find(occurPointer3->nodeLabel);
        if(refIter != labelReference->end()){
            unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = refIter->second->find(edgeLV);
            if(levelIter != refIter->second->end()){
                unordered_map<string, unordered_set<treapNode*> *>::iterator eIter = levelIter->second->find(occurPointer4->nodeLabel);
                if(eIter != levelIter->second->end()){
                    unordered_set<treapNode*>::iterator setIter = eIter->second->find(occurPointer4);
                    if(setIter != eIter->second->end()){
                        eIter->second->erase(setIter);
                    }
                }
            }
        }

        refIter = labelReference->find(occurPointer4->nodeLabel);
        if(refIter != labelReference->end()){
            unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = refIter->second->find(edgeLV);
            if(levelIter != refIter->second->end()){
                unordered_map<string, unordered_set<treapNode*> *>::iterator eIter = levelIter->second->find(occurPointer3->nodeLabel);
                if(eIter != levelIter->second->end()){
                    unordered_set<treapNode*>::iterator setIter = eIter->second->find(occurPointer3);
                    if(setIter != eIter->second->end()){
                        eIter->second->erase(setIter);
                    }
                }
            }
        }
    }
    else{
        unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>::iterator refIter
                = labelReference->find(occurPointer2->nodeLabel);
        if(refIter != labelReference->end()){
            unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = refIter->second->find(edgeLV);
            if(levelIter != refIter->second->end()){
                unordered_map<string, unordered_set<treapNode*> *>::iterator eIter = levelIter->second->find(occurPointer4->nodeLabel);
                if(eIter != levelIter->second->end()){
                    unordered_set<treapNode*>::iterator setIter = eIter->second->find(occurPointer4);
                    if(setIter != eIter->second->end()){
                        eIter->second->erase(setIter);
                    }
                }
            }
        }

        refIter = labelReference->find(occurPointer4->nodeLabel);
        if(refIter != labelReference->end()){
            unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = refIter->second->find(edgeLV);
            if(levelIter != refIter->second->end()){
                unordered_map<string, unordered_set<treapNode*> *>::iterator eIter = levelIter->second->find(occurPointer2->nodeLabel);
                if(eIter != levelIter->second->end()){
                    unordered_set<treapNode*>::iterator setIter = eIter->second->find(occurPointer2);
                    if(setIter != eIter->second->end()){
                        eIter->second->erase(setIter);
                    }
                }
            }
        }
    }

    hdtCutTreapAtLeftPosition(piece2,occurPointer4,piece3,piece4);

    //piece1 is the leftmost piece, piece3 is the middle one, piece4 is the rightmost one
    //the rightmost node of piece1 is the same as the leftmost node of piece4

    //then delete the leftmost node from the treap piece4 and relink pieces1 and piece4
    //return the relinked piece and piece3 as the resulted treaps

    //look at the piece4, if the treap contains only 1 node, then it must be the same as the rightmost node in piece1
    //if the size of treap piece4 is greater than 1, then cut the treap piece4 at the right position of the leftmost node
    //one of resulted pieces contains that node and needs to be deleted, while another resulting piece will be returned as
    //part of the treap. Then relink two pieces
    if(piece4->weight == 1){
        //piece4 contains only one single node
        if(piece4->activeOccur){
            treapNode *rightmostNode = findRightmostNode(piece1);
            rightmostNode->activeOccur = true;
            //update activeOccur info
            unordered_map<int, unordered_map<string, treapNode*> *>::iterator lvIter = levelActiveOccurMap->find(edgeLV);
            if(lvIter != levelActiveOccurMap->end()){
                unordered_map<string, treapNode*>::iterator eIter = lvIter->second->find(rightmostNode->nodeLabel);
                if(eIter != lvIter->second->end()){
                    eIter->second = rightmostNode;
                }
            }

            if(piece4->active){
                rightmostNode->active = true;
                //backtrack all the way to the root to update activeChild info
                for(treapNode* p = rightmostNode; p; p = p->parent){
                    if(p->parent != nullptr){
                        p->parent->actChild = true;
                    }
                }
            }
        }

        delete piece4;

        treapU = piece1;
        treapV = piece3;
    }
    else{
        //piece4 contains more than 1 single node
        treapNode *leftmostNode = findLeftmostNode(piece4);

        treapNode *subPiece1 = nullptr;
        treapNode *subPiece2 = nullptr;
        hdtCutTreapAtRightPosition(piece4,leftmostNode,subPiece1,subPiece2);

        if(subPiece1->activeOccur){
            treapNode *rightmostNode = findRightmostNode(piece1);
            rightmostNode->activeOccur = true;
            //update activeOccur info

            unordered_map<int, unordered_map<string, treapNode*> *>::iterator lvIter = levelActiveOccurMap->find(edgeLV);
            if(lvIter != levelActiveOccurMap->end()){
                unordered_map<string, treapNode*>::iterator eIter = lvIter->second->find(rightmostNode->nodeLabel);
                if(eIter != lvIter->second->end()){
                    eIter->second = rightmostNode;
                }
            }

            if(subPiece1->active){
                rightmostNode->active = true;
                //backtrack all the way to the root to update activeChild info
                for(treapNode* p = rightmostNode; p; p = p->parent){
                    if(p->parent != nullptr){
                        p->parent->actChild = true;
                    }
                }
            }
        }

        treapNode *lmNode = findLeftmostNode(subPiece2);
        //delete the adjacent pointers
        unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>::iterator refIter
                = labelReference->find(subPiece1->nodeLabel);
        if(refIter != labelReference->end()){
            unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = refIter->second->find(edgeLV);
            if(levelIter != refIter->second->end()){
                unordered_map<string, unordered_set<treapNode*> *>::iterator eIter = levelIter->second->find(lmNode->nodeLabel);
                if(eIter != levelIter->second->end()){
                    unordered_set<treapNode*>::iterator setIter = eIter->second->find(lmNode);
                    if(setIter != eIter->second->end()){
                        eIter->second->erase(setIter);
                    }
                }
            }
        }

        refIter = labelReference->find(lmNode->nodeLabel);
        if(refIter != labelReference->end()){
            unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = refIter->second->find(edgeLV);
            if(levelIter != refIter->second->end()){
                unordered_map<string, unordered_set<treapNode*> *>::iterator eIter = levelIter->second->find(subPiece1->nodeLabel);
                if(eIter != levelIter->second->end()){
                    unordered_set<treapNode*>::iterator setIter = eIter->second->find(subPiece1);
                    if(setIter != eIter->second->end()){
                        eIter->second->erase(setIter);
                    }
                }
            }
        }

        delete subPiece1;

        //reconnect piece1 and subPiece2
        treapU = reLinkTreaps(piece1,subPiece2,edgeLV);
        treapV = piece3;
    }
}

//This method borrows idea from Raj D. Iyer and H. S. Rahul's implementation of HDT algorithm and implement it by ourselves
//full citation: R. Iyer, D. Karger, H. Rahul, and M. Thorup. An experimental study of polylogarithmic,
//fully dynamic, connectivity algorithms. J. Exp. Algorithmics, 6, Dec. 2001
//URL:http://people.csail.mit.edu/rahul/blurbs/dyncon-jea2001.html

//given two pointers of two nodes, compute the paths from the nodes to the root
//return the smaller one as the result
bool idp_hdt::isFirstOccurSmallerThanSecondOccur(treapNode *occurA, treapNode *occurB) {
    if(occurA == nullptr || occurB == nullptr){
        return false;
    }

    if(occurA == occurB) {
        return false;
    }

    treapNode* temp_first = occurA;
    treapNode* temp_second = occurB;

    int dist_first = computeLengthOfPathToRoot(temp_first);
    vector<treapNode*>* first_sequence = computePathToRoot(dist_first, occurA);

    int dist_second = computeLengthOfPathToRoot(temp_second);
    vector<treapNode*>* second_sequence = computePathToRoot(dist_second, occurB);

    int mark = 0;
    for(int i = 0; (i < dist_first) && (i < dist_second); i++){
        if((*first_sequence)[i] == (*second_sequence)[i]){
            mark++;
        }
        else{
            break;
        }
    }

    bool flag = false;

    if(mark < dist_first){
        if((*first_sequence)[mark] == (*first_sequence)[mark]->parent->lChild){
            flag = true;
        }
    }
    else{
        if(mark < dist_second){
            if((*second_sequence)[mark] == (*second_sequence)[mark]->parent->rChild){
                flag = true;
            }
        }
    }

    vector<treapNode*>().swap((*first_sequence));
    vector<treapNode*>().swap((*second_sequence));
    delete first_sequence;
    delete second_sequence;
    return flag;
}

//compute the length from the current node occurrence to the root
int idp_hdt::computeLengthOfPathToRoot(treapNode *currentNode) {
    int length = 0;
    for(treapNode* tempNode = currentNode; tempNode->parent != nullptr;){
        tempNode = tempNode->parent;
        length++;
    }
    return length;
}

//compute the path from the current node occurrence to the root
vector<treapNode*>* idp_hdt::computePathToRoot(int pathLength, treapNode *currentNode) {
    vector<treapNode*> *pathSequence = new vector<treapNode*>((unsigned long)pathLength);
    int tempIndex = pathLength;
    for(treapNode* temp_node = currentNode; temp_node->parent != nullptr; temp_node = temp_node->parent){
        (*pathSequence)[--tempIndex] = temp_node;
    }
    return pathSequence;
}

//cut the treap at the right position of cutPosition, return treapPiece1 and treapPiece2
void idp_hdt::hdtCutTreapAtRightPosition(treapNode *&currentTreap, treapNode *&cutPosition, treapNode *&treapPiece1,
                                         treapNode *&treapPiece2) {
    if(cutPosition == nullptr){
        treapPiece1 = nullptr;
        treapPiece2 = nullptr;
        return;
    }

    //initialize a dummy node, which will be deleted later
    treapNode* dummyNode = new treapNode();
    dummyNode->parent = nullptr;
    dummyNode->lChild = nullptr;
    dummyNode->rChild = nullptr;
    dummyNode->weight = 1;
    dummyNode->actChild = false;
    dummyNode->active = false;
    dummyNode->activeOccur = false;

    //find the subtree successor of node position
    treapNode* successor;
    if(cutPosition->rChild != nullptr){
        for(successor = cutPosition->rChild; successor->lChild; successor = successor->lChild);
    }
    else{
        successor = nullptr;
    }

    //update pointers
    if(successor == nullptr){
        cutPosition->rChild = dummyNode;
        dummyNode->parent = cutPosition;
    }
    else{
        successor->lChild = dummyNode;
        dummyNode->parent = successor;
    }

    //then rotate dummy node to the root
    if(dummyNode->parent != nullptr){
        for(treapNode* p = dummyNode->parent; p; p = p->parent){
            if(p->rChild == dummyNode){
                rightRotate(p);
            }
            else if(p->lChild == dummyNode){
                leftRotate(p);
            }
        }
    }

    if(dummyNode->lChild != nullptr) {
        treapPiece1 = dummyNode->lChild;
    }
    else{
        treapPiece1 = nullptr;
    }

    if(dummyNode->rChild != nullptr){
        treapPiece2 = dummyNode->rChild;
    }
    else{
        treapPiece2 = nullptr;
    }

    if(treapPiece1 != nullptr){
        treapPiece1->parent = nullptr;
    }
    if(treapPiece2 != nullptr){
        treapPiece2->parent = nullptr;
    }

    dummyNode->lChild = nullptr;
    dummyNode->rChild = nullptr;
    delete dummyNode;
}

//cut the treap at the left position of cutPosition, return treapPiece1 and treapPiece2
void idp_hdt::hdtCutTreapAtLeftPosition(treapNode *&currentTreap, treapNode *&cutPosition, treapNode *&treapPiece1,
                                        treapNode *&treapPiece2) {
    if(cutPosition == nullptr){
        treapPiece1 = nullptr;
        treapPiece2 = nullptr;
        return;
    }

    //initialize a dummy node, which will be deleted later
    treapNode* dummyNode = new treapNode();
    dummyNode->parent = nullptr;
    dummyNode->lChild = nullptr;
    dummyNode->rChild = nullptr;
    dummyNode->weight = 1;
    dummyNode->actChild = false;
    dummyNode->active = false;
    dummyNode->activeOccur = false;

    //find the subtree pioneer of node position
    treapNode* pioneer;
    if(cutPosition->lChild != nullptr){
        for(pioneer = cutPosition->lChild; pioneer->rChild; pioneer = pioneer->rChild);
    }
    else{
        pioneer = nullptr;
    }

    if(pioneer == nullptr){
        cutPosition->lChild = dummyNode;
        dummyNode->parent = cutPosition;
    }
    else{
        pioneer->rChild = dummyNode;
        dummyNode->parent = pioneer;
    }

    //then rotate dummy node to the root
    if(dummyNode->parent != nullptr){
        for(treapNode* p = dummyNode->parent; p; p = p->parent){
            if(p->rChild == dummyNode){
                rightRotate(p);
            }
            else if(p->lChild == dummyNode){
                leftRotate(p);
            }
        }
    }

    if(dummyNode->lChild != nullptr) {
        treapPiece1 = dummyNode->lChild;
    }
    else{
        treapPiece1 = nullptr;
    }

    if(dummyNode->rChild != nullptr){
        treapPiece2 = dummyNode->rChild;
    }
    else{
        treapPiece2 = nullptr;
    }

    if(treapPiece1 != nullptr){
        treapPiece1->parent = nullptr;
    }
    if(treapPiece2 != nullptr){
        treapPiece2->parent = nullptr;
    }

    dummyNode->lChild = nullptr;
    dummyNode->rChild = nullptr;
    delete dummyNode;
}

//rotate left part
void idp_hdt::leftRotate(treapNode *&root) {

    treapNode* parent = root->parent;
    int left_right_mark = -1;
    if(parent != nullptr){
        if(parent->lChild == root){
            left_right_mark = 0;
        }
        else if(parent->rChild == root){
            left_right_mark = 1;
        }
    }

    treapNode *temp = root->lChild;
    root->lChild = temp->rChild;

    //update active info
    if(root->lChild != nullptr){
        root->lChild->parent = root;
        temp->weight -= root->lChild->weight;
        if(root->lChild->active || root->lChild->actChild){
            root->actChild = true;
        }
        else{
            if(root->rChild != nullptr){
                if(!(root->rChild->active || root->rChild->actChild)){
                    root->actChild = false;
                }
                else{
                    root->actChild = true;
                }
            }
            else{
                root->actChild = false;
            }
        }
    }
    else{
        if(root->rChild != nullptr){
            if(!(root->rChild->active || root->rChild->actChild)){
                root->actChild = false;
            }
            else{
                root->actChild = true;
            }
        }
        else{
            root->actChild = false;
        }
    }

    if(root->lChild != nullptr && root->rChild != nullptr){
        root->weight = root->lChild->weight + root->rChild->weight + 1;
    }
    else if(root->lChild != nullptr && root->rChild == nullptr){
        root->weight = root->lChild->weight + 1;
    }
    else if(root->lChild == nullptr && root->rChild != nullptr){
        root->weight = root->rChild->weight + 1;
    }
    else{
        root->weight = 1;
    }

    temp->rChild = root;

    //update the active info
    if(temp->rChild != nullptr){
        temp->rChild->parent = temp;
        if(temp->rChild->active || temp->rChild->actChild){
            temp->actChild = true;
        }
        else{
            if(temp->lChild != nullptr){
                if(!(temp->lChild->active || temp->lChild->actChild)){
                    temp->actChild = false;
                }
                else{
                    temp->actChild = true;
                }
            }
            else{
                temp->actChild = false;
            }
        }
    }
    else{
        if(temp->lChild != nullptr){
            if(!(temp->lChild->active || temp->lChild->actChild)){
                temp->actChild = false;
            }
            else{
                temp->actChild = true;
            }
        }
        else{
            temp->actChild = false;
        }
    }


    if(temp->lChild != nullptr && temp->rChild != nullptr){
        temp->weight = temp->lChild->weight + temp->rChild->weight + 1;
    }
    else if(temp->lChild != nullptr && temp->rChild == nullptr){
        temp->weight = temp->lChild->weight + 1;
    }
    else if(temp->lChild == nullptr && temp->rChild != nullptr){
        temp->weight = temp->rChild->weight + 1;
    }
    else{
        temp->weight = 1;
    }

    root = temp;

    root->parent = parent;
    if(parent != nullptr){
        if(left_right_mark == 0){
            parent->lChild = root;
        }
        else if(left_right_mark == 1){
            parent->rChild = root;
        }
    }
}

//rotate right part
void idp_hdt::rightRotate(treapNode *&root) {
    treapNode *parent = root->parent;
    int left_right_mark = -1;
    if(parent != nullptr){
        if(parent->lChild == root){
            left_right_mark = 0;
        }
        else if(parent->rChild == root){
            left_right_mark = 1;
        }
    }

    treapNode *temp = root->rChild;
    root->rChild = temp->lChild;

    if(root->rChild != nullptr){
        root->rChild->parent = root;
        if(root->rChild->active || root->rChild->actChild){
            root->actChild = true;
        }
        else{
            if(root->lChild != nullptr){
                if(!(root->lChild->active || root->lChild->actChild)){
                    root->actChild = false;
                }
                else{
                    root->actChild = true;
                }
            }
            else{
                root->actChild = false;
            }
        }
    }
    else{
        if(root->lChild != nullptr){
            if(!(root->lChild->active || root->lChild->actChild)){
                root->actChild = false;
            }
            else{
                root->actChild = true;
            }
        }
        else{
            root->actChild = false;
        }
    }

    if(root->lChild != nullptr && root->rChild != nullptr){
        root->weight = root->lChild->weight + root->rChild->weight + 1;
    }
    else if(root->lChild != nullptr && root->rChild == nullptr){
        root->weight = root->lChild->weight + 1;
    }
    else if(root->lChild == nullptr && root->rChild != nullptr){
        root->weight = root->rChild->weight + 1;
    }
    else{
        root->weight = 1;
    }

    temp->lChild = root;

    //update the active info
    if(temp->lChild != nullptr){
        temp->lChild->parent = temp;
        if(temp->lChild->active || temp->lChild->actChild){
            temp->actChild = true;
        }
        else{
            if(temp->rChild != nullptr){
                if(!(temp->rChild->active || temp->rChild->actChild)){
                    temp->actChild = false;
                }
                else{
                    temp->actChild = true;
                }
            }
            else{
                temp->actChild = false;
            }
        }
    }
    else{
        if(temp->rChild != nullptr){
            if(!(temp->rChild->active || temp->rChild->actChild)){
                temp->actChild = false;
            }
            else{
                temp->actChild = true;
            }
        }
        else{
            temp->actChild = false;
        }
    }

    if(temp->lChild != nullptr && temp->rChild != nullptr){
        temp->weight = temp->lChild->weight + temp->rChild->weight + 1;
    }
    else if(temp->lChild != nullptr && temp->rChild == nullptr){
        temp->weight = temp->lChild->weight + 1;
    }
    else if(temp->lChild == nullptr && temp->rChild != nullptr){
        temp->weight = temp->rChild->weight + 1;
    }
    else{
        temp->weight = 1;
    }

    root = temp;

    root->parent = parent;
    if(parent != nullptr){
        if(left_right_mark == 0){
            parent->lChild = root;
        }
        else if(left_right_mark == 1){
            parent->rChild = root;
        }
    }
}

//find the leftmost node in the treap
treapNode* idp_hdt::findLeftmostNode(treapNode *tree) {
    treapNode* left = nullptr;
    for(treapNode* cursor = tree; cursor != nullptr; cursor = cursor->lChild){
        left = cursor;
    }
    return left;
}

//find the rightmost ndoe in the treap

treapNode* idp_hdt::findRightmostNode(treapNode *tree) {
    treapNode* right = nullptr;
    for(treapNode* cursor = tree; cursor != nullptr; cursor = cursor->rChild){
        right = cursor;
    }
    return right;
}

//reconnect two treaps
treapNode* idp_hdt::reLinkTreaps(treapNode *treap1, treapNode *treap2, int edgeLV) {
    if(treap1 == nullptr && treap2== nullptr){
        return nullptr;
    }

    if(treap2 == nullptr && treap1 != nullptr){
        return treap1;
    }

    if(treap1 == nullptr && treap2 != nullptr){
        return treap2;
    }

    //initialize a dummy node
    treapNode* dummyNode = new treapNode();
    dummyNode->parent = nullptr;
    dummyNode->lChild = treap1;
    dummyNode->rChild = treap2;
    dummyNode->weight = treap1->weight + treap2->weight + 1;
    dummyNode->active = false;
    dummyNode->activeOccur = false;
    dummyNode->actChild = (treap1->actChild || treap2->actChild || treap1->active || treap2->active);
    treap1->parent = dummyNode;
    treap2->parent = dummyNode;

    treapNode *leftmostTreap2 = findLeftmostNode(treap2);
    treapNode *rightmostTreap1 = findRightmostNode(treap1);

    //update adjacency pointers
    unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>::iterator refIter
            = labelReference->find(rightmostTreap1->nodeLabel);
    if(refIter != labelReference->end()){
        unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator lvIter = refIter->second->find(edgeLV);
        if(lvIter != refIter->second->end()){
            unordered_map<string, unordered_set<treapNode*> *>::iterator eIter = lvIter->second->find(leftmostTreap2->nodeLabel);
            if(eIter != lvIter->second->end()){
                eIter->second->insert(leftmostTreap2);
            }
            else{
                unordered_set<treapNode*> *tempSet = new unordered_set<treapNode*>();
                tempSet->insert(leftmostTreap2);
                lvIter->second->insert(unordered_map<string, unordered_set<treapNode*> *>::value_type(leftmostTreap2->nodeLabel,tempSet));
            }
        }
        else{
            unordered_set<treapNode*> *tempSet = new unordered_set<treapNode*>();
            tempSet->insert(leftmostTreap2);
            unordered_map<string, unordered_set<treapNode*> *> *tempMap = new unordered_map<string, unordered_set<treapNode*> *>();
            tempMap->insert(unordered_map<string, unordered_set<treapNode*> *>::value_type(leftmostTreap2->nodeLabel,tempSet));
            refIter->second->insert(unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::value_type(edgeLV,tempMap));
        }
    }
    else{
        unordered_set<treapNode*> *tempSet = new unordered_set<treapNode*>();
        tempSet->insert(leftmostTreap2);
        unordered_map<string, unordered_set<treapNode*> *> *tempMap = new unordered_map<string, unordered_set<treapNode*> *>();
        tempMap->insert(unordered_map<string, unordered_set<treapNode*> *>::value_type(leftmostTreap2->nodeLabel,tempSet));
        unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *lvMap = new unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>();
        lvMap->insert(unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::value_type(edgeLV, tempMap));
        labelReference->insert(unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>::value_type(rightmostTreap1->nodeLabel, lvMap));
    }

    refIter = labelReference->find(leftmostTreap2->nodeLabel);
    if(refIter != labelReference->end()){
        unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator lvIter = refIter->second->find(edgeLV);
        if(lvIter != refIter->second->end()){
            unordered_map<string, unordered_set<treapNode*> *>::iterator eIter = lvIter->second->find(rightmostTreap1->nodeLabel);
            if(eIter != lvIter->second->end()){
                eIter->second->insert(rightmostTreap1);
            }
            else{
                unordered_set<treapNode*> *tempSet = new unordered_set<treapNode*>();
                tempSet->insert(rightmostTreap1);
                lvIter->second->insert(unordered_map<string, unordered_set<treapNode*> *>::value_type(rightmostTreap1->nodeLabel,tempSet));
            }
        }
        else{
            unordered_set<treapNode*> *tempSet = new unordered_set<treapNode*>();
            tempSet->insert(rightmostTreap1);
            unordered_map<string, unordered_set<treapNode*> *> *tempMap = new unordered_map<string, unordered_set<treapNode*> *>();
            tempMap->insert(unordered_map<string, unordered_set<treapNode*> *>::value_type(rightmostTreap1->nodeLabel,tempSet));
            refIter->second->insert(unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::value_type(edgeLV,tempMap));
        }
    }
    else{
        unordered_set<treapNode*> *tempSet = new unordered_set<treapNode*>();
        tempSet->insert(rightmostTreap1);
        unordered_map<string, unordered_set<treapNode*> *> *tempMap = new unordered_map<string, unordered_set<treapNode*> *>();
        tempMap->insert(unordered_map<string, unordered_set<treapNode*> *>::value_type(rightmostTreap1->nodeLabel,tempSet));
        unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *lvMap = new unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>();
        lvMap->insert(unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::value_type(edgeLV, tempMap));
        labelReference->insert(unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>::value_type(leftmostTreap2->nodeLabel, lvMap));
    }

    while(dummyNode->lChild != nullptr || dummyNode->rChild != nullptr){
        if(dummyNode->lChild != nullptr && dummyNode->rChild != nullptr){
            //left child and right child both exist
            if(dummyNode->lChild->priority > dummyNode->rChild->priority){
                rightRotate(dummyNode);
                dummyNode->weight -= 1;
                dummyNode = dummyNode->lChild;
            }
            else{
                leftRotate(dummyNode);
                dummyNode->weight -= 1;
                dummyNode = dummyNode->rChild;
            }
        }
        else if(dummyNode->lChild != nullptr && dummyNode->rChild == nullptr){
            leftRotate(dummyNode);
            dummyNode->weight -= 1;
            dummyNode = dummyNode->rChild;
        }
        else if(dummyNode->rChild != nullptr && dummyNode->lChild == nullptr){
            rightRotate(dummyNode);
            dummyNode->weight -= 1;
            dummyNode = dummyNode->lChild;

        }
    }

    if(dummyNode->parent->lChild == dummyNode){
        dummyNode->parent->lChild = nullptr;
        dummyNode->parent = nullptr;
    }
    else if(dummyNode->parent->rChild == dummyNode){
        dummyNode->parent->rChild = nullptr;
        dummyNode->parent = nullptr;
    }

    delete dummyNode;

    if(treap1->parent == nullptr){
        return treap1;
    }
    else{
        return treap2;
    }
}

//reroot the euler tour or treap at node u
void idp_hdt::rerooting(string u, treapNode *&tree, int edgeLV) {
    if(tree->lChild == nullptr && tree->rChild == nullptr){
        return;
    }

    if(u == findLeftmostNode(tree)->nodeLabel){
        return;
    }

    //if u is not the root of the euler tour(not starting at u)
    treapNode *uOccur = nullptr;
    unordered_map<int, unordered_map<string, treapNode*> *>::iterator lvMapIter = levelActiveOccurMap->find(edgeLV);
    if(lvMapIter != levelActiveOccurMap->end()){
        unordered_map<string, treapNode*>::iterator occurIter = lvMapIter->second->find(u);
        if(occurIter != lvMapIter->second->end()){
            uOccur = occurIter->second;
        }
    }

    treapNode *piece1 = nullptr;
    treapNode *piece2 = nullptr;
    treapNode *mergeTreap = nullptr;

    //as u is not the leftmost node in the treap, then cut the treap at the left side of the node
    hdtCutTreapAtLeftPosition(tree,uOccur,piece1,piece2);

    treapNode *rightmostOfPiece1 = findRightmostNode(piece1);
    treapNode *leftmostOfPiece2 = findLeftmostNode(piece2);

    //delete the adjacent reference
    unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>::iterator refIter
            = labelReference->find(leftmostOfPiece2->nodeLabel);
    if(refIter != labelReference->end()){
        unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = refIter->second->find(edgeLV);
        if(levelIter != refIter->second->end()){
            unordered_map<string, unordered_set<treapNode*> *>::iterator eIter = levelIter->second->find(rightmostOfPiece1->nodeLabel);
            if(eIter != levelIter->second->end()){
                unordered_set<treapNode*>::iterator setIter = eIter->second->find(rightmostOfPiece1);
                if(setIter != eIter->second->end()){
                    eIter->second->erase(setIter);
                }
            }
        }
    }

    refIter = labelReference->find(rightmostOfPiece1->nodeLabel);
    if(refIter != labelReference->end()){
        unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = refIter->second->find(edgeLV);
        if(levelIter != refIter->second->end()){
            unordered_map<string, unordered_set<treapNode*> *>::iterator eIter = levelIter->second->find(leftmostOfPiece2->nodeLabel);
            if(eIter != levelIter->second->end()){
                if(eIter->second->size() > 1){
                    unordered_set<treapNode*>::iterator setIter = eIter->second->find(leftmostOfPiece2);
                    if(setIter != eIter->second->end()){
                        eIter->second->erase(setIter);
                    }
                }
            }
        }
    }

    //delete the leftmost node in piece1
    //if the piece1 only has one node, then discard it directly, if not, then remove the leftmost node from it
    if(piece1->weight == 1){
        if(piece1->activeOccur){
            treapNode *rightmostOfPiece2 = findRightmostNode(piece2);
            rightmostOfPiece2->activeOccur = true;
            //update activeOccur info
            unordered_map<int, unordered_map<string, treapNode*> *>::iterator lvIter = levelActiveOccurMap->find(edgeLV);
            if(lvIter != levelActiveOccurMap->end()){
                unordered_map<string, treapNode*>::iterator eIter = lvIter->second->find(rightmostOfPiece2->nodeLabel);
                if(eIter != lvIter->second->end()){
                    eIter->second = rightmostOfPiece2;
                }
            }

            if(piece1->active){
                rightmostOfPiece2->active = true;
                //backtrack all the way to the root to update activeChild info
                for(treapNode* p = rightmostOfPiece2; p; p = p->parent){
                    if(p->parent != nullptr){
                        p->parent->actChild = true;
                    }
                }
            }
        }
        mergeTreap = piece2;
    }
    else{
        //piece1 contains more than 1 single node
        treapNode *subPiece1 = nullptr;
        treapNode *subPiece2 = nullptr;
        treapNode *leftmostOfPiece1 = findLeftmostNode(piece1);
        hdtCutTreapAtRightPosition(piece1,leftmostOfPiece1,subPiece1,subPiece2);

        if(subPiece1->activeOccur){
            treapNode *rightmostOfPiece2 = findRightmostNode(piece2);
            rightmostOfPiece2->activeOccur = true;
            //update activeOccur info
            unordered_map<int, unordered_map<string, treapNode*> *>::iterator lvIter = levelActiveOccurMap->find(edgeLV);
            if(lvIter != levelActiveOccurMap->end()){
                unordered_map<string, treapNode*>::iterator eIter = lvIter->second->find(rightmostOfPiece2->nodeLabel);
                if(eIter != lvIter->second->end()){
                    eIter->second = rightmostOfPiece2;
                }
            }

            if(subPiece1->active){
                rightmostOfPiece2->active = true;
                //backtrack all the way to the root to update activeChild info
                for(treapNode* p = rightmostOfPiece2; p; p = p->parent){
                    if(p->parent != nullptr){
                        p->parent->actChild = true;
                    }
                }
            }
        }

        treapNode *lmNode = findLeftmostNode(subPiece2);
        //delete the adjacent pointers
        unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>::iterator referenceIter
                = labelReference->find(subPiece1->nodeLabel);
        if(referenceIter != labelReference->end()){
            unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = referenceIter->second->find(edgeLV);
            if(levelIter != referenceIter->second->end()){
                unordered_map<string, unordered_set<treapNode*> *>::iterator eIter = levelIter->second->find(lmNode->nodeLabel);
                if(eIter != levelIter->second->end()){
                    unordered_set<treapNode*>::iterator setIter = eIter->second->find(lmNode);
                    if(setIter != eIter->second->end()){
                        eIter->second->erase(setIter);
                    }
                }
            }
        }

        referenceIter = labelReference->find(lmNode->nodeLabel);
        if(referenceIter != labelReference->end()){
            unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = referenceIter->second->find(edgeLV);
            if(levelIter != referenceIter->second->end()){
                unordered_map<string, unordered_set<treapNode*> *>::iterator eIter = levelIter->second->find(subPiece1->nodeLabel);
                if(eIter != levelIter->second->end()){
                    unordered_set<treapNode*>::iterator setIter = eIter->second->find(subPiece1);
                    if(setIter != eIter->second->end()){
                        eIter->second->erase(setIter);
                    }
                }
            }
        }

        delete subPiece1;

        //reconnect piece2 and subPiece2
        mergeTreap = reLinkTreaps(piece2,subPiece2,edgeLV);
    }

    treapNode *newNode = new treapNode();
    newNode->weight = 1;
    newNode->nodeLabel = leftmostOfPiece2->nodeLabel;
    newNode->active = false;
    newNode->activeOccur = false;
    newNode->actChild = false;
    newNode->parent = nullptr;
    newNode->number = nodeCount++;
    newNode->priority = ((rand()<<15)+rand())%200000000;
    newNode->lChild = nullptr;
    newNode->rChild = nullptr;

    tree = reLinkTreaps(mergeTreap,newNode,edgeLV);
}

//find replacement, does not use sampling method
graphEdge idp_hdt::findReplacementEdge(treapNode *smallTreap, treapNode *largeTreap, int edgeLV) {

    stack<treapNode*> *tempStack = new stack<treapNode*>();
    unordered_set<string> *visitedNode = new unordered_set<string>();
    unordered_set<graphEdge,hashEdge> *visitedEdge = new unordered_set<graphEdge,hashEdge>();

    if(smallTreap != nullptr && (smallTreap->active || smallTreap->actChild)){
        tempStack->push(smallTreap);
    }

    while(!tempStack->empty()){
        treapNode *topNode = tempStack->top();
        tempStack->pop();

        if(topNode->rChild != nullptr && (topNode->rChild->active || topNode->rChild->actChild)){
            tempStack->push(topNode->rChild);
        }

        if(topNode->lChild != nullptr && (topNode->lChild->active || topNode->lChild->actChild)){
            tempStack->push(topNode->lChild);
        }

        if(topNode->active){
            unordered_set<string>::iterator visitedIter = visitedNode->find(topNode->nodeLabel);
            if(visitedIter != visitedNode->end()){
                continue;
            }

            visitedNode->insert(topNode->nodeLabel);

            unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*>::iterator endpIter = nonTreeEdge->find(topNode->nodeLabel);
            if(endpIter != nonTreeEdge->end()){
                unordered_map<int, unordered_set<graphEdge, hashEdge>*>::iterator lvIter = endpIter->second->find(edgeLV);
                if(lvIter != endpIter->second->end()){
                    for(unordered_set<graphEdge,hashEdge>::iterator edgeIter = lvIter->second->begin(); edgeIter != lvIter->second->end();){
                        graphEdge foundEdge = *edgeIter;

                        unordered_set<graphEdge,hashEdge>::iterator edgeVisIter = visitedEdge->find(foundEdge);
                        if(edgeVisIter != visitedEdge->end()){
                            edgeIter++;
                            continue;
                        }

                        visitedEdge->insert(foundEdge);

                        treapNode *root1 = nullptr;
                        treapNode *root2 = nullptr;

                        unordered_map<int, unordered_map<string, treapNode*> *>::iterator lvMapIter = levelActiveOccurMap->find(edgeLV);
                        if(lvMapIter != levelActiveOccurMap->end()){
                            unordered_map<string, treapNode*>::iterator occurIter = lvMapIter->second->find(foundEdge.endPointer1);
                            if(occurIter != lvMapIter->second->end()){
                                root1 = hdtFindRoot(occurIter->second);
                            }

                            occurIter = lvMapIter->second->find(foundEdge.endPointer2);
                            if(occurIter != lvMapIter->second->end()){
                                root2 = hdtFindRoot(occurIter->second);
                            }
                        }

                        if((smallTreap == root1 && largeTreap == root2) || (smallTreap == root2 && largeTreap == root1)){

                            //found a replacement edge
                            unordered_map<graphEdge, int, hashEdge>::iterator elistIter = edgeList->find(foundEdge);
                            if(elistIter != edgeList->end()){
                                if(elistIter->second == edgeLV){

                                    treeEdge->insert(unordered_map<graphEdge,int,hashEdge>::value_type(foundEdge,edgeLV));

                                    unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*>::iterator subendpIter = nonTreeEdge->find(foundEdge.endPointer1);
                                    if(subendpIter != nonTreeEdge->end()){
                                        unordered_map<int, unordered_set<graphEdge, hashEdge>*>::iterator sublvIter = subendpIter->second->find(edgeLV);
                                        if(sublvIter != subendpIter->second->end()){
                                            unordered_set<graphEdge, hashEdge>::iterator nonedgeIter = sublvIter->second->find(foundEdge);
                                            if(nonedgeIter != sublvIter->second->end()){
                                                sublvIter->second->erase(nonedgeIter);
                                            }
                                        }
                                    }

                                    subendpIter = nonTreeEdge->find(foundEdge.endPointer2);
                                    if(subendpIter != nonTreeEdge->end()){
                                        unordered_map<int, unordered_set<graphEdge, hashEdge>*>::iterator sublvIter = subendpIter->second->find(edgeLV);
                                        if(sublvIter != subendpIter->second->end()){
                                            unordered_set<graphEdge, hashEdge>::iterator nonedgeIter = sublvIter->second->find(foundEdge);
                                            if(nonedgeIter != sublvIter->second->end()){
                                                sublvIter->second->erase(nonedgeIter);
                                            }
                                        }
                                    }
                                }
                            }

                            return foundEdge;
                        }

                        if(edgeLV < logN && smallTreap == root1 && smallTreap == root2){

                            //delete from the current level
                            edgeIter = lvIter->second->erase(edgeIter);
                            //two endpoints are in the small treap
                            //promote it up to level i + 1 if edgeLV is smaller than logN
                            unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*>::iterator subendpIter = nonTreeEdge->find(foundEdge.endPointer1);
                            if(subendpIter != nonTreeEdge->end()){
                                unordered_map<int, unordered_set<graphEdge, hashEdge>*>::iterator sublvIter = subendpIter->second->find(edgeLV+1);
                                if(sublvIter != subendpIter->second->end()){
                                    sublvIter->second->insert(foundEdge);
                                }
                                else{
                                    unordered_set<graphEdge,hashEdge> *tempSet = new unordered_set<graphEdge,hashEdge>();
                                    tempSet->insert(foundEdge);
                                    subendpIter->second->insert(unordered_map<int,unordered_set<graphEdge,hashEdge>*>::value_type(edgeLV+1,tempSet));
                                }

                                sublvIter = subendpIter->second->find(edgeLV);
                                if(sublvIter != subendpIter->second->end()){
                                    unordered_set<graphEdge,hashEdge>::iterator neIter = sublvIter->second->find(foundEdge);
                                    if(neIter != sublvIter->second->end()){
                                        sublvIter->second->erase(neIter);
                                    }
                                }
                            }

                            subendpIter = nonTreeEdge->find(foundEdge.endPointer2);
                            if(subendpIter != nonTreeEdge->end()){
                                unordered_map<int, unordered_set<graphEdge, hashEdge>*>::iterator sublvIter = subendpIter->second->find(edgeLV+1);
                                if(sublvIter != subendpIter->second->end()){
                                    sublvIter->second->insert(foundEdge);
                                }
                                else{
                                    unordered_set<graphEdge,hashEdge> *tempSet = new unordered_set<graphEdge,hashEdge>();
                                    tempSet->insert(foundEdge);
                                    subendpIter->second->insert(unordered_map<int,unordered_set<graphEdge,hashEdge>*>::value_type(edgeLV+1,tempSet));
                                }

                                sublvIter = subendpIter->second->find(edgeLV);
                                if(sublvIter != subendpIter->second->end()){
                                    unordered_set<graphEdge,hashEdge>::iterator neIter = sublvIter->second->find(foundEdge);
                                    if(neIter != sublvIter->second->end()){
                                        sublvIter->second->erase(neIter);
                                    }
                                }
                            }

                            unordered_map<graphEdge, int, hashEdge>::iterator elistIter = edgeList->find(foundEdge);
                            if(elistIter != edgeList->end()){
                                if(elistIter->second == edgeLV){
                                    elistIter->second += 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    delete tempStack;
    delete visitedNode;
    delete visitedEdge;

    graphEdge returnEdge(" ", " ");
    return returnEdge;
}

//print out treap
void idp_hdt::printTreap(treapNode *treapRoot) {
    if(treapRoot != nullptr) {
        printTreap(treapRoot->lChild);
        cout << treapRoot << " " << "number:" << treapRoot->number << " value:" << treapRoot->nodeLabel << " priority:";
        cout << treapRoot->priority << " active:" <<treapRoot->active << " weight:" << treapRoot->weight << " isFirstOccur: " << treapRoot->activeOccur;
        if(treapRoot->parent != nullptr) {
            cout << " parent:" << treapRoot->parent->number;
        }
        cout << " active child: " << treapRoot->actChild;
        if(treapRoot->parent != nullptr){
            cout << " parent: " << treapRoot->parent->nodeLabel;
        }
        cout << endl;
        printTreap(treapRoot->rChild);
    }
}

//destructor
idp_hdt::~idp_hdt() {
    stack<treapNode*> *tempStack = new stack<treapNode*>();
    for(unordered_map<int, unordered_set<treapNode*> *>::iterator lvTreapIter = levelTreapMap->begin(); lvTreapIter != levelTreapMap->end();
            lvTreapIter++){
        for(unordered_set<treapNode*>::iterator it = lvTreapIter->second->begin(); it != lvTreapIter->second->end(); it++){
            if(*it != nullptr){
                tempStack->push(*it);
            }

            while(!tempStack->empty()){
                treapNode* p = tempStack->top();
                tempStack->pop();

                if(p->lChild != nullptr){
                    tempStack->push(p->lChild);
                }

                if(p->rChild != nullptr){
                    tempStack->push(p->rChild);
                }

                delete p;
                p = nullptr;
            }
        }
        delete lvTreapIter->second;
    }
    delete levelTreapMap;
    delete tempStack;

    for(unordered_map<int, unordered_map<string, treapNode*> *>::iterator lvMapIter = levelActiveOccurMap->begin();
            lvMapIter != levelActiveOccurMap->end(); lvMapIter++){
        delete lvMapIter->second;
    }
    delete levelActiveOccurMap;

    delete edgeList;
    delete treeEdge;

    for(unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*>::iterator nonTreeIter = nonTreeEdge->begin();
            nonTreeIter != nonTreeEdge->end(); nonTreeIter++){
        for(unordered_map<int, unordered_set<graphEdge, hashEdge>*>::iterator lvIter = nonTreeIter->second->begin();
                lvIter != nonTreeIter->second->end(); lvIter++){
            delete lvIter->second;
        }
        delete nonTreeIter->second;
    }
    delete nonTreeEdge;

    for(unordered_map<string, unordered_set<string>*>::iterator adjIter = adjacencyList->begin(); adjIter != adjacencyList->end();
            adjIter++){
        delete adjIter->second;
    }
    delete adjacencyList;

    for(unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>::iterator labelIter =
            labelReference->begin(); labelIter != labelReference->end(); labelIter++){
        for(unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = labelIter->second->begin();
            levelIter != labelIter->second->end(); levelIter++){
            for(unordered_map<string, unordered_set<treapNode*> *>::iterator subIter = levelIter->second->begin();
                subIter != levelIter->second->end(); subIter++){
                delete subIter->second;
            }
            delete levelIter->second;
        }
        delete labelIter->second;
    }
    delete labelReference;

    delete treapVector;
}