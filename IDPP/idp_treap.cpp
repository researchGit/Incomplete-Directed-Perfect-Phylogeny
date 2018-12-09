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

#include "idp_treap.h"

using namespace std;

//insert a node in the treap;
void idp_treap::insertNode(unordered_map<int, list<string>*> *eulerTourSequenceLists,
                           unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*> *nonTreeEdgesList) {

    for(unordered_map<int, list<string>*>::iterator seqListIter = eulerTourSequenceLists->begin();
            seqListIter != eulerTourSequenceLists->end(); seqListIter++){
        treapNode *treapRoot = nullptr;

        unordered_map<string, int> *labelCount = new unordered_map<string, int>();
        lastVisitedPointer = nullptr;

        for(list<string>::iterator listIter = seqListIter->second->begin(); listIter != seqListIter->second->end();
            listIter++){
            unordered_map<string, int>::iterator lcIter = labelCount->find(*listIter);
            if(lcIter != labelCount->end()){
                //indicate that the current one is not the first occurrence of the node
                lcIter->second += 1;
                insert(countOfNode++, *listIter, treapRoot, false, false, treapRoot);
            }
            else{
                labelCount->insert(unordered_map<string, int>::value_type(*listIter, 1));
                unordered_map<string, unordered_map<int, unordered_set<graphEdge, hashEdge>*>*>::iterator edgeIter =
                        nonTreeEdgesList->find(*listIter);
                if(edgeIter != nonTreeEdgesList->end()){
                    unordered_map<int, unordered_set<graphEdge, hashEdge>*>::iterator levelIter = edgeIter->second->find(0);
                    if(levelIter != edgeIter->second->end()){
                        if(levelIter->second->size() > 0){
                            insert(countOfNode++, *listIter, treapRoot, true, true, treapRoot);
                        }
                        else{
                            insert(countOfNode++, *listIter, treapRoot, false, true, treapRoot);
                        }
                    }
                }
                else{
                    insert(countOfNode++, *listIter, treapRoot, false, true, treapRoot);
                }
            }

        }

        //push this treap into treapList
        if(treapRoot != nullptr){
            treapList->insert(treapRoot);
        }
    }
}

//process the node in the treap
//applied the method posted on website: http://blog.ruofeidu.com/treap-in-45-lines-of-c/ to generate the random number for priority
void idp_treap::insert(int number, string label, treapNode *&root, bool active, bool occur, treapNode *parentNode) {
    if(root == nullptr){
        root = new treapNode(number,label,active);
        root->lChild = nullptr;
        root->rChild = nullptr;
        root->parent = parentNode;
        root->activeOccur = occur;

        //push into activeOccur list
        if(occur){
            activeOccur->insert(unordered_map<string, treapNode*>::value_type(label, root));
        }

        if(lastVisitedPointer == nullptr){
            //for the first one
            lastVisitedPointer = root;
        }
        else{
            //create the reference map
            unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>::iterator labelIter =
                    levelLabelReferenceMap->find(label);
            if(labelIter != levelLabelReferenceMap->end()){
                unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = labelIter->second->find(0);
                if(levelIter != labelIter->second->end()){
                    //find it
                    unordered_map<string, unordered_set<treapNode*> *>::iterator subIter = levelIter->second->find(lastVisitedPointer->nodeLabel);
                    if(subIter != levelIter->second->end()){
                        subIter->second->insert(lastVisitedPointer);
                    }
                    else{
                        unordered_set<treapNode*> *tempSet = new unordered_set<treapNode*>();
                        tempSet->insert(lastVisitedPointer);
                        levelIter->second->insert(unordered_map<string, unordered_set<treapNode*> *>::value_type(lastVisitedPointer->nodeLabel,tempSet));
                    }
                }
                else{
                    unordered_set<treapNode*> *tempSet = new unordered_set<treapNode*>();
                    tempSet->insert(lastVisitedPointer);
                    unordered_map<string, unordered_set<treapNode*> *> *tempLabelOccurMap =
                            new unordered_map<string, unordered_set<treapNode*> *>();
                    tempLabelOccurMap->insert(unordered_map<string, unordered_set<treapNode*> *>::value_type(lastVisitedPointer->nodeLabel, tempSet));
                    labelIter->second->insert(unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::value_type(0, tempLabelOccurMap));
                }
            }
            else{
                unordered_set<treapNode*> *tempSet = new unordered_set<treapNode*>();
                tempSet->insert(lastVisitedPointer);
                unordered_map<string, unordered_set<treapNode*> *> *tempLabelOccurMap =
                        new unordered_map<string, unordered_set<treapNode*> *>();
                tempLabelOccurMap->insert(unordered_map<string, unordered_set<treapNode*> *>::value_type(lastVisitedPointer->nodeLabel, tempSet));
                unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *tempLevelLabel =
                        new unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>();
                tempLevelLabel->insert(unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::value_type(0, tempLabelOccurMap));
                levelLabelReferenceMap->insert(unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>::value_type(label,tempLevelLabel));
            }

            labelIter = levelLabelReferenceMap->find(lastVisitedPointer->nodeLabel);
            if(labelIter != levelLabelReferenceMap->end()){
                unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = labelIter->second->find(0);
                if(levelIter != labelIter->second->end()){
                    //find it
                    unordered_map<string, unordered_set<treapNode*> *>::iterator subIter = levelIter->second->find(label);
                    if(subIter != levelIter->second->end()){
                        subIter->second->insert(root);
                    }
                    else{
                        unordered_set<treapNode*> *tempSet = new unordered_set<treapNode*>();
                        tempSet->insert(root);
                        levelIter->second->insert(unordered_map<string, unordered_set<treapNode*>*>::value_type(label,tempSet));
                    }
                }
                else{
                    unordered_set<treapNode*> *tempSet = new unordered_set<treapNode*>();
                    tempSet->insert(root);
                    unordered_map<string, unordered_set<treapNode*> *> *tempLabelOccurMap =
                            new unordered_map<string, unordered_set<treapNode*> *>();
                    tempLabelOccurMap->insert(unordered_map<string, unordered_set<treapNode*> *>::value_type(label, tempSet));
                    labelIter->second->insert(unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::value_type(0, tempLabelOccurMap));
                }
            }
            else{
                unordered_set<treapNode*> *tempSet = new unordered_set<treapNode*>();
                tempSet->insert(root);
                unordered_map<string, unordered_set<treapNode*> *> *tempLabelOccurMap =
                        new unordered_map<string, unordered_set<treapNode*> *>();
                tempLabelOccurMap->insert(unordered_map<string, unordered_set<treapNode*> *>::value_type(label, tempSet));
                unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *tempLevelLabel =
                        new unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>();
                tempLevelLabel->insert(unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::value_type(0, tempLabelOccurMap));
                levelLabelReferenceMap->insert(unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>::value_type(lastVisitedPointer->nodeLabel,tempLevelLabel));
            }
            lastVisitedPointer = root;
        }
    }
    else if(number < root->number){
        root->weight++;
        insert(number, label, root->lChild, active, occur, root);
        if(root->lChild->priority < root->priority){
            leftRotate(root);
        }
        if(root->lChild != NULL){
            if(root->lChild->active || root->lChild->actChild){
                root->actChild = true;
            }
        }
        if(root->rChild != NULL){
            if(root->rChild->active || root->rChild->actChild){
                root->actChild = true;
            }
        }
    }
    else{
        root->weight++;
        insert(number, label, root->rChild, active, occur, root);
        if(root->rChild->priority < root->priority){
            rightRotate(root);
        }
        if(root->lChild != NULL){
            if(root->lChild->active || root->lChild->actChild){
                root->actChild = true;
            }
        }
        if(root->rChild != NULL){
            if(root->rChild->active || root->rChild->actChild){
                root->actChild = true;
            }
        }
    }
}

//rotate left part
void idp_treap::leftRotate(treapNode *&root) {

    treapNode* parent = root->parent;
    int left_right_mark = -1;
    if(parent != NULL){
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
    if(root->lChild != NULL){
        root->lChild->parent = root;
        temp->weight -= root->lChild->weight;
        if(root->lChild->active || root->lChild->actChild){
            root->actChild = true;
        }
        else{
            if(root->rChild != NULL){
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
        if(root->rChild != NULL){
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

    if(root->lChild != NULL && root->rChild != NULL){
        root->weight = root->lChild->weight + root->rChild->weight + 1;
    }
    else if(root->lChild != NULL && root->rChild == NULL){
        root->weight = root->lChild->weight + 1;
    }
    else if(root->lChild == NULL && root->rChild != NULL){
        root->weight = root->rChild->weight + 1;
    }
    else{
        root->weight = 1;
    }

    temp->rChild = root;

    //update the active info
    if(temp->rChild != NULL){
        temp->rChild->parent = temp;
        if(temp->rChild->active || temp->rChild->actChild){
            temp->actChild = true;
        }
        else{
            if(temp->lChild != NULL){
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
        if(temp->lChild != NULL){
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


    if(temp->lChild != NULL && temp->rChild != NULL){
        temp->weight = temp->lChild->weight + temp->rChild->weight + 1;
    }
    else if(temp->lChild != NULL && temp->rChild == NULL){
        temp->weight = temp->lChild->weight + 1;
    }
    else if(temp->lChild == NULL && temp->rChild != NULL){
        temp->weight = temp->rChild->weight + 1;
    }
    else{
        temp->weight = 1;
    }

    root = temp;

    root->parent = parent;
    if(parent != NULL){
        if(left_right_mark == 0){
            parent->lChild = root;
        }
        else if(left_right_mark == 1){
            parent->rChild = root;
        }
    }
}

//rotate right part
void idp_treap::rightRotate(treapNode *&root) {
    treapNode *parent = root->parent;
    int left_right_mark = -1;
    if(parent != NULL){
        if(parent->lChild == root){
            left_right_mark = 0;
        }
        else if(parent->rChild == root){
            left_right_mark = 1;
        }
    }

    treapNode *temp = root->rChild;
    root->rChild = temp->lChild;

    if(root->rChild != NULL){
        root->rChild->parent = root;
        if(root->rChild->active || root->rChild->actChild){
            root->actChild = true;
        }
        else{
            if(root->lChild != NULL){
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
        if(root->lChild != NULL){
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

    if(root->lChild != NULL && root->rChild != NULL){
        root->weight = root->lChild->weight + root->rChild->weight + 1;
    }
    else if(root->lChild != NULL && root->rChild == NULL){
        root->weight = root->lChild->weight + 1;
    }
    else if(root->lChild == NULL && root->rChild != NULL){
        root->weight = root->rChild->weight + 1;
    }
    else{
        root->weight = 1;
    }

    temp->lChild = root;

    //update the active info
    if(temp->lChild != NULL){
        temp->lChild->parent = temp;
        if(temp->lChild->active || temp->lChild->actChild){
            temp->actChild = true;
        }
        else{
            if(temp->rChild != NULL){
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
        if(temp->rChild != NULL){
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

    if(temp->lChild != NULL && temp->rChild != NULL){
        temp->weight = temp->lChild->weight + temp->rChild->weight + 1;
    }
    else if(temp->lChild != NULL && temp->rChild == NULL){
        temp->weight = temp->lChild->weight + 1;
    }
    else if(temp->lChild == NULL && temp->rChild != NULL){
        temp->weight = temp->rChild->weight + 1;
    }
    else{
        temp->weight = 1;
    }

    root = temp;

    root->parent = parent;
    if(parent != NULL){
        if(left_right_mark == 0){
            parent->lChild = root;
        }
        else if(left_right_mark == 1){
            parent->rChild = root;
        }
    }
}

//print the treap
void idp_treap::printTreap(treapNode *treapRoot) {
    if(treapRoot != nullptr) {
        printTreap(treapRoot->lChild);
        cout << treapRoot << " " << "number:" << treapRoot->number << " value:" << treapRoot->nodeLabel << " priority:";
        cout << treapRoot->priority << " active:" <<treapRoot->active << " weight:" << treapRoot->weight << " isFirstOccur: " << treapRoot->activeOccur;
        if(treapRoot->parent != NULL) {
            cout << " parent:" << treapRoot->parent->number;
        }
        cout << " active child: " << treapRoot->actChild;
        if(treapRoot->parent != NULL){
            cout << " parent: " << treapRoot->parent->nodeLabel;
        }
        cout << endl;
        printTreap(treapRoot->rChild);
    }
}

//print the levelLabelReferenceMap
void idp_treap::printLevelLabelReferenceMap() {
    for(unordered_map<string, unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *> *>::iterator
    labelIter = levelLabelReferenceMap->begin(); labelIter != levelLabelReferenceMap->end(); labelIter++){
        cout << "label " << labelIter->first << " ";
        for(unordered_map<int, unordered_map<string, unordered_set<treapNode*> *> *>::iterator levelIter = labelIter->second->begin();
                levelIter != labelIter->second->end(); levelIter++){
            cout << "at level " << levelIter->first << ": ";
            for(unordered_map<string, unordered_set<treapNode*> *>::iterator subIter = levelIter->second->begin();
                    subIter != levelIter->second->end(); subIter++){
                cout << subIter->first << " -- ";
                for(unordered_set<treapNode*>::iterator setIter = subIter->second->begin(); setIter != subIter->second->end(); setIter++){
                    cout << *setIter << " ";
                }
            }
        }
        cout << endl;
    }
}

//print out active oocurrence
void idp_treap::printActiveOccur() {
    cout << "active occurrence:" << activeOccur->size() << endl;
    for(unordered_map<string, treapNode*>::iterator occurIter = activeOccur->begin(); occurIter != activeOccur->end(); occurIter++){
        cout << "\t" << occurIter->first << "," << occurIter->second << endl;
    }
}

//destructor
idp_treap::~idp_treap() {
    lastVisitedPointer = nullptr;
    delete lastVisitedPointer;
}