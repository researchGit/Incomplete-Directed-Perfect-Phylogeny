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

#ifndef IDP_IDP_STRUCTURE_H
#define IDP_IDP_STRUCTURE_H

#endif //IDP_IDP_STRUCTURE_H

#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <limits>

using namespace std;

struct graphEdge{
    string endPointer1;
    string endPointer2;
    bool operator == (const graphEdge &t) const {
        return  endPointer1 == t.endPointer1 && endPointer2 == t.endPointer2;
    }
    graphEdge(){};
    graphEdge(string a, string b){endPointer1 = a; endPointer2 = b;};
};

struct hashEdge {
    size_t operator () (const graphEdge &t) const {
        hash<string> hash;
        return hash(t.endPointer1 + t.endPointer2);
    }
};

struct outputTree{
    outputTree *outputParent;
    unordered_set<outputTree*> *outputChildren;
    string label;
    outputTree(){outputParent = nullptr; outputChildren = new unordered_set<outputTree*>(); label = "";};
    outputTree(string a):label(a){outputParent = nullptr; outputChildren = nullptr;};
    outputTree(string a, outputTree* parent):label(a),outputParent(parent){outputChildren = new unordered_set<outputTree*>();};
};