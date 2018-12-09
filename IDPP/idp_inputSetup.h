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

#ifndef IDP_IDP_INPUTSETUP_H
#define IDP_IDP_INPUTSETUP_H

#include <iostream>
#include "idp_structure.h"

struct fancyKSubStructure{
    unordered_set<string> *setS;
    unordered_set<string> *setC;
    fancyKSubStructure(){setS = new unordered_set<string>(); setC = new unordered_set<string>();};
    ~fancyKSubStructure(){delete setS; delete setC;};
};

struct fancyKStructure{
    unordered_map<string, fancyKSubStructure*> *charToFancyK;
    unordered_map<string, fancyKSubStructure*> *speciesToFancyK;
    unordered_set<fancyKSubStructure*> *subStructureList;
    fancyKStructure(){charToFancyK = new unordered_map<string, fancyKSubStructure*>();
        speciesToFancyK = new unordered_map<string, fancyKSubStructure*>();
        subStructureList = new unordered_set<fancyKSubStructure*>();};
    ~fancyKStructure(){
            for(unordered_set<fancyKSubStructure*>::iterator  iterator1 = subStructureList->begin();
                    iterator1 != subStructureList->end(); iterator1++){
                    delete *iterator1;
            }
            delete subStructureList; delete charToFancyK; delete speciesToFancyK;
    }
};

class idp_inputSetup {

public:
    unordered_map<int, fancyKStructure*> *constructFancyKAndEdgeLists(char **matrix, int columnSize, int rowSize);
    unordered_set<graphEdge, hashEdge> *getE0(){return E0;};
    unordered_set<graphEdge, hashEdge> *getE1(){return E1;};
    unordered_set<graphEdge, hashEdge> *getEq(){return Eq;};

private:
    void printEdgeLists();
    void printFancyKLists(unordered_map<int, fancyKStructure*> *fancyKList);

private:
    unordered_set<graphEdge, hashEdge> *E0 = new unordered_set<graphEdge, hashEdge>();
    unordered_set<graphEdge, hashEdge> *E1 = new unordered_set<graphEdge, hashEdge>();
    unordered_set<graphEdge, hashEdge> *Eq = new unordered_set<graphEdge, hashEdge>();
};


#endif //IDP_IDP_INPUTSETUP_H
