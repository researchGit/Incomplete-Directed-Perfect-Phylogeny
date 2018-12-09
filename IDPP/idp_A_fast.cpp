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

#include "idp_A_fast.h"
#include "idp_matrixTransform.h"
#include "idp_depthFirstTraversal.h"
#include "idp_treap.h"
#include "idp_hdt.h"

using namespace std;

//custom constructor
idp_A_fast::idp_A_fast(unordered_map<int, fancyKStructure *> *fancyKList, unordered_set<graphEdge, hashEdge> *E0,
                       unordered_set<graphEdge, hashEdge> *E1, unordered_set<graphEdge, hashEdge> *Eq) {
    //initialization, step 1(a)

    //set t <- 0
    t = 0;

    //set fancy K^0 <- {S U C}
    fancyK = fancyKList;

    //set G <- G(A) (there is no need to have another graph structure
    eZero = E0;
    eOne = E1;
    eQuestion = Eq;

    //T ←triv(S).
    unordered_map<int, fancyKStructure*>::iterator fancyKIter = fancyK->find(t);
    if(fancyKIter != fancyK->end()){
        for(unordered_map<string, fancyKSubStructure*>::iterator stfIter = fancyKIter->second->speciesToFancyK->begin();
                stfIter != fancyKIter->second->speciesToFancyK->end(); stfIter++){
            outputTree *outputNode = new outputTree(stfIter->first, outputRoot);
            outputRoot->outputChildren->insert(outputNode);
            labelToNode->insert(unordered_map<string, outputTree*>::value_type(stfIter->first, outputNode));
        }
    }
    outputSet->insert(outputRoot);
}

//initialization step of IDP algorithm
void idp_A_fast::idpProcessing(char** matrix, int columnSize, int rowSize, bool enablePromotion) {

    unordered_map<int, fancyKStructure*>::iterator ksIter = fancyK->find(t);
    if(ksIter != fancyK->end()){
        //initialization, step 1(b)

        //for each character c, and i in {1,?} do:
        //Set d^i_c <- |{s in S|(s,c) in E_i}|
        unordered_map<string, fancyKSubStructure*> *tempChars = ksIter->second->charToFancyK;
        unordered_map<string, fancyKSubStructure*> *tempSpecies = ksIter->second->speciesToFancyK;

        for(unordered_map<string, fancyKSubStructure*>::iterator charIter = tempChars->begin();
                charIter != tempChars->end(); charIter++){
            string charStr = charIter->first;
            for(unordered_map<string, fancyKSubStructure*>::iterator specIter = tempSpecies->begin();
                    specIter != tempSpecies->end(); specIter++){
                string specStr = specIter->first;
                graphEdge tempEdge;
                if(charStr < specStr){
                    tempEdge.endPointer1 = charStr;
                    tempEdge.endPointer2 = specStr;
                }
                else{
                    tempEdge.endPointer1 = specStr;
                    tempEdge.endPointer2 = charStr;
                }
                unordered_set<graphEdge,hashEdge>::iterator edgeIter = eOne->find(tempEdge);
                if(edgeIter != eOne->end()){
                    //indicate it is in E1
                    unordered_map<string, int>::iterator charNumIter = dcOne->find(charStr);
                    if(charNumIter != dcOne->end()){
                        //increase the count directly
                        charNumIter->second += 1;
                    }
                    else{
                        dcOne->insert(unordered_map<string, int>::value_type(charStr,1));
                    }
                }

                edgeIter = eQuestion->find(tempEdge);
                if(edgeIter != eQuestion->end()){
                    //indicate it is in E?
                    unordered_map<string, int>::iterator charNumIter = dcQuestion->find(charStr);
                    if(charNumIter != dcQuestion->end()){
                        //increase the count directly
                        charNumIter->second += 1;
                    }
                    else{
                        dcQuestion->insert(unordered_map<string, int>::value_type(charStr,1));
                    }
                }
            }
        }

        //initialization, step 1(c)
        //remove all S-semiuniversal and all null characters from G.
        unordered_set<string> *sSemiAndNull = new unordered_set<string>();
        int sizeS = rowSize;
        for(unordered_map<string, fancyKSubStructure*>::iterator charIter = tempChars->begin();
            charIter != tempChars->end(); charIter++){
            int sizeOne = 0;
            int sizeQuestion = 0;
            string charStr = charIter->first;
            unordered_map<string, int>::iterator strCountIter = dcOne->find(charStr);
            if(strCountIter != dcOne->end()){
                sizeOne = strCountIter->second;
            }
            strCountIter = dcQuestion->find(charIter->first);
            if(strCountIter != dcOne->end()){
                sizeQuestion = strCountIter->second;
            }
            if(sizeOne != 0){
                if((sizeOne+sizeQuestion) == sizeS){
                    //indicate that this character is S-semiuniversal
                    sSemiAndNull->insert(charStr);
                    outputRoot->label += charStr + "+";
                }
            }
            else{
                //indicate that this character is null character
                sSemiAndNull->insert(charStr);
                outputTree *outputNode = new outputTree(charStr);
                outputSet->insert(outputNode);
            }
        }
        outputRoot->label = outputRoot->label.substr(0, outputRoot->label.length() - 1);

        //initialization, step 1(d)
        //initialize a data structure for maintaining the connected components of G.

        idp_matrixTransform matrixTransform;
        unordered_map<string, unordered_set<string>*> *adjacentList = matrixTransform.constructAdjacencyList(matrix, columnSize, rowSize, sSemiAndNull);

        idp_depthFirstTraversal depthFirstTraversal;
        depthFirstTraversal.iterativeDepthFirstSearch(adjacentList);

        unordered_map<int,list<string> *> *eulerTourSequence = depthFirstTraversal.getEulerTourSequenceLists();

        //build up treaps
        idp_treap treap;
        treap.insertNode(eulerTourSequence, depthFirstTraversal.getNonTreeEdgesList());

        unordered_map<graphEdge, int, hashEdge> *allEdges = depthFirstTraversal.getAllEdges();

        idp_hdt hdt(columnSize + rowSize, treap.getTreapList(), treap.getActiveOccur(), enablePromotion, allEdges,
                depthFirstTraversal.getTreeEdges(), depthFirstTraversal.getNonTreeEdgesList(), adjacentList,
                treap.getLevelLabelReferenceMap(), treap.getCountOfNode());

        //While E(G) is not ∅ do:
        while(!allEdges->empty()){
            //step 2(a)
            //increment t;
            t += 1;

            //step 2(b)
            //explicitly compute the set K^t of connected components K1, . . . , Kr of G.
            unordered_map<int, unordered_set<treapNode*> *> *levelTreapMap = hdt.getLevelTreapMap();
            unordered_set<treapNode*> *treaps = new unordered_set<treapNode*>();
            unordered_map<int, unordered_set<treapNode*>*>::iterator ltmIter = levelTreapMap->find(0);
            if(ltmIter != levelTreapMap->end()){
                treaps = ltmIter->second;
            }

            if(treaps != nullptr){
                fancyKStructure *subFancyK = new fancyKStructure();
                stack<treapNode*> *tempStack = new stack<treapNode*>();
                unordered_set<string> *visitedNode = new unordered_set<string>();
                for(unordered_set<treapNode*>::iterator treapIter = treaps->begin(); treapIter != treaps->end();
                        treapIter++){
                    treapNode *tRoot = *treapIter;
                    fancyKSubStructure *objOfSubStructure = new fancyKSubStructure();

                    if(tRoot != nullptr && tRoot->weight > 1){
                        tempStack->push(tRoot);
                        subFancyK->subStructureList->insert(objOfSubStructure);
                    }


                    while(!tempStack->empty()){
                        treapNode *tp = tempStack->top();
                        tempStack->pop();

                        if(tp->lChild != nullptr){
                            tempStack->push(tp->lChild);
                        }

                        if(tp->rChild != nullptr){
                            tempStack->push(tp->rChild);
                        }

                        unordered_set<string>::iterator visIter = visitedNode->find(tp->nodeLabel);
                        if(visIter != visitedNode->end()){
                            continue;
                        }
                        visitedNode->insert(tp->nodeLabel);

                        if(tp->nodeLabel[0] == 'c'){
                            string charStr = tp->nodeLabel;
                            unordered_map<string, fancyKSubStructure*>::iterator ltsIter = subFancyK->charToFancyK->find(charStr);
                            if(ltsIter == subFancyK->charToFancyK->end()){
                                objOfSubStructure->setC->insert(charStr);
                                subFancyK->charToFancyK->insert(unordered_map<string, fancyKSubStructure*>::value_type(charStr, objOfSubStructure));
                            }
                        }
                        else if(tp->nodeLabel[0] == 's'){
                            string speciesStr = tp->nodeLabel;
                            unordered_map<string, fancyKSubStructure*>::iterator ltsIter = subFancyK->speciesToFancyK->find(speciesStr);
                            if(ltsIter == subFancyK->speciesToFancyK->end()){
                                objOfSubStructure->setS->insert(speciesStr);
                                subFancyK->speciesToFancyK->insert(unordered_map<string, fancyKSubStructure*>::value_type(speciesStr, objOfSubStructure));
                            }
                        }
                    }
                }

                fancyK->insert(unordered_map<int, fancyKStructure*>::value_type(t, subFancyK));
            }


            //step 2(c)
            //For each connected component Ki ∈ K^t such that |E(Ki)| ≥ 1 do:
            unordered_map<int, fancyKStructure*>::iterator lvtIter = fancyK->find(t);
            if(lvtIter != fancyK->end()){
                fancyKStructure *subFancyK = lvtIter->second;
                for(unordered_set<fancyKSubStructure*>::iterator subsIter = subFancyK->subStructureList->begin();
                    subsIter != subFancyK->subStructureList->end(); subsIter++){
                    fancyKSubStructure *substruct = *subsIter;
                    if(substruct->setC->size() > 0 && substruct->setS->size() > 0){
                        //create a new tree node
                        outputTree* newOutputNode = new outputTree();
                        //step 2(c)i
                        //i. Pick any character c' ∈ C(Ki).
                        string pickCharStr = *(substruct->setC->begin());

                        //step 2(c)ii
                        //ii. Compute S = S(K) \ S(Ki), where K is the component in K^(t−1) which contains c.
                        unordered_map<int, fancyKStructure*>::iterator lvtminusIter = fancyK->find(t - 1);
                        if(lvtminusIter != fancyK->end()){
                            fancyKStructure *tminus1subFancyK = lvtminusIter->second;
                            unordered_set<string> *ski = substruct->setS;

                            unordered_set<string> *sk = new unordered_set<string>();
                            unordered_map<string, fancyKSubStructure*>::iterator tminus1specIter = tminus1subFancyK->charToFancyK->find(pickCharStr);
                            if(tminus1specIter != tminus1subFancyK->charToFancyK->end()){
                                sk = tminus1specIter->second->setS;
                            }
                            for(unordered_set<string>::iterator skIter = sk->begin(); skIter != sk->end(); skIter++){
                                unordered_set<string>::iterator skiIter = ski->find(*skIter);
                                if(skiIter == ski->end()){
                                    //step 2(c)iii
                                    //iii. For each species-character pair (s, c) ∈ S × C(Ki) do:
                                    //If (s, c) ∈ E? then decrement d^?_c.
                                    for(unordered_set<string>::iterator ckiIter = substruct->setC->begin(); ckiIter !=
                                            substruct->setC->end(); ckiIter++){
                                        graphEdge edgePair;
                                        if(*skIter < *ckiIter){
                                            edgePair.endPointer1 = *skIter;
                                            edgePair.endPointer2 = *ckiIter;
                                        }
                                        else{
                                            edgePair.endPointer1 = *ckiIter;
                                            edgePair.endPointer2 = *skIter;
                                        }
                                        unordered_set<graphEdge, hashEdge>::iterator epIter = eQuestion->find(edgePair);
                                        if(epIter != eQuestion->end()){
                                            unordered_map<string, int>::iterator strCountIter = dcQuestion->find(*ckiIter);
                                            if(strCountIter != dcQuestion->end()){
                                                strCountIter->second -= 1;
                                            }
                                        }
                                    }
                                }
                            }

                            //step 2(c)iv
                            //iv. Compute the set U of all characters in Ki which are S(Ki)-semiuniversal in A.
                            unordered_set<string> *skiSemiuniversal = new unordered_set<string>();
                            for(unordered_set<string>::iterator charIter = substruct->setC->begin(); charIter !=
                                    substruct->setC->end(); charIter++){
                                string subCharStr = *charIter;
                                int sizeOfSki = (int)substruct->setS->size();
                                int sizeOneAndQuestion = 0;
                                unordered_map<string, int>::iterator dcIter = dcOne->find(subCharStr);
                                if(dcIter != dcOne->end()){
                                    sizeOneAndQuestion += dcIter->second;
                                }

                                dcIter = dcQuestion->find(subCharStr);
                                if(dcIter != dcQuestion->end()){
                                    sizeOneAndQuestion += dcIter->second;
                                }

                                if(sizeOfSki == sizeOneAndQuestion){
                                    skiSemiuniversal->insert(subCharStr);
                                    newOutputNode->label += subCharStr + "+";
                                }
                            }

                            //step 2(c)v
                            //v. If U = ∅, then output False and halt.
                            if(skiSemiuniversal->empty()){
                                cout << "FALSE" << endl;
                                exit(0);
                            }

                            //step 2(c)vi
                            //vi. Set T ←T ∪{S(Ki)}.
                            //this step is done sperately in this method

                            //step 2(c)vii
                            //vii. Remove U from G and update the data structure of connected
                            //components accordingly.
                            for(unordered_set<string>::iterator semiIter = skiSemiuniversal->begin();
                                    semiIter != skiSemiuniversal->end(); semiIter++){
                                string endpoint1 = *semiIter;
                                for(unordered_set<string>::iterator setSIter = substruct->setS->begin(); setSIter !=
                                        substruct->setS->end(); setSIter++){
                                    string endpoint2 = *setSIter;
                                    graphEdge deleteEdge;
                                    if(endpoint1 < endpoint2){
                                        deleteEdge.endPointer1 = endpoint1;
                                        deleteEdge.endPointer2 = endpoint2;
                                    }
                                    else{
                                        deleteEdge.endPointer1 = endpoint2;
                                        deleteEdge.endPointer2 = endpoint1;
                                    }

                                    unordered_set<graphEdge,hashEdge>::iterator edgeFinder = eOne->find(deleteEdge);
                                    if(edgeFinder != eOne->end()){

                                        hdt.hdtDeleteEdge(endpoint1, endpoint2);

                                    }
//                                    else{
//                                        //print info can be deleted later
//                                        edgeFinder = eQuestion->find(deleteEdge);
//                                        if(edgeFinder != eQuestion->end()){
//                                            cout << "try to delete edge in E? (" << endpoint1 << "," << endpoint2 << ")" << endl;
//                                        }
//                                    }

                                    //update the output tree info
                                    unordered_map<string, outputTree*>::iterator parentIter = labelToNode->find(endpoint2);
                                    if(parentIter != labelToNode->end()){
                                        if(newOutputNode->outputParent == nullptr){
                                            newOutputNode->outputParent = parentIter->second->outputParent;
                                        }

                                        parentIter->second->outputParent = newOutputNode;

                                        unordered_set<outputTree*> *childList = newOutputNode->outputParent->outputChildren;
                                        unordered_set<outputTree*>::iterator exiIter = childList->find(parentIter->second);
                                        if(exiIter != childList->end()){
                                            childList->erase(exiIter);

                                            exiIter = childList->find(newOutputNode);
                                            if(exiIter == childList->end()){
                                                childList->insert(newOutputNode);
                                            }
                                        }

                                        newOutputNode->outputChildren->insert(parentIter->second);
                                    }
                                }
                            }
                        }

                        if(newOutputNode->outputChildren->size() == 1){
                            unordered_set<outputTree*> *childList = newOutputNode->outputChildren;
                            outputTree *childNode = *(childList->begin());
                            newOutputNode->label += childNode->label + "+";
                            childNode->outputParent = nullptr;
                            newOutputNode->outputChildren = nullptr;
                            delete childNode;
                            delete childList;
                        }

                        newOutputNode->label = newOutputNode->label.substr(0, newOutputNode->label.length() - 1);
                    }
                    else{
                        continue;
                    }
                }
            }
        }
    }
}

//print method, print out the fancyK structure
void idp_A_fast::printFancyKLists(unordered_map<int, fancyKStructure*> *fancyKList) {
    for(unordered_map<int, fancyKStructure*>::iterator tIter = fancyKList->begin();
        tIter != fancyKList->end(); tIter++){
        cout << "At t = " << tIter->first << ", fancyK structure contains:" << endl;
        for(unordered_map<string, fancyKSubStructure*>::iterator subIter = tIter->second->charToFancyK->begin();
            subIter != tIter->second->charToFancyK->end(); subIter++){
            if(subIter->second != nullptr){
                cout << "\tlabel " << subIter->first << " is in the set of" << subIter->second << " which contains character set {";
                for(unordered_set<string>::iterator strIter = subIter->second->setC->begin(); strIter != subIter->second->setC->end();
                    strIter++){
                    cout << *strIter << ",";
                }
                cout << "} and species set {";
                for(unordered_set<string>::iterator strIter = subIter->second->setS->begin(); strIter != subIter->second->setS->end();
                    strIter++){
                    cout << *strIter << ",";
                }
                cout << "}" << endl;
            }
            else{
                cout << "\tlabel " << subIter->first << " does not related to an edge" << endl;
            }
        }

        cout << endl;

        for(unordered_map<string, fancyKSubStructure*>::iterator subIter = tIter->second->speciesToFancyK->begin();
            subIter != tIter->second->speciesToFancyK->end(); subIter++){
            if(subIter->second != nullptr){
                cout << "\tlabel " << subIter->first << " is in the set of" << subIter->second << " which contains character set {";
                for(unordered_set<string>::iterator strIter = subIter->second->setC->begin(); strIter != subIter->second->setC->end();
                    strIter++){
                    cout << *strIter << ",";
                }
                cout << "} and species set {";
                for(unordered_set<string>::iterator strIter = subIter->second->setS->begin(); strIter != subIter->second->setS->end();
                    strIter++){
                    cout << *strIter << ",";
                }
                cout << "}" << endl;
            }
            else{
                cout << "\tlabel " << subIter->first << " does not related to an edge" << endl;
            }
        }

        cout << endl;

        for(unordered_set<fancyKSubStructure*>::iterator subIter = tIter->second->subStructureList->begin();
            subIter != tIter->second->subStructureList->end(); subIter++){
            if(*subIter != nullptr){
                cout << "\tindividual component " << *subIter << endl;
            }
        }
    }
}

//print method, print out dcOne
void idp_A_fast::printDcOne() {
    cout << "print out d^1_c:" << endl;
    for(unordered_map<string, int>::iterator iterator1 = dcOne->begin(); iterator1 != dcOne->end(); iterator1++){
        cout << "\tcharacter label " << iterator1->first << " has a count of " << iterator1->second << endl;
    }
}

//print method, print out dcQuestion
void idp_A_fast::printDcQuestion() {
    cout << "print out d^?_c:" << endl;
    for(unordered_map<string, int>::iterator iterator1 = dcQuestion->begin(); iterator1 != dcQuestion->end(); iterator1++){
        cout << "\tcharacter label " << iterator1->first << " has a count of " << iterator1->second << endl;
    }
}

//print method, print out outputTree
void idp_A_fast::printOutputTree() {
    cout << "print out output tree:" << endl;
    stack<outputTree*> tempStack;
    int count = 0;
    for(unordered_set<outputTree*>::iterator outputIter = outputSet->begin(); outputIter != outputSet->end(); outputIter++){
        if(*outputIter != nullptr){
            tempStack.push(*outputIter);
        }
        count++;
        cout << "output tree " << count << endl;
        while(!tempStack.empty()){
            outputTree *node = tempStack.top();
            tempStack.pop();
            cout << "node " << node << " with label "<< node->label << " has a parent node: ";
            if(node->outputParent == nullptr){
                cout << "null";
            }
            else{
                cout << node->outputParent;
            }
            cout << endl;
            if(node->outputChildren != nullptr){
                for(unordered_set<outputTree*>::iterator childIter = node->outputChildren->begin(); childIter != node->outputChildren->end();
                    childIter++){
                    tempStack.push(*childIter);
                }
            }
        }
    }
}

void idp_A_fast::write2file(int fileCount) {
    string countStr = to_string(fileCount);
    string fileName = "resultFile" + countStr;
    ofstream outFile(fileName);

    for(unordered_set<outputTree*>::iterator outputIter = outputSet->begin(); outputIter != outputSet->end(); outputIter++){
        string outputNewick;
        if(*outputIter != nullptr){
            outputNewick = convert2newick(*outputIter, outputNewick);
            outFile << outputNewick;
            outFile << ";";
            outFile << "\r\n";
        }
    }

    outFile.close();
}

string idp_A_fast::convert2newick(outputTree *tree, string newick) {
    if(tree != NULL){
        if(tree->outputChildren != nullptr && tree->outputChildren->size() > 0){
            //this tree has the children
            newick += "(";
            for(unordered_set<outputTree *>::iterator _child_entry = tree->outputChildren->begin(); _child_entry != tree->outputChildren->end();
                _child_entry++){
                newick = convert2newick((*_child_entry), newick);
                newick += ',';
            }
            if(newick[newick.length()-1] == ','){
                newick = newick.substr(0, newick.length()-1);
            }
            newick += ")";
        }

        string value = tree->label;
        //in all the data we used, there was no weight associated with any label, so we simply leave it empty here
        string weight = "";

        if(weight.length() > 0){
            value += ":";
            value += weight;
        }

        newick += value;
    }
    return newick;
}

idp_A_fast::~idp_A_fast() {
    for(unordered_map<int, fancyKStructure*>::iterator iterator1 = fancyK->begin(); iterator1 != fancyK->end(); iterator1++){
        delete iterator1->second;
        iterator1->second = nullptr;
    }
    delete fancyK;
    delete eZero;
    delete eOne;
    delete eQuestion;
    delete dcOne;
    delete dcQuestion;

    stack<outputTree*> *tempStack = new stack<outputTree*>();
    for(unordered_set<outputTree*>::iterator iterator3 = outputSet->begin(); iterator3 != outputSet->end(); iterator3++){
        if(*iterator3 != nullptr){
            tempStack->push(*iterator3);
        }

        while(!tempStack->empty()){
            outputTree* p = tempStack->top();
            tempStack->pop();
            if(p->outputChildren != nullptr){
                for(unordered_set<outputTree*>::iterator childIter = p->outputChildren->begin(); childIter != p->outputChildren->end();
                    childIter++){
                    if(*childIter != nullptr){
                        tempStack->push(*childIter);
                    }
                }
            }
            delete p->outputChildren;
            p->outputChildren = nullptr;
            delete p;
            p = nullptr;
        }
    }
    delete labelToNode;
    delete tempStack;
    delete outputSet;
}
