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

#include "idp_matrixTransform.h"

using namespace std;

//convert character matrix to an adjacency list
unordered_map<string, unordered_set<string>*>* idp_matrixTransform::constructAdjacencyList(char **matrix, int columnSize, int rowSize,
                                                                                           unordered_set<string> *skipList) {
    //declare an adjacency list
    //maps from label to adjacency elements
    unordered_map<string, unordered_set<string>*>* adjacencyList = new unordered_map<string, unordered_set<string>*>();

    for(int i = 0; i < columnSize; i++){
        string characterStr = "c_" + to_string(i + 1);
        unordered_set<string>::iterator skipIter = skipList->find(characterStr);
        if(skipIter != skipList->end()){
            continue;
        }
        unordered_set<string>* tmp_list = new unordered_set<string>();
        unordered_set<string>* ques_list = new unordered_set<string>();
        for(int j = 0; j < rowSize; j++){
            string speciesStr = "s_" + to_string(j + 1);
            if(matrix[j][i] == '1'){
                tmp_list->insert(speciesStr);

                //search for character string in the adjacency list, if it is not there, create the entry
                unordered_map<string, unordered_set<string>*>::iterator list_iter = adjacencyList->find(speciesStr);
                if(list_iter != adjacencyList->end()){
                    //find it
                    list_iter->second->insert(characterStr);
                }
                else{
                    //did not find it
                    unordered_set<string>* sub_list = new unordered_set<string>();
                    sub_list->insert(characterStr);
                    adjacencyList->insert(unordered_map<string, unordered_set<string>*>::value_type(speciesStr, sub_list));
                }

            }
        }
        if(!tmp_list->empty()){
            adjacencyList->insert(unordered_map<string, unordered_set<string>*>::value_type(characterStr, tmp_list));
        }
    }

    return adjacencyList;
}

//print method, print out the adjacency list
void idp_matrixTransform::printAdjacencyList(unordered_map<string, unordered_set<string>*> *print_list) {
    cout << "print out adjacency list:" << endl;
    for(unordered_map<string, unordered_set<string> *>::iterator list_iter = print_list->begin(); list_iter != print_list->end(); list_iter++){
        cout << "\t" << list_iter->first << " --> ";
        for(unordered_set<string>::iterator list_sub_iter = list_iter->second->begin(); list_sub_iter != list_iter->second->end(); list_sub_iter++){
            cout << *list_sub_iter << " ";
        }
        cout << endl;
    }
}

