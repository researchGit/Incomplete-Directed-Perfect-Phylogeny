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

#include "idp_inputSetup.h"

using namespace std;

//construct fancy K^0 structure and edge lists E0, E1, and E?
unordered_map<int, fancyKStructure*> *idp_inputSetup::constructFancyKAndEdgeLists(char **matrix, int columnSize, int rowSize) {
    //fancy K^0 structure is defined
    unordered_map<int, fancyKStructure*> *fancyK = new unordered_map<int, fancyKStructure*>();
    fancyKStructure *subFancyK = new fancyKStructure();

    unordered_set<string> *allChars = new unordered_set<string>();
    unordered_set<string> *usedChars = new unordered_set<string>();
    unordered_set<string> *allSpecies = new unordered_set<string>();
    unordered_set<string> *usedSpecies = new unordered_set<string>();

    for(int i = 0; i < rowSize; i++){
        string speciesStr = "s_" + to_string(i + 1);
        allSpecies->insert(speciesStr);
        for(int j = 0; j < columnSize; j++){
            string characterStr = "c_" + to_string(j + 1);
            allChars->insert(characterStr);
            graphEdge edge;
            if(speciesStr < characterStr){
                edge.endPointer1 = speciesStr;
                edge.endPointer2 = characterStr;
            }
            else{
                edge.endPointer1 = characterStr;
                edge.endPointer2 = speciesStr;
            }
            if(matrix[i][j] == '1'){
                usedSpecies->insert(speciesStr);
                usedChars->insert(characterStr);

                unordered_map<string, fancyKSubStructure*>::iterator ltsIter1 = subFancyK->charToFancyK->find(characterStr);
                unordered_map<string, fancyKSubStructure*>::iterator ltsIter2 = subFancyK->speciesToFancyK->find(speciesStr);

                if(ltsIter1 == subFancyK->charToFancyK->end() && ltsIter2 == subFancyK->speciesToFancyK->end()){
                    //both of them are not in the set so far
                    fancyKSubStructure *objOfSubStructure = new fancyKSubStructure();
                    objOfSubStructure->setS->insert(speciesStr);
                    objOfSubStructure->setC->insert(characterStr);
                    subFancyK->charToFancyK->insert(unordered_map<string, fancyKSubStructure*>::value_type(characterStr, objOfSubStructure));
                    subFancyK->speciesToFancyK->insert(unordered_map<string, fancyKSubStructure*>::value_type(speciesStr, objOfSubStructure));
                    subFancyK->subStructureList->insert(objOfSubStructure);
                }
                else if(ltsIter1 != subFancyK->charToFancyK->end() && ltsIter2 == subFancyK->speciesToFancyK->end()){
                    //one of the endpoint is in the list, therefore, insert another endpoint into the same fancyK structure
                    fancyKSubStructure *objOfSubStructure = ltsIter1->second;
                    objOfSubStructure->setS->insert(speciesStr);
                    subFancyK->speciesToFancyK->insert(unordered_map<string, fancyKSubStructure*>::value_type(speciesStr, objOfSubStructure));
                }
                else if(ltsIter1 == subFancyK->charToFancyK->end() && ltsIter2 != subFancyK->speciesToFancyK->end()){
                    //the same case as the previous one
                    fancyKSubStructure *objOfSubStructure = ltsIter2->second;
                    objOfSubStructure->setC->insert(characterStr);
                    subFancyK->charToFancyK->insert(unordered_map<string, fancyKSubStructure*>::value_type(characterStr, objOfSubStructure));
                }

                //push this edge in the edge list E1
                E1->insert(edge);
            }
            else if(matrix[i][j] == '0'){
                //push it into the edge list E0
                E0->insert(edge);
            }
            else{
                usedSpecies->insert(speciesStr);
                usedChars->insert(characterStr);

                unordered_map<string, fancyKSubStructure*>::iterator ltsIter1 = subFancyK->charToFancyK->find(characterStr);
                unordered_map<string, fancyKSubStructure*>::iterator ltsIter2 = subFancyK->speciesToFancyK->find(speciesStr);

                if(ltsIter1 == subFancyK->charToFancyK->end() && ltsIter2 == subFancyK->speciesToFancyK->end()){
                    //both of them are not in the set so far
                    fancyKSubStructure *objOfSubStructure = new fancyKSubStructure();
                    objOfSubStructure->setS->insert(speciesStr);
                    objOfSubStructure->setC->insert(characterStr);
                    subFancyK->charToFancyK->insert(unordered_map<string, fancyKSubStructure*>::value_type(characterStr, objOfSubStructure));
                    subFancyK->speciesToFancyK->insert(unordered_map<string, fancyKSubStructure*>::value_type(speciesStr, objOfSubStructure));
                    subFancyK->subStructureList->insert(objOfSubStructure);
                }
                else if(ltsIter1 != subFancyK->charToFancyK->end() && ltsIter2 == subFancyK->speciesToFancyK->end()){
                    //one of the endpoint is in the list, therefore, insert another endpoint into the same fancyK structure
                    fancyKSubStructure *objOfSubStructure = ltsIter1->second;
                    objOfSubStructure->setS->insert(speciesStr);
                    subFancyK->speciesToFancyK->insert(unordered_map<string, fancyKSubStructure*>::value_type(speciesStr, objOfSubStructure));
                }
                else if(ltsIter1 == subFancyK->charToFancyK->end() && ltsIter2 != subFancyK->speciesToFancyK->end()){
                    //the same case as the previous one
                    fancyKSubStructure *objOfSubStructure = ltsIter2->second;
                    objOfSubStructure->setC->insert(characterStr);
                    subFancyK->charToFancyK->insert(unordered_map<string, fancyKSubStructure*>::value_type(characterStr, objOfSubStructure));
                }

                //push it into the edge list E?
                Eq->insert(edge);
            }
        }
    }

    for(unordered_set<string>::iterator setIter = allChars->begin(); setIter != allChars->end(); setIter++){
        unordered_set<string>::iterator usedIter = usedChars->find(*setIter);
        if(usedIter == usedChars->end()){
            subFancyK->charToFancyK->insert(unordered_map<string, fancyKSubStructure*>::value_type(*setIter, nullptr));
        }
    }

    for(unordered_set<string>::iterator setIter = allSpecies->begin(); setIter != allSpecies->end(); setIter++){
        unordered_set<string>::iterator usedIter = usedSpecies->find(*setIter);
        if(usedIter == usedChars->end()){
            subFancyK->speciesToFancyK->insert(unordered_map<string, fancyKSubStructure*>::value_type(*setIter, nullptr));
        }
    }

    fancyK->insert(unordered_map<int, fancyKStructure*>::value_type(0, subFancyK));

    delete allChars;
    delete allSpecies;
    delete usedChars;
    delete usedSpecies;
    return fancyK;
}

//print method, print out the fancyK structure
void idp_inputSetup::printFancyKLists(unordered_map<int, fancyKStructure*> *fancyKList) {
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

//print method, print out the edge lists
void idp_inputSetup::printEdgeLists() {
    cout << "E0:" << E0->size() << endl;
    for(unordered_set<graphEdge,hashEdge>::iterator edge_iter = E0->begin(); edge_iter != E0->end(); edge_iter++){
        cout << "\t("  << (*edge_iter).endPointer1 << "," << (*edge_iter).endPointer2 << ")" << endl;
    }

    cout << "E1:" << E1->size() << endl;
    for(unordered_set<graphEdge,hashEdge>::iterator edge_iter = E1->begin(); edge_iter != E1->end(); edge_iter++){
        cout << "\t("  << (*edge_iter).endPointer1 << "," << (*edge_iter).endPointer2 << ")" << endl;
    }

    cout << "E?:" << Eq->size() << endl;
    for(unordered_set<graphEdge,hashEdge>::iterator edge_iter = Eq->begin(); edge_iter != Eq->end(); edge_iter++){
        cout << "\t("  << (*edge_iter).endPointer1 << "," << (*edge_iter).endPointer2 << ")" << endl;
    }
}