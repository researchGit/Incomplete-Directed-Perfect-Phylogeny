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

#include "idp_generator.h"

using namespace std;

//in order to tree with n nodes, we can generate a Prufer code sequence with length of n - 2
//to make sure the tree contains m species, we will remove m numbers from the sequence
char** idp_generator::generateNumberSequence(int m, int n, int N) {
    if(n <= m || m > N){
        cout << "Input numbers are wrong!" << endl;
        exit(0);
    }

    characterList = new vector<int>(n, 0);

    for(int i = 0; i < n; i++){
        (*characterList)[i] = i + 1;
    }

    //remove m numbers from the vector
    for(int i = 0; i < m; i++){
        int randomIndex = (int)(rand() % characterList->size());
        vector<int>::iterator vecIter = characterList->begin() + randomIndex;
        speciesList->insert(*vecIter);
        characterList->erase(vecIter);
    }

    int size = n - m;
    for(int i = 0; i < n - m; i++){
        pruferCode->push_back((*characterList)[i]);
    }
    for(int i = 0; i < m - 2; i++){
        int randomIndex = (rand() % size);
        pruferCode->push_back((*characterList)[randomIndex]);
    }
    random_shuffle(pruferCode->begin(), pruferCode->end());

    return convert2Tree(n, N);
}

char** idp_generator::generateNumberSequenceForTreeCompatibilityLowDensity(int m, int n, int N) {
    if(n <= m || m > N){
        cout << "Input numbers are wrong!" << endl;
        exit(0);
    }

    characterList = new vector<int>(n, 0);

    for(int i = 0; i < n; i++){
        (*characterList)[i] = i + 1;
    }

    //remove m numbers from the vector
    for(int i = 0; i < m; i++){
        int randomIndex = (int)(rand() % characterList->size());
        vector<int>::iterator vecIter = characterList->begin() + randomIndex;
        speciesList->insert(*vecIter);
        characterList->erase(vecIter);
    }

    int size = n - m;
    for(int i = 0; i < n - m; i++){
        pruferCode->push_back((*characterList)[i]);
    }
    for(int i = 0; i < m - 2; i++){
        int randomIndex = (rand() % size);
        pruferCode->push_back((*characterList)[randomIndex]);
    }
    random_shuffle(pruferCode->begin(), pruferCode->end());

    return convert2Tree4TreeCompaibilityLowDensity(n, N);
}

char** idp_generator::generateNumberSequenceForTreeCompatibilityHighDensity(int m, int n, int N) {
    if(n <= m || m > N){
        cout << "Input numbers are wrong!" << endl;
        exit(0);
    }

    characterList = new vector<int>(n, 0);

    for(int i = 0; i < n; i++){
        (*characterList)[i] = i + 1;
    }

    //remove m numbers from the vector
    for(int i = 0; i < m; i++){
        int randomIndex = (int)(rand() % characterList->size());
        vector<int>::iterator vecIter = characterList->begin() + randomIndex;
        speciesList->insert(*vecIter);
        characterList->erase(vecIter);
    }

    int size = n - m;
    for(int i = 0; i < n - m; i++){
        pruferCode->push_back((*characterList)[i]);
    }
    for(int i = 0; i < m - 2; i++){
        int randomIndex = (rand() % size);
        pruferCode->push_back((*characterList)[randomIndex]);
    }
    random_shuffle(pruferCode->begin(), pruferCode->end());

    return convert2Tree4TreeCompaibilityHighDensity(n, N);
}

char** idp_generator::generateNumberSequenceForHighRatio(int m, int n, int N) {
    if(n <= m || m > N){
        cout << "Input numbers are wrong!" << endl;
        exit(0);
    }

    characterList = new vector<int>(n, 0);

    for(int i = 0; i < n; i++){
        (*characterList)[i] = i + 1;
    }

    //remove m numbers from the vector
    for(int i = 0; i < m; i++){
        int randomIndex = (int)(rand() % characterList->size());
        vector<int>::iterator vecIter = characterList->begin() + randomIndex;
        speciesList->insert(*vecIter);
        characterList->erase(vecIter);
    }

    int size = n - m;
    for(int i = 0; i < n - m; i++){
        pruferCode->push_back((*characterList)[i]);
    }
    for(int i = 0; i < m - 2; i++){
        int randomIndex = (rand() % size);
        pruferCode->push_back((*characterList)[randomIndex]);
    }
    random_shuffle(pruferCode->begin(), pruferCode->end());

    return convert2TreeWithHighRatio(n, N);
}

char** idp_generator::generateNumberSequenceForIntermediateRatio(int m, int n, int N) {
    if(n <= m || m > N){
        cout << "Input numbers are wrong!" << endl;
        exit(0);
    }

    characterList = new vector<int>(n, 0);

    for(int i = 0; i < n; i++){
        (*characterList)[i] = i + 1;
    }

    //remove m numbers from the vector
    for(int i = 0; i < m; i++){
        int randomIndex = (int)(rand() % characterList->size());
        vector<int>::iterator vecIter = characterList->begin() + randomIndex;
        speciesList->insert(*vecIter);
        characterList->erase(vecIter);
    }

    int size = n - m;
    for(int i = 0; i < n - m; i++){
        pruferCode->push_back((*characterList)[i]);
    }
    for(int i = 0; i < m - 2; i++){
        int randomIndex = (rand() % size);
        pruferCode->push_back((*characterList)[randomIndex]);
    }
    random_shuffle(pruferCode->begin(), pruferCode->end());

    return convert2TreeWithIntermediateRatio(n, N);
}

//convert Prufer Code to Adjacency List while entry of 1 is higher
char** idp_generator::convert2TreeWithHighRatio(int n, int N) {
    vector<int> *degree = new vector<int>(n + 1, 0);
    unordered_map<int, unordered_set<int> *> *adjacencyMap = new unordered_map<int, unordered_set<int> *>();

    for(int i = 0; i < n - 2; i++){
        (*degree)[(*pruferCode)[i]] += 1;
    }

    for(int i = 0; i < n; i++){
        (*degree)[i + 1] += 1;
    }

    for(int i = 0; i < n - 2; i++){
        int j = 1;
        while((*degree)[j] != 1){
            j++;
        }
        int endpoint1 = j;
        int endpoint2 = (*pruferCode)[i];

        unordered_map<int, unordered_set<int> *>::iterator mapIter = adjacencyMap->find(endpoint1);
        if(mapIter != adjacencyMap->end()){
            mapIter->second->insert(endpoint2);
        }
        else{
            unordered_set<int> *tempSet = new unordered_set<int>();
            tempSet->insert(endpoint2);
            adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint1, tempSet));
        }

        mapIter = adjacencyMap->find(endpoint2);
        if(mapIter != adjacencyMap->end()){
            mapIter->second->insert(endpoint1);
        }
        else{
            unordered_set<int> *tempSet = new unordered_set<int>();
            tempSet->insert(endpoint1);
            adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint2, tempSet));
        }

        (*degree)[endpoint1] -= 1;
        (*degree)[endpoint2] -= 1;
    }

    int endpoint1 = 1;
    while((*degree)[endpoint1] != 1){
        endpoint1++;
    }
    int endpoint2 = n;
    while((*degree)[endpoint2] != 1){
        endpoint2--;
    }

    unordered_map<int, unordered_set<int> *>::iterator mapIter = adjacencyMap->find(endpoint1);
    if(mapIter != adjacencyMap->end()){
        mapIter->second->insert(endpoint2);
    }
    else{
        unordered_set<int> *tempSet = new unordered_set<int>();
        tempSet->insert(endpoint2);
        adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint1, tempSet));
    }

    mapIter = adjacencyMap->find(endpoint2);
    if(mapIter != adjacencyMap->end()){
        mapIter->second->insert(endpoint1);
    }
    else{
        unordered_set<int> *tempSet = new unordered_set<int>();
        tempSet->insert(endpoint1);
        adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint2, tempSet));
    }

    degree->clear();
    vector<int>().swap((*degree));
    delete degree;

    //find any internal nodes as root
    int randomIndex = (int)(rand() % characterList->size());

    int numberRoot = (*characterList)[randomIndex];

    stack<int> *tempStack = new stack<int>();
    unordered_map<int, inputTree*> *labelToNode = new unordered_map<int, inputTree*>();
    tempStack->push(numberRoot);
    inputTree *root = new inputTree();
    root->label = numberRoot;
    root->parent = nullptr;
    labelToNode->insert(unordered_map<int, inputTree*>::value_type(numberRoot, root));
    int subCount = 0;
    while(!tempStack->empty()){
        int number = tempStack->top();
        tempStack->pop();

        unordered_set<int>::iterator exiIter = speciesList->find(number);
        if(exiIter != speciesList->end()){
            continue;
        }

        inputTree *node = nullptr;
        unordered_map<int, inputTree*>::iterator nodeIter = labelToNode->find(number);
        if(nodeIter != labelToNode->end()){
            node = nodeIter->second;
        }

        mapIter = adjacencyMap->find(number);
        if(mapIter != adjacencyMap->end()){
            for(unordered_set<int>::iterator setIter= mapIter->second->begin(); setIter != mapIter->second->end(); setIter++){
                if(node->parent != nullptr && *setIter == node->parent->label){
                    continue;
                }
                inputTree *childNode = new inputTree();
                childNode->parent = node;
                childNode->label = *setIter;
                exiIter = speciesList->find(*setIter);
                if(exiIter != speciesList->end()){
                    childNode->sublabel = subCount++;
                }
                node->childList->insert(childNode);
                labelToNode->insert(unordered_map<int, inputTree*>::value_type(*setIter, childNode));
                tempStack->push(*setIter);
            }
        }
    }

    for(unordered_set<int>::iterator setIter = speciesList->begin(); setIter != speciesList->end(); setIter++){
        unordered_map<int, inputTree*>::iterator nodeIter = labelToNode->find(*setIter);
        if(nodeIter != labelToNode->end()){
            int number = 0;
            inputTree *start = nodeIter->second;
            if(start != nullptr){
                number = start->sublabel;
            }
            for(; start != nullptr; start = start->parent){
                if(start->parent != nullptr){
                    start->parent->coverSpecies->push_back(number);
                }
            }
        }
    }

    //create a 2d matrix
    int m = (int)speciesList->size();
    rowSize = m;
    columnSize = n - m;
    char **matrix = new char*[m];
    for(int i = 0; i < m; i++){
        matrix[i] = new char[n - m];
    }

    for(int i = 0 ; i < m; i++){
        for(int j = 0; j < n - m; j++){
            matrix[i][j] = '0';
        }
    }

    stack<inputTree*> *rootStack = new stack<inputTree*>();
    rootStack->push(root);
    int columnCount = 0;
    while(!rootStack->empty()){
        inputTree *p = rootStack->top();
        rootStack->pop();

        if(p != nullptr && p->sublabel == -1){
            //indicate this node is one endpoint of an internal edge
            if(p->coverSpecies != nullptr){
                for(list<int>::iterator lIter = p->coverSpecies->begin(); lIter != p->coverSpecies->end(); lIter++){
                    matrix[*lIter][columnCount] = '1';
                }
                columnCount++;
            }

            if(p->childList != nullptr){
                for(unordered_set<inputTree*>::iterator sIter = p->childList->begin(); sIter != p->childList->end(); sIter++){
                    rootStack->push(*sIter);
                }
            }
        }
    }

    //find those columns with more than half entries are 1
    vector<int>* moreThanHalf = new vector<int>();
    for(int i = 1; i < (n-m); i++){
        int entryCount = 0;
        for(int j = 0; j < m; j++){
            if(matrix[j][i] == '1') entryCount++;
        }
        double entryRatio = (double)entryCount/m;
        if(entryRatio >= 0.5){
            moreThanHalf->push_back(i);
        }
    }
    double changeRatio = 0.4;
    while(moreThanHalf->size() == 0 && changeRatio >= 0.1){
        for(int i = 1; i < (n-m); i++){
            int entryCount = 0;
            for(int j = 0; j < m; j++){
                if(matrix[j][i] == '1') entryCount++;
            }
            double entryRatio = (double)entryCount/m;
            if(entryRatio >= changeRatio){
                moreThanHalf->push_back(i);
            }
        }
        changeRatio -= 0.1;
    }

    rowSize = m;
    columnSize = N;
    char **expandMatrix = new char*[m];
    for(int i = 0; i < m; i++){
        expandMatrix[i] = new char[N];
    }

    for(int i = 0 ; i < m; i++){
        for(int j = 0; j < n - m; j++){
            expandMatrix[i][j] = matrix[i][j];
            if(expandMatrix[i][j] == '1'){
                sizeOfE1++;
            }
            else{
                sizeOfE0++;
            }
        }
    }

    for(int i = n - m; i < N; i++){
        int randomColumn = (int)(rand() % moreThanHalf->size());
        for(int j = 0; j < m; j++){
            expandMatrix[j][i] = matrix[j][(*moreThanHalf)[randomColumn]];
            if(expandMatrix[j][i] == '1'){
                sizeOfE1++;
            }
            else{
                sizeOfE0++;
            }
        }
    }

    //destruct
    rootStack->push(root);
    while(!rootStack->empty()){
        inputTree *p = rootStack->top();
        rootStack->pop();

        if(p != nullptr){
            if(p->childList != nullptr){
                for(unordered_set<inputTree*>::iterator sIter = p->childList->begin(); sIter != p->childList->end(); sIter++){
                    rootStack->push(*sIter);
                }
            }
            delete p->coverSpecies;
            delete p->childList;
            delete p;
        }
    }

    delete tempStack;
    delete labelToNode;
    delete rootStack;
    moreThanHalf->clear();
    vector<int>().swap((*moreThanHalf));
    delete moreThanHalf;

    //destruct original 2d matrix
    for(int i = 0 ; i < m; i++){
        delete[] matrix[i];
    }
    delete[] matrix;

    return expandMatrix;
}

char** idp_generator::convert2TreeWithIntermediateRatio(int n, int N) {
    vector<int> *degree = new vector<int>(n + 1, 0);
    unordered_map<int, unordered_set<int> *> *adjacencyMap = new unordered_map<int, unordered_set<int> *>();

    for(int i = 0; i < n - 2; i++){
        (*degree)[(*pruferCode)[i]] += 1;
    }

    for(int i = 0; i < n; i++){
        (*degree)[i + 1] += 1;
    }

    for(int i = 0; i < n - 2; i++){
        int j = 1;
        while((*degree)[j] != 1){
            j++;
        }
        int endpoint1 = j;
        int endpoint2 = (*pruferCode)[i];

        unordered_map<int, unordered_set<int> *>::iterator mapIter = adjacencyMap->find(endpoint1);
        if(mapIter != adjacencyMap->end()){
            mapIter->second->insert(endpoint2);
        }
        else{
            unordered_set<int> *tempSet = new unordered_set<int>();
            tempSet->insert(endpoint2);
            adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint1, tempSet));
        }

        mapIter = adjacencyMap->find(endpoint2);
        if(mapIter != adjacencyMap->end()){
            mapIter->second->insert(endpoint1);
        }
        else{
            unordered_set<int> *tempSet = new unordered_set<int>();
            tempSet->insert(endpoint1);
            adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint2, tempSet));
        }

        (*degree)[endpoint1] -= 1;
        (*degree)[endpoint2] -= 1;
    }

    int endpoint1 = 1;
    while((*degree)[endpoint1] != 1){
        endpoint1++;
    }
    int endpoint2 = n;
    while((*degree)[endpoint2] != 1){
        endpoint2--;
    }

    unordered_map<int, unordered_set<int> *>::iterator mapIter = adjacencyMap->find(endpoint1);
    if(mapIter != adjacencyMap->end()){
        mapIter->second->insert(endpoint2);
    }
    else{
        unordered_set<int> *tempSet = new unordered_set<int>();
        tempSet->insert(endpoint2);
        adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint1, tempSet));
    }

    mapIter = adjacencyMap->find(endpoint2);
    if(mapIter != adjacencyMap->end()){
        mapIter->second->insert(endpoint1);
    }
    else{
        unordered_set<int> *tempSet = new unordered_set<int>();
        tempSet->insert(endpoint1);
        adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint2, tempSet));
    }

    degree->clear();
    vector<int>().swap((*degree));
    delete degree;

    //find any internal nodes as root
    int randomIndex = (int)(rand() % characterList->size());

    int numberRoot = (*characterList)[randomIndex];

    stack<int> *tempStack = new stack<int>();
    unordered_map<int, inputTree*> *labelToNode = new unordered_map<int, inputTree*>();
    tempStack->push(numberRoot);
    inputTree *root = new inputTree();
    root->label = numberRoot;
    root->parent = nullptr;
    labelToNode->insert(unordered_map<int, inputTree*>::value_type(numberRoot, root));
    int subCount = 0;
    while(!tempStack->empty()){
        int number = tempStack->top();
        tempStack->pop();

        unordered_set<int>::iterator exiIter = speciesList->find(number);
        if(exiIter != speciesList->end()){
            continue;
        }

        inputTree *node = nullptr;
        unordered_map<int, inputTree*>::iterator nodeIter = labelToNode->find(number);
        if(nodeIter != labelToNode->end()){
            node = nodeIter->second;
        }

        mapIter = adjacencyMap->find(number);
        if(mapIter != adjacencyMap->end()){
            for(unordered_set<int>::iterator setIter= mapIter->second->begin(); setIter != mapIter->second->end(); setIter++){
                if(node->parent != nullptr && *setIter == node->parent->label){
                    continue;
                }
                inputTree *childNode = new inputTree();
                childNode->parent = node;
                childNode->label = *setIter;
                exiIter = speciesList->find(*setIter);
                if(exiIter != speciesList->end()){
                    childNode->sublabel = subCount++;
                }
                node->childList->insert(childNode);
                labelToNode->insert(unordered_map<int, inputTree*>::value_type(*setIter, childNode));
                tempStack->push(*setIter);
            }
        }
    }

    for(unordered_set<int>::iterator setIter = speciesList->begin(); setIter != speciesList->end(); setIter++){
        unordered_map<int, inputTree*>::iterator nodeIter = labelToNode->find(*setIter);
        if(nodeIter != labelToNode->end()){
            int number = 0;
            inputTree *start = nodeIter->second;
            if(start != nullptr){
                number = start->sublabel;
            }
            for(; start != nullptr; start = start->parent){
                if(start->parent != nullptr){
                    start->parent->coverSpecies->push_back(number);
                }
            }
        }
    }

    //create a 2d matrix
    int m = (int)speciesList->size();
    rowSize = m;
    columnSize = n - m;
    char **matrix = new char*[m];
    for(int i = 0; i < m; i++){
        matrix[i] = new char[n - m];
    }

    for(int i = 0 ; i < m; i++){
        for(int j = 0; j < n - m; j++){
            matrix[i][j] = '0';
        }
    }

    stack<inputTree*> *rootStack = new stack<inputTree*>();
    rootStack->push(root);
    int columnCount = 0;
    while(!rootStack->empty()){
        inputTree *p = rootStack->top();
        rootStack->pop();

        if(p != nullptr && p->sublabel == -1){
            //indicate this node is one endpoint of an internal edge
            if(p->coverSpecies != nullptr){
                for(list<int>::iterator lIter = p->coverSpecies->begin(); lIter != p->coverSpecies->end(); lIter++){
                    matrix[*lIter][columnCount] = '1';
                }
                columnCount++;
            }

            if(p->childList != nullptr){
                for(unordered_set<inputTree*>::iterator sIter = p->childList->begin(); sIter != p->childList->end(); sIter++){
                    rootStack->push(*sIter);
                }
            }
        }
    }

    //find those columns with more than half entries are 1
    vector<int>* moreThanHalf = new vector<int>();
    for(int i = 1; i < (n-m); i++){
        int entryCount = 0;
        for(int j = 0; j < m; j++){
            if(matrix[j][i] == '1') entryCount++;
        }
        double entryRatio = (double)entryCount/m;
        if(entryRatio >= 0.2){
            moreThanHalf->push_back(i);
        }
    }
    double changeRatio = 0.1;
    while(moreThanHalf->size() == 0 && changeRatio >= 0.1){
        for(int i = 1; i < (n-m); i++){
            int entryCount = 0;
            for(int j = 0; j < m; j++){
                if(matrix[j][i] == '1') entryCount++;
            }
            double entryRatio = (double)entryCount/m;
            if(entryRatio >= changeRatio){
                moreThanHalf->push_back(i);
            }
        }
        changeRatio -= 0.1;
    }

    rowSize = m;
    columnSize = N;
    char **expandMatrix = new char*[m];
    for(int i = 0; i < m; i++){
        expandMatrix[i] = new char[N];
    }

    for(int i = 0 ; i < m; i++){
        for(int j = 0; j < n - m; j++){
            expandMatrix[i][j] = matrix[i][j];
            if(expandMatrix[i][j] == '1'){
                sizeOfE1++;
            }
            else{
                sizeOfE0++;
            }
        }
    }

    for(int i = n - m; i < N; i++){
        int randomColumn = (int)(rand() % moreThanHalf->size());
        for(int j = 0; j < m; j++){
            expandMatrix[j][i] = matrix[j][(*moreThanHalf)[randomColumn]];
            if(expandMatrix[j][i] == '1'){
                sizeOfE1++;
            }
            else{
                sizeOfE0++;
            }
        }
    }

    //destruct
    rootStack->push(root);
    while(!rootStack->empty()){
        inputTree *p = rootStack->top();
        rootStack->pop();

        if(p != nullptr){
            if(p->childList != nullptr){
                for(unordered_set<inputTree*>::iterator sIter = p->childList->begin(); sIter != p->childList->end(); sIter++){
                    rootStack->push(*sIter);
                }
            }
            delete p->coverSpecies;
            delete p->childList;
            delete p;
        }
    }

    delete tempStack;
    delete labelToNode;
    delete rootStack;
    moreThanHalf->clear();
    vector<int>().swap((*moreThanHalf));
    delete moreThanHalf;

    //destruct original 2d matrix
    for(int i = 0 ; i < m; i++){
        delete[] matrix[i];
    }
    delete[] matrix;

    return expandMatrix;
}

//convert Prufer Code to Adjacency List
char** idp_generator::convert2Tree(int n, int N) {
    vector<int> *degree = new vector<int>(n + 1, 0);
    unordered_map<int, unordered_set<int> *> *adjacencyMap = new unordered_map<int, unordered_set<int> *>();

    for(int i = 0; i < n - 2; i++){
        (*degree)[(*pruferCode)[i]] += 1;
    }

    for(int i = 0; i < n; i++){
        (*degree)[i + 1] += 1;
    }

    for(int i = 0; i < n - 2; i++){
        int j = 1;
        while((*degree)[j] != 1){
            j++;
        }
        int endpoint1 = j;
        int endpoint2 = (*pruferCode)[i];

        unordered_map<int, unordered_set<int> *>::iterator mapIter = adjacencyMap->find(endpoint1);
        if(mapIter != adjacencyMap->end()){
            mapIter->second->insert(endpoint2);
        }
        else{
            unordered_set<int> *tempSet = new unordered_set<int>();
            tempSet->insert(endpoint2);
            adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint1, tempSet));
        }

        mapIter = adjacencyMap->find(endpoint2);
        if(mapIter != adjacencyMap->end()){
            mapIter->second->insert(endpoint1);
        }
        else{
            unordered_set<int> *tempSet = new unordered_set<int>();
            tempSet->insert(endpoint1);
            adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint2, tempSet));
        }

        (*degree)[endpoint1] -= 1;
        (*degree)[endpoint2] -= 1;
    }

    int endpoint1 = 1;
    while((*degree)[endpoint1] != 1){
        endpoint1++;
    }
    int endpoint2 = n;
    while((*degree)[endpoint2] != 1){
        endpoint2--;
    }

    unordered_map<int, unordered_set<int> *>::iterator mapIter = adjacencyMap->find(endpoint1);
    if(mapIter != adjacencyMap->end()){
        mapIter->second->insert(endpoint2);
    }
    else{
        unordered_set<int> *tempSet = new unordered_set<int>();
        tempSet->insert(endpoint2);
        adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint1, tempSet));
    }

    mapIter = adjacencyMap->find(endpoint2);
    if(mapIter != adjacencyMap->end()){
        mapIter->second->insert(endpoint1);
    }
    else{
        unordered_set<int> *tempSet = new unordered_set<int>();
        tempSet->insert(endpoint1);
        adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint2, tempSet));
    }

    degree->clear();
    vector<int>().swap((*degree));
    delete degree;

    //find any internal nodes as root
    int randomIndex = (int)(rand() % characterList->size());

    int numberRoot = (*characterList)[randomIndex];

    stack<int> *tempStack = new stack<int>();
    unordered_map<int, inputTree*> *labelToNode = new unordered_map<int, inputTree*>();
    tempStack->push(numberRoot);
    inputTree *root = new inputTree();
    root->label = numberRoot;
    root->parent = nullptr;
    labelToNode->insert(unordered_map<int, inputTree*>::value_type(numberRoot, root));
    int subCount = 0;
    while(!tempStack->empty()){
        int number = tempStack->top();
        tempStack->pop();

        unordered_set<int>::iterator exiIter = speciesList->find(number);
        if(exiIter != speciesList->end()){
            continue;
        }

        inputTree *node = nullptr;
        unordered_map<int, inputTree*>::iterator nodeIter = labelToNode->find(number);
        if(nodeIter != labelToNode->end()){
            node = nodeIter->second;
        }

        mapIter = adjacencyMap->find(number);
        if(mapIter != adjacencyMap->end()){
            for(unordered_set<int>::iterator setIter= mapIter->second->begin(); setIter != mapIter->second->end(); setIter++){
                if(node->parent != nullptr && *setIter == node->parent->label){
                    continue;
                }
                inputTree *childNode = new inputTree();
                childNode->parent = node;
                childNode->label = *setIter;
                exiIter = speciesList->find(*setIter);
                if(exiIter != speciesList->end()){
                    childNode->sublabel = subCount++;
                }
                node->childList->insert(childNode);
                labelToNode->insert(unordered_map<int, inputTree*>::value_type(*setIter, childNode));
                tempStack->push(*setIter);
            }
        }
    }

    for(unordered_set<int>::iterator setIter = speciesList->begin(); setIter != speciesList->end(); setIter++){
        unordered_map<int, inputTree*>::iterator nodeIter = labelToNode->find(*setIter);
        if(nodeIter != labelToNode->end()){
            int number = 0;
            inputTree *start = nodeIter->second;
            if(start != nullptr){
                number = start->sublabel;
            }
            for(; start != nullptr; start = start->parent){
                if(start->parent != nullptr){
                    start->parent->coverSpecies->push_back(number);
                }
            }
        }
    }


    //create a 2d matrix
    int m = (int)speciesList->size();
    rowSize = m;
    columnSize = n - m;
    char **matrix = new char*[m];
    for(int i = 0; i < m; i++){
        matrix[i] = new char[n - m];
    }

    for(int i = 0 ; i < m; i++){
        for(int j = 0; j < n - m; j++){
            matrix[i][j] = '0';
        }
    }

    stack<inputTree*> *rootStack = new stack<inputTree*>();
    rootStack->push(root);
    int columnCount = 0;
    while(!rootStack->empty()){
        inputTree *p = rootStack->top();
        rootStack->pop();

        if(p != nullptr && p->sublabel == -1){
            //indicate this node is one endpoint of an internal edge
            if(p->coverSpecies != nullptr){
                for(list<int>::iterator lIter = p->coverSpecies->begin(); lIter != p->coverSpecies->end(); lIter++){
                    matrix[*lIter][columnCount] = '1';
                }
                columnCount++;
            }

            if(p->childList != nullptr){
                for(unordered_set<inputTree*>::iterator sIter = p->childList->begin(); sIter != p->childList->end(); sIter++){
                    rootStack->push(*sIter);
                }
            }
        }
    }

    rowSize = m;
    columnSize = N;
    char **expandMatrix = new char*[m];
    for(int i = 0; i < m; i++){
        expandMatrix[i] = new char[N];
    }

    for(int i = 0 ; i < m; i++){
        for(int j = 0; j < n - m; j++){
            expandMatrix[i][j] = matrix[i][j];
            if(expandMatrix[i][j] == '1'){
                sizeOfE1++;
            }
        }
    }

    for(int i = 0; i < m; i++){
        for(int j = n - m; j < N; j++){
            expandMatrix[i][j] = expandMatrix[i][j % (n - m)];
            if(expandMatrix[i][j] == '1'){
                sizeOfE1++;
            }
        }
    }



    delete tempStack;
    delete labelToNode;
    delete rootStack;

    //destruct original 2d matrix
    for(int i = 0 ; i < m; i++){
        delete[] matrix[i];
    }
    delete[] matrix;

    return expandMatrix;
}

char** idp_generator::convert2Tree4TreeCompaibilityLowDensity(int n, int N) {
    vector<int> *degree = new vector<int>(n + 1, 0);
    unordered_map<int, unordered_set<int> *> *adjacencyMap = new unordered_map<int, unordered_set<int> *>();

    for(int i = 0; i < n - 2; i++){
        (*degree)[(*pruferCode)[i]] += 1;
    }

    for(int i = 0; i < n; i++){
        (*degree)[i + 1] += 1;
    }

    for(int i = 0; i < n - 2; i++){
        int j = 1;
        while((*degree)[j] != 1){
            j++;
        }
        int endpoint1 = j;
        int endpoint2 = (*pruferCode)[i];

        unordered_map<int, unordered_set<int> *>::iterator mapIter = adjacencyMap->find(endpoint1);
        if(mapIter != adjacencyMap->end()){
            mapIter->second->insert(endpoint2);
        }
        else{
            unordered_set<int> *tempSet = new unordered_set<int>();
            tempSet->insert(endpoint2);
            adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint1, tempSet));
        }

        mapIter = adjacencyMap->find(endpoint2);
        if(mapIter != adjacencyMap->end()){
            mapIter->second->insert(endpoint1);
        }
        else{
            unordered_set<int> *tempSet = new unordered_set<int>();
            tempSet->insert(endpoint1);
            adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint2, tempSet));
        }

        (*degree)[endpoint1] -= 1;
        (*degree)[endpoint2] -= 1;
    }

    int endpoint1 = 1;
    while((*degree)[endpoint1] != 1){
        endpoint1++;
    }
    int endpoint2 = n;
    while((*degree)[endpoint2] != 1){
        endpoint2--;
    }

    unordered_map<int, unordered_set<int> *>::iterator mapIter = adjacencyMap->find(endpoint1);
    if(mapIter != adjacencyMap->end()){
        mapIter->second->insert(endpoint2);
    }
    else{
        unordered_set<int> *tempSet = new unordered_set<int>();
        tempSet->insert(endpoint2);
        adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint1, tempSet));
    }

    mapIter = adjacencyMap->find(endpoint2);
    if(mapIter != adjacencyMap->end()){
        mapIter->second->insert(endpoint1);
    }
    else{
        unordered_set<int> *tempSet = new unordered_set<int>();
        tempSet->insert(endpoint1);
        adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint2, tempSet));
    }


    degree->clear();
    vector<int>().swap((*degree));
    delete degree;


    //find any internal nodes as root
    int randomIndex = (int)(rand() % characterList->size());

    int numberRoot = (*characterList)[randomIndex];

    stack<int> *tempStack = new stack<int>();
    unordered_map<int, inputTree*> *labelToNode = new unordered_map<int, inputTree*>();
    tempStack->push(numberRoot);
    inputTree *root = new inputTree();
    root->label = numberRoot;
    root->parent = nullptr;
    labelToNode->insert(unordered_map<int, inputTree*>::value_type(numberRoot, root));
    int subCount = 0;
    while(!tempStack->empty()){
        int number = tempStack->top();
        tempStack->pop();

        unordered_set<int>::iterator exiIter = speciesList->find(number);
        if(exiIter != speciesList->end()){
            continue;
        }

        inputTree *node = nullptr;
        unordered_map<int, inputTree*>::iterator nodeIter = labelToNode->find(number);
        if(nodeIter != labelToNode->end()){
            node = nodeIter->second;
        }

        mapIter = adjacencyMap->find(number);
        if(mapIter != adjacencyMap->end()){
            for(unordered_set<int>::iterator setIter= mapIter->second->begin(); setIter != mapIter->second->end(); setIter++){
                if(node->parent != nullptr && *setIter == node->parent->label){
                    continue;
                }
                inputTree *childNode = new inputTree();
                childNode->parent = node;
                childNode->label = *setIter;
                exiIter = speciesList->find(*setIter);
                if(exiIter != speciesList->end()){
                    childNode->sublabel = subCount++;
                }
                node->childList->insert(childNode);
                labelToNode->insert(unordered_map<int, inputTree*>::value_type(*setIter, childNode));
                tempStack->push(*setIter);
            }
        }
    }

    for(unordered_set<int>::iterator setIter = speciesList->begin(); setIter != speciesList->end(); setIter++){
        unordered_map<int, inputTree*>::iterator nodeIter = labelToNode->find(*setIter);
        if(nodeIter != labelToNode->end()){
            int number = 0;
            inputTree *start = nodeIter->second;
            if(start != nullptr){
                number = start->sublabel;
            }
            for(; start != nullptr; start = start->parent){
                if(start->parent != nullptr){
                    start->parent->coverSpecies->push_back(number);
                }
            }
        }
    }

    //create a 2d matrix
    int m = (int)speciesList->size();
    rowSize = m;
    columnSize = n - m;
    char **matrix = new char*[m];
    for(int i = 0; i < m; i++){
        matrix[i] = new char[n - m];
    }

    for(int i = 0 ; i < m; i++){
        for(int j = 0; j < n - m; j++){
            matrix[i][j] = '0';
        }
    }

    stack<inputTree*> *rootStack = new stack<inputTree*>();
    rootStack->push(root);
    int columnCount = 0;
    while(!rootStack->empty()){
        inputTree *p = rootStack->top();
        rootStack->pop();

        if(p != nullptr && p->sublabel == -1){
            //indicate this node is one endpoint of an internal edge
            if(p->coverSpecies != nullptr){
                for(list<int>::iterator lIter = p->coverSpecies->begin(); lIter != p->coverSpecies->end(); lIter++){
                    matrix[*lIter][columnCount] = '1';
                }
                columnCount++;
            }

            if(p->childList != nullptr){
                for(unordered_set<inputTree*>::iterator sIter = p->childList->begin(); sIter != p->childList->end(); sIter++){
                    rootStack->push(*sIter);
                }
            }
        }
    }

    rowSize = m;
    columnSize = N;
    char **expandMatrix = new char*[m];
    for(int i = 0; i < m; i++){
        expandMatrix[i] = new char[N];
    }

    for(int i = 0 ; i < m; i++){
        for(int j = 0; j < n - m; j++){
            expandMatrix[i][j] = matrix[i][j];
            if(expandMatrix[i][j] == '1'){
                sizeOfE1++;
            }
        }
    }

    for(int i = 0; i < m; i++){
        for(int j = n - m; j < N; j++){
            expandMatrix[i][j] = expandMatrix[i][j % (n - m)];
            if(expandMatrix[i][j] == '1'){
                sizeOfE1++;
            }
        }
    }


    delete tempStack;
    delete labelToNode;
    delete rootStack;

    //destruct original 2d matrix
    for(int i = 0 ; i < m; i++){
        delete[] matrix[i];
    }
    delete[] matrix;

    return expandMatrix;
}


char** idp_generator::convert2Tree4TreeCompaibilityHighDensity(int n, int N) {
    vector<int> *degree = new vector<int>(n + 1, 0);
    unordered_map<int, unordered_set<int> *> *adjacencyMap = new unordered_map<int, unordered_set<int> *>();

    for(int i = 0; i < n - 2; i++){
        (*degree)[(*pruferCode)[i]] += 1;
    }

    for(int i = 0; i < n; i++){
        (*degree)[i + 1] += 1;
    }

    for(int i = 0; i < n - 2; i++){
        int j = 1;
        while((*degree)[j] != 1){
            j++;
        }
        int endpoint1 = j;
        int endpoint2 = (*pruferCode)[i];

        unordered_map<int, unordered_set<int> *>::iterator mapIter = adjacencyMap->find(endpoint1);
        if(mapIter != adjacencyMap->end()){
            mapIter->second->insert(endpoint2);
        }
        else{
            unordered_set<int> *tempSet = new unordered_set<int>();
            tempSet->insert(endpoint2);
            adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint1, tempSet));
        }

        mapIter = adjacencyMap->find(endpoint2);
        if(mapIter != adjacencyMap->end()){
            mapIter->second->insert(endpoint1);
        }
        else{
            unordered_set<int> *tempSet = new unordered_set<int>();
            tempSet->insert(endpoint1);
            adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint2, tempSet));
        }

        (*degree)[endpoint1] -= 1;
        (*degree)[endpoint2] -= 1;
    }

    int endpoint1 = 1;
    while((*degree)[endpoint1] != 1){
        endpoint1++;
    }
    int endpoint2 = n;
    while((*degree)[endpoint2] != 1){
        endpoint2--;
    }

    unordered_map<int, unordered_set<int> *>::iterator mapIter = adjacencyMap->find(endpoint1);
    if(mapIter != adjacencyMap->end()){
        mapIter->second->insert(endpoint2);
    }
    else{
        unordered_set<int> *tempSet = new unordered_set<int>();
        tempSet->insert(endpoint2);
        adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint1, tempSet));
    }

    mapIter = adjacencyMap->find(endpoint2);
    if(mapIter != adjacencyMap->end()){
        mapIter->second->insert(endpoint1);
    }
    else{
        unordered_set<int> *tempSet = new unordered_set<int>();
        tempSet->insert(endpoint1);
        adjacencyMap->insert(unordered_map<int, unordered_set<int>*>::value_type(endpoint2, tempSet));
    }


    degree->clear();
    vector<int>().swap((*degree));
    delete degree;

    //find any internal nodes as root
    int randomIndex = (int)(rand() % characterList->size());

    int numberRoot = (*characterList)[randomIndex];

    stack<int> *tempStack = new stack<int>();
    unordered_map<int, inputTree*> *labelToNode = new unordered_map<int, inputTree*>();
    tempStack->push(numberRoot);
    inputTree *root = new inputTree();
    root->label = numberRoot;
    root->parent = nullptr;
    labelToNode->insert(unordered_map<int, inputTree*>::value_type(numberRoot, root));
    int subCount = 0;
    while(!tempStack->empty()){
        int number = tempStack->top();
        tempStack->pop();

        unordered_set<int>::iterator exiIter = speciesList->find(number);
        if(exiIter != speciesList->end()){
            continue;
        }

        inputTree *node = nullptr;
        unordered_map<int, inputTree*>::iterator nodeIter = labelToNode->find(number);
        if(nodeIter != labelToNode->end()){
            node = nodeIter->second;
        }

        mapIter = adjacencyMap->find(number);
        if(mapIter != adjacencyMap->end()){
            for(unordered_set<int>::iterator setIter= mapIter->second->begin(); setIter != mapIter->second->end(); setIter++){
                if(node->parent != nullptr && *setIter == node->parent->label){
                    continue;
                }
                inputTree *childNode = new inputTree();
                childNode->parent = node;
                childNode->label = *setIter;
                exiIter = speciesList->find(*setIter);
                if(exiIter != speciesList->end()){
                    childNode->sublabel = subCount++;
                }
                node->childList->insert(childNode);
                labelToNode->insert(unordered_map<int, inputTree*>::value_type(*setIter, childNode));
                tempStack->push(*setIter);
            }
        }
    }

    for(unordered_set<int>::iterator setIter = speciesList->begin(); setIter != speciesList->end(); setIter++){
        unordered_map<int, inputTree*>::iterator nodeIter = labelToNode->find(*setIter);
        if(nodeIter != labelToNode->end()){
            int number = 0;
            inputTree *start = nodeIter->second;
            if(start != nullptr){
                number = start->sublabel;
            }
            for(; start != nullptr; start = start->parent){
                if(start->parent != nullptr){
                    start->parent->coverSpecies->push_back(number);
                }
            }
        }
    }


    //create a 2d matrix
    int m = (int)speciesList->size();
    rowSize = m;
    columnSize = n - m;
    char **matrix = new char*[m];
    for(int i = 0; i < m; i++){
        matrix[i] = new char[n - m];
    }

    for(int i = 0 ; i < m; i++){
        for(int j = 0; j < n - m; j++){
            matrix[i][j] = '0';
        }
    }


    stack<inputTree*> *rootStack = new stack<inputTree*>();
    rootStack->push(root);
    int columnCount = 0;
    while(!rootStack->empty()){
        inputTree *p = rootStack->top();
        rootStack->pop();

        if(p != nullptr && p->sublabel == -1){
            //indicate this node is one endpoint of an internal edge
            if(p->coverSpecies != nullptr){
                for(list<int>::iterator lIter = p->coverSpecies->begin(); lIter != p->coverSpecies->end(); lIter++){
                    matrix[*lIter][columnCount] = '1';
                }
                columnCount++;
            }

            if(p->childList != nullptr){
                for(unordered_set<inputTree*>::iterator sIter = p->childList->begin(); sIter != p->childList->end(); sIter++){
                    rootStack->push(*sIter);
                }
            }
        }
    }

    //find those columns with more than half entries are 1
    vector<int>* moreThanHalf = new vector<int>();
    for(int i = 1; i < (n-m); i++){
        int entryCount = 0;
        for(int j = 0; j < m; j++){
            if(matrix[j][i] == '1') entryCount++;
        }
        double entryRatio = (double)entryCount/m;
        if(entryRatio >= 0.5){
            moreThanHalf->push_back(i);
        }
    }
    double changeRatio = 0.4;
    while(moreThanHalf->size() == 0 && changeRatio >= 0.1){
        for(int i = 1; i < (n-m); i++){
            int entryCount = 0;
            for(int j = 0; j < m; j++){
                if(matrix[j][i] == '1') entryCount++;
            }
            double entryRatio = (double)entryCount/m;
            if(entryRatio >= changeRatio){
                moreThanHalf->push_back(i);
            }
        }
        changeRatio -= 0.1;
    }


    rowSize = m;
    columnSize = N;
    char **expandMatrix = new char*[m];
    for(int i = 0; i < m; i++){
        expandMatrix[i] = new char[N];
    }

    for(int i = 0 ; i < m; i++){
        for(int j = 0; j < n - m; j++){
            expandMatrix[i][j] = matrix[i][j];
            if(expandMatrix[i][j] == '1'){
                sizeOfE1++;
            }
            else{
                sizeOfE0++;
            }
        }
    }

    for(int i = n - m; i < N; i++){
        int randomColumn = (int)(rand() % moreThanHalf->size());
        for(int j = 0; j < m; j++){
            expandMatrix[j][i] = matrix[j][(*moreThanHalf)[randomColumn]];
            if(expandMatrix[j][i] == '1'){
                sizeOfE1++;
            }
            else{
                sizeOfE0++;
            }
        }
    }



    //destruct
    rootStack->push(root);
    while(!rootStack->empty()){
        inputTree *p = rootStack->top();
        rootStack->pop();

        if(p != nullptr){
            if(p->childList != nullptr){
                for(unordered_set<inputTree*>::iterator sIter = p->childList->begin(); sIter != p->childList->end(); sIter++){
                    rootStack->push(*sIter);
                }
            }
            delete p->coverSpecies;
            delete p->childList;
            delete p;
        }
    }

    delete tempStack;
    delete labelToNode;
    delete rootStack;
    moreThanHalf->clear();
    vector<int>().swap((*moreThanHalf));
    delete moreThanHalf;

    //destruct original 2d matrix
    for(int i = 0 ; i < m; i++){
        delete[] matrix[i];
    }
    delete[] matrix;

    return expandMatrix;
}

char** idp_generator::randomConvertE0toEq(int percentageRemain, char **matrix) {
    char **convertMatrix = new char*[rowSize];
    for(int i = 0; i < rowSize; i++){
        convertMatrix[i] = new char[columnSize];
    }

    vector<coordinate*> *coordinateVecOfE0 = new vector<coordinate*>();

    for(int i = 0; i < rowSize; i++){
        for(int j = 0; j < columnSize; j++){
            convertMatrix[i][j] = matrix[i][j];
            if(convertMatrix[i][j] == '0'){
                coordinate *coordinateObj = new coordinate(i, j);
                coordinateVecOfE0->push_back(coordinateObj);
            }
        }
    }

    //randomly choose one element from vector and convert the value of 1 to 0
    int frequency4E0 = (rowSize*columnSize - sizeOfE1) * (100 - percentageRemain) / 100;
    cout << "Zero entries converted are " << frequency4E0 << endl;
    while(frequency4E0 > 0){
        if(coordinateVecOfE0->size() > 1){
            int randomIndex = (int)(rand() % coordinateVecOfE0->size());
            convertMatrix[(*coordinateVecOfE0)[randomIndex]->x][(*coordinateVecOfE0)[randomIndex]->y] = '?';
            vector<coordinate*>::iterator vecIter = coordinateVecOfE0->begin();
            coordinateVecOfE0->erase(vecIter + randomIndex);
        }
        else{
            convertMatrix[(*coordinateVecOfE0)[0]->x][(*coordinateVecOfE0)[0]->y] = '?';
            vector<coordinate*>::iterator vecIter = coordinateVecOfE0->begin();
            coordinateVecOfE0->erase(vecIter);
        }
        frequency4E0--;
    }

    coordinateVecOfE0->clear();
    vector<coordinate*>().swap((*coordinateVecOfE0));
    delete coordinateVecOfE0;
    return convertMatrix;
}

char** idp_generator::randomConvertE0toEq(int percentageRemain, char **matrix, int row, int col) {
    rowSize = row;
    columnSize = col;
    sizeOfE1 = 0;
    char **convertMatrix = new char*[rowSize];
    for(int i = 0; i < rowSize; i++){
        convertMatrix[i] = new char[columnSize];
    }

    vector<coordinate*> *coordinateVecOfE0 = new vector<coordinate*>();

    for(int i = 0; i < rowSize; i++){
        for(int j = 0; j < columnSize; j++){
            convertMatrix[i][j] = matrix[i][j];
            if(convertMatrix[i][j] == '0'){
                coordinate *coordinateObj = new coordinate(i, j);
                coordinateVecOfE0->push_back(coordinateObj);
            }
            else{
                sizeOfE1++;
            }
        }
    }

    //randomly choose one element from vector and convert the value of 1 to 0
    int frequency4E0 = (rowSize*columnSize - sizeOfE1) * (100 - percentageRemain) / 100;
    cout << "Zero entries converted are " << frequency4E0 << endl;
    while(frequency4E0 > 0){
        if(coordinateVecOfE0->size() > 1){
            int randomIndex = (int)(rand() % coordinateVecOfE0->size());
            convertMatrix[(*coordinateVecOfE0)[randomIndex]->x][(*coordinateVecOfE0)[randomIndex]->y] = '?';
            swap((*coordinateVecOfE0)[randomIndex],(*coordinateVecOfE0)[coordinateVecOfE0->size() - 1]);
            vector<coordinate*>::iterator vecIter = coordinateVecOfE0->begin();
            coordinateVecOfE0->erase(vecIter + coordinateVecOfE0->size() - 1);
        }
        else{
            convertMatrix[(*coordinateVecOfE0)[0]->x][(*coordinateVecOfE0)[0]->y] = '?';
            vector<coordinate*>::iterator vecIter = coordinateVecOfE0->begin();
            coordinateVecOfE0->erase(vecIter);
        }
        frequency4E0--;
    }

    coordinateVecOfE0->clear();
    vector<coordinate*>().swap((*coordinateVecOfE0));
    delete coordinateVecOfE0;
    return convertMatrix;
}

char** idp_generator::randomConvertE0toEqBasedOnE1(int percentageRemain, char **matrix, int row, int col) {
    rowSize = row;
    columnSize = col;
    sizeOfE1 = 0;
    char **convertMatrix = new char*[rowSize];
    for(int i = 0; i < rowSize; i++){
        convertMatrix[i] = new char[columnSize];
    }

    vector<coordinate*> *coordinateVecOfE0 = new vector<coordinate*>();

    for(int i = 0; i < rowSize; i++){
        for(int j = 0; j < columnSize; j++){
            convertMatrix[i][j] = matrix[i][j];
            if(convertMatrix[i][j] == '0'){
                coordinate *coordinateObj = new coordinate(i, j);
                coordinateVecOfE0->push_back(coordinateObj);
            }
            else{
                sizeOfE1++;
            }
        }
    }

    //randomly choose one element from vector and convert the value of 1 to 0
    int frequency4E0 = sizeOfE1 * (100 - percentageRemain) / 100;
    cout << "Zero entries converted are " << frequency4E0 << endl;
    while(frequency4E0 > 0){
        if(coordinateVecOfE0->size() > 1){
            int randomIndex = (int)(rand() % coordinateVecOfE0->size());
            convertMatrix[(*coordinateVecOfE0)[randomIndex]->x][(*coordinateVecOfE0)[randomIndex]->y] = '?';
            vector<coordinate*>::iterator vecIter = coordinateVecOfE0->begin();
            coordinateVecOfE0->erase(vecIter + randomIndex);
        }
        else{
            convertMatrix[(*coordinateVecOfE0)[0]->x][(*coordinateVecOfE0)[0]->y] = '?';
            vector<coordinate*>::iterator vecIter = coordinateVecOfE0->begin();
            coordinateVecOfE0->erase(vecIter);
        }
        frequency4E0--;
    }

    coordinateVecOfE0->clear();
    vector<coordinate*>().swap((*coordinateVecOfE0));
    delete coordinateVecOfE0;
    return convertMatrix;
}

char** idp_generator::randomConvertE1ToEQ(int percentageRemain, char** matrix) {
    char **convertMatrix = new char*[rowSize];
    for(int i = 0; i < rowSize; i++){
        convertMatrix[i] = new char[columnSize];
    }

    vector<coordinate*> *coordinateVec = new vector<coordinate*>();

    for(int i = 0; i < rowSize; i++){
        for(int j = 0; j < columnSize; j++){
            convertMatrix[i][j] = matrix[i][j];
            if(convertMatrix[i][j] == '1'){
                coordinate *coordinateObj = new coordinate(i, j);
                coordinateVec->push_back(coordinateObj);
            }
        }
    }

    //randomly choose one element from vector and convert the value of 1 to 0
    int frequency = sizeOfE1 * (100 - percentageRemain) / 100;
    cout << "Edges converted are " << frequency << endl;
    while(frequency > 0){
        if(coordinateVec->size() > 1){
            int randomIndex = (int)(rand() % coordinateVec->size());
            convertMatrix[(*coordinateVec)[randomIndex]->x][(*coordinateVec)[randomIndex]->y] = '?';
            vector<coordinate*>::iterator vecIter = coordinateVec->begin();
            coordinateVec->erase(vecIter + randomIndex);
        }
        else{
            convertMatrix[(*coordinateVec)[0]->x][(*coordinateVec)[0]->y] = '?';
            vector<coordinate*>::iterator vecIter = coordinateVec->begin();
            coordinateVec->erase(vecIter);
        }
        frequency--;
    }

    delete coordinateVec;
    return convertMatrix;
}

char** idp_generator::randomConvertE1E0ToEQ(int percentageRemain, char **matrix) {
    char **convertMatrix = new char*[rowSize];
    for(int i = 0; i < rowSize; i++){
        convertMatrix[i] = new char[columnSize];
    }

    vector<coordinate*> *coordinateVecOfE1 = new vector<coordinate*>();
    vector<coordinate*> *coordinateVecOfE0 = new vector<coordinate*>();

    for(int i = 0; i < rowSize; i++){
        for(int j = 0; j < columnSize; j++){
            convertMatrix[i][j] = matrix[i][j];
            if(convertMatrix[i][j] == '1'){
                coordinate *coordinateObj = new coordinate(i, j);
                coordinateVecOfE1->push_back(coordinateObj);
            }
            else if(convertMatrix[i][j] == '0'){
                coordinate *coordinateObj = new coordinate(i, j);
                coordinateVecOfE0->push_back(coordinateObj);
            }
        }
    }

    //randomly choose one element from vector and convert the value of 1 to 0
    int frequency = sizeOfE1 * (100 - percentageRemain) / 100;
    int frequency4E0 = frequency;
    cout << "Edges converted are " << frequency << endl;
    while(frequency > 0){
        if(coordinateVecOfE1->size() > 1){
            int randomIndex = (int)(rand() % coordinateVecOfE1->size());
            convertMatrix[(*coordinateVecOfE1)[randomIndex]->x][(*coordinateVecOfE1)[randomIndex]->y] = '?';
            vector<coordinate*>::iterator vecIter = coordinateVecOfE1->begin();
            coordinateVecOfE1->erase(vecIter + randomIndex);
        }
        else{
            convertMatrix[(*coordinateVecOfE1)[0]->x][(*coordinateVecOfE1)[0]->y] = '?';
            vector<coordinate*>::iterator vecIter = coordinateVecOfE1->begin();
            coordinateVecOfE1->erase(vecIter);
        }
        frequency--;
    }

    cout << "Zero entries converted are " << frequency4E0 << endl;
    while(frequency4E0 > 0){
        if(coordinateVecOfE0->size() > 1){
            int randomIndex = (int)(rand() % coordinateVecOfE0->size());
            convertMatrix[(*coordinateVecOfE0)[randomIndex]->x][(*coordinateVecOfE0)[randomIndex]->y] = '?';
            vector<coordinate*>::iterator vecIter = coordinateVecOfE0->begin();
            coordinateVecOfE0->erase(vecIter + randomIndex);
        }
        else{
            convertMatrix[(*coordinateVecOfE0)[0]->x][(*coordinateVecOfE0)[0]->y] = '?';
            vector<coordinate*>::iterator vecIter = coordinateVecOfE0->begin();
            coordinateVecOfE0->erase(vecIter);
        }
        frequency4E0--;
    }

    for(int i = 0; i < coordinateVecOfE0->size(); i++){
        delete (*coordinateVecOfE0)[i];
    }
    vector<coordinate*>().swap((*coordinateVecOfE0));
    for(int i = 0 ; i < coordinateVecOfE1->size(); i++){
        delete (*coordinateVecOfE1)[i];
    }
    vector<coordinate*>().swap((*coordinateVecOfE1));
    delete coordinateVecOfE1;
    delete coordinateVecOfE0;
    return convertMatrix;
}

char** idp_generator::randomConvertE1E0ToEQ(int percentageRemain, char **matrix, int row, int col) {
    rowSize = row;
    columnSize = col;
    sizeOfE1 = 0;
    char **convertMatrix = new char*[rowSize];
    for(int i = 0; i < rowSize; i++){
        convertMatrix[i] = new char[columnSize];
    }

    vector<coordinate*> *coordinateVecOfE1 = new vector<coordinate*>();
    vector<coordinate*> *coordinateVecOfE0 = new vector<coordinate*>();

    for(int i = 0; i < rowSize; i++){
        for(int j = 0; j < columnSize; j++){
            convertMatrix[i][j] = matrix[i][j];
            if(convertMatrix[i][j] == '1'){
                coordinate *coordinateObj = new coordinate(i, j);
                coordinateVecOfE1->push_back(coordinateObj);
                sizeOfE1++;
            }
            else if(convertMatrix[i][j] == '0'){
                coordinate *coordinateObj = new coordinate(i, j);
                coordinateVecOfE0->push_back(coordinateObj);
            }
        }
    }

    //randomly choose one element from vector and convert the value of 1 to 0
    int frequency = sizeOfE1 * (100 - percentageRemain) / 100;
    int frequency4E0 = (rowSize*columnSize-sizeOfE1) * (100 -percentageRemain) / 100;
    cout << "Edges converted are " << frequency << endl;
    while(frequency > 0){
        if(coordinateVecOfE1->size() > 1){
            int randomIndex = (int)(rand() % coordinateVecOfE1->size());
            convertMatrix[(*coordinateVecOfE1)[randomIndex]->x][(*coordinateVecOfE1)[randomIndex]->y] = '?';
            swap((*coordinateVecOfE1)[randomIndex],(*coordinateVecOfE1)[coordinateVecOfE1->size() - 1]);
            vector<coordinate*>::iterator vecIter = coordinateVecOfE1->begin();
            coordinateVecOfE1->erase(vecIter + coordinateVecOfE1->size() - 1);
        }
        else{
            convertMatrix[(*coordinateVecOfE1)[0]->x][(*coordinateVecOfE1)[0]->y] = '?';
            vector<coordinate*>::iterator vecIter = coordinateVecOfE1->begin();
            coordinateVecOfE1->erase(vecIter);
        }
        frequency--;
    }

    cout << "Zero entries converted are " << frequency4E0 << endl;
    while(frequency4E0 > 0){
        if(coordinateVecOfE0->size() > 1){
            int randomIndex = (int)(rand() % coordinateVecOfE0->size());
            convertMatrix[(*coordinateVecOfE0)[randomIndex]->x][(*coordinateVecOfE0)[randomIndex]->y] = '?';
            swap((*coordinateVecOfE0)[randomIndex],(*coordinateVecOfE0)[coordinateVecOfE0->size() - 1]);
            vector<coordinate*>::iterator vecIter = coordinateVecOfE0->begin();
            coordinateVecOfE0->erase(vecIter + coordinateVecOfE0->size() - 1);
        }
        else{
            convertMatrix[(*coordinateVecOfE0)[0]->x][(*coordinateVecOfE0)[0]->y] = '?';
            vector<coordinate*>::iterator vecIter = coordinateVecOfE0->begin();
            coordinateVecOfE0->erase(vecIter);
        }
        frequency4E0--;
    }

    for(int i = 0; i < coordinateVecOfE1->size(); i++){
        delete (*coordinateVecOfE1)[i];
    }
    vector<coordinate*>().swap((*coordinateVecOfE1));
    delete coordinateVecOfE1;
    for(int i = 0 ; i < coordinateVecOfE0->size(); i++){
        delete (*coordinateVecOfE0)[i];
    }
    vector<coordinate*>().swap((*coordinateVecOfE0));
    delete coordinateVecOfE0;
    return convertMatrix;
}

void idp_generator::idpp2TreeCompatibility(char **matrix, int row, int col, int count) {
    int tempCount = 1;
    string matrixFilePath = "idpp2Tree_Matrix" + to_string(count);
    string treesFilePath = "idpp2Tree_Trees" + to_string(count);
    ofstream write1;
    write1.open(matrixFilePath);
    write1.close();
    ofstream write2;
    write2.open(treesFilePath);
    write2.close();
    for(int i = 0; i < col; i++){
        string rootLabel;
        if(i % row == 0){
            rootLabel = "c_" + to_string(i + 1);
        }
        else{
            rootLabel = "f_" + to_string(tempCount++);
        }

        treeCom *root = new treeCom(rootLabel);

        if(i % row == 0){
            for(int j = 0; j < row; j++){
                if(matrix[j][i] == '1'){
                    treeCom *sChild = new treeCom("s_" + to_string(j+1));
                    root->treeChild->insert(sChild);
                }
            }
        }
        else{
            unordered_set<string> *zeroLabels = new unordered_set<string>();
            unordered_set<string> *oneLabels = new unordered_set<string>();
            for(int j = 0; j < row; j++){
                if(matrix[j][i] == '0'){
                    zeroLabels->insert("s_" + to_string(j+1));
                }
                else if(matrix[j][i] == '1'){
                    oneLabels->insert("s_" + to_string(j+1));
                }
            }

            if(zeroLabels->empty()){
                if(oneLabels->size() == 1){
                    root->label = *(oneLabels->begin());
                }
                else{
                    root->label = "c_" + to_string(i + 1);
                    for(unordered_set<string>::iterator setIter = oneLabels->begin();
                        setIter != oneLabels->end(); setIter++){
                        treeCom *childNode = new treeCom(*setIter);
                        root->treeChild->insert(childNode);
                    }
                }
            }
            else{
                for(unordered_set<string>::iterator setIter = zeroLabels->begin();
                    setIter != zeroLabels->end(); setIter++){
                    treeCom *childNode = new treeCom(*setIter);
                    root->treeChild->insert(childNode);
                }
                if(oneLabels->size() == 1){
                    treeCom *childNode = new treeCom(*(oneLabels->begin()));
                    root->treeChild->insert(childNode);
                }
                else{
                    treeCom *cNode = new treeCom("c_" + to_string(i + 1));
                    for(unordered_set<string>::iterator setIter = oneLabels->begin();
                        setIter != oneLabels->end(); setIter++){
                        treeCom *childNode = new treeCom(*setIter);
                        cNode->treeChild->insert(childNode);
                    }
                    root->treeChild->insert(cNode);
                }
            }
            zeroLabels->clear();
            delete zeroLabels;
            oneLabels->clear();
            delete oneLabels;
        }

        string newickStr;
        newickStr = convert2newick(root, newickStr);
        newickStr += ";";

        writeInputTrees2File(treesFilePath, newickStr);

        stack<treeCom*> *tempStack = new stack<treeCom*>();
        tempStack->push(root);
        while(!tempStack->empty()){
            treeCom* node = tempStack->top();
            tempStack->pop();
            if(!node->treeChild->empty()){
                for(unordered_set<treeCom*>::iterator setIter = node->treeChild->begin(); setIter != node->treeChild->end();
                    setIter++){
                    tempStack->push(*setIter);
                }
            }
            else{
                delete node->treeChild;
            }
            delete node;
        }
        delete tempStack;
    }

    writeMatrix2file(matrixFilePath, matrix, row, col);

}

void idp_generator::idpp2TreeCompatibilityHighDensity(char **matrix, int row, int col, int count) {
    int tempCount = 1;
    string matrixFilePath = "idpp2Tree_Matrix" + to_string(count);
    string treesFilePath = "idpp2Tree_Trees" + to_string(count);
    ofstream write1;
    write1.open(matrixFilePath);
    write1.close();
    ofstream write2;
    write2.open(treesFilePath);
    write2.close();
    for(int i = 0; i < col; i++){
        string rootLabel;
        if(i == 0){
            rootLabel = "c_" + to_string(i + 1);
        }
        else{
            rootLabel = "f_" + to_string(tempCount++);
        }

        treeCom *root = new treeCom(rootLabel);

        if(i == 0){
            for(int j = 0; j < row; j++){
                if(matrix[j][i] == '1'){
                    treeCom *sChild = new treeCom("s_" + to_string(j+1));
                    root->treeChild->insert(sChild);
                }
            }
        }
        else{
            unordered_set<string> *zeroLabels = new unordered_set<string>();
            unordered_set<string> *oneLabels = new unordered_set<string>();
            for(int j = 0; j < row; j++){
                if(matrix[j][i] == '0'){
                    zeroLabels->insert("s_" + to_string(j+1));
                }
                else if(matrix[j][i] == '1'){
                    oneLabels->insert("s_" + to_string(j+1));
                }
            }

            if(zeroLabels->empty()){
                if(oneLabels->size() == 1){
                    root->label = *(oneLabels->begin());
                }
                else{
                    root->label = "c_" + to_string(i + 1);
                    for(unordered_set<string>::iterator setIter = oneLabels->begin();
                        setIter != oneLabels->end(); setIter++){
                        treeCom *childNode = new treeCom(*setIter);
                        root->treeChild->insert(childNode);
                    }
                }
            }
            else{
                for(unordered_set<string>::iterator setIter = zeroLabels->begin();
                    setIter != zeroLabels->end(); setIter++){
                    treeCom *childNode = new treeCom(*setIter);
                    root->treeChild->insert(childNode);
                }
                if(oneLabels->size() == 1){
                    treeCom *childNode = new treeCom(*(oneLabels->begin()));
                    root->treeChild->insert(childNode);
                }
                else{
                    treeCom *cNode = new treeCom("c_" + to_string(i + 1));
                    for(unordered_set<string>::iterator setIter = oneLabels->begin();
                        setIter != oneLabels->end(); setIter++){
                        treeCom *childNode = new treeCom(*setIter);
                        cNode->treeChild->insert(childNode);
                    }
                    root->treeChild->insert(cNode);
                }
            }
            zeroLabels->clear();
            delete zeroLabels;
            oneLabels->clear();
            delete oneLabels;
        }

        string newickStr;
        newickStr = convert2newick(root, newickStr);
        newickStr += ";";

        writeInputTrees2File(treesFilePath, newickStr);

        stack<treeCom*> *tempStack = new stack<treeCom*>();
        tempStack->push(root);
        while(!tempStack->empty()){
            treeCom* node = tempStack->top();
            tempStack->pop();
            if(!node->treeChild->empty()){
                for(unordered_set<treeCom*>::iterator setIter = node->treeChild->begin(); setIter != node->treeChild->end();
                    setIter++){
                    tempStack->push(*setIter);
                }
            }
            else{
                delete node->treeChild;
            }
            delete node;
        }
        delete tempStack;
    }

    writeMatrix2file(matrixFilePath, matrix, row, col);

}

void idp_generator::writeInputTrees2File(string filePath, string ioStr) {
    ofstream write;
    write.open(filePath, ios::app);
    if(!write){
        cout << "file cannot be opened" << endl;
        abort();
    }
    else{
        write << ioStr;
        write << "\r\n";
    }
    write.close();
}

void idp_generator::writeMatrix2file(string filePath, char **matrix, int row, int col) {
    ofstream outFile(filePath);

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if(j != col - 1){
                outFile << matrix[i][j];
                outFile << " ";
            }
            else{
                outFile << matrix[i][j];
                outFile << "\r\n";
            }
        }
    }

    outFile.close();
}

string idp_generator::convert2newick(treeCom *tree, string newick) {
    if(tree != nullptr){
        if(tree->treeChild != nullptr && tree->treeChild->size() > 0){
            //this tree has the children
            newick += "(";
            for(unordered_set<treeCom *>::iterator _child_entry = tree->treeChild->begin(); _child_entry != tree->treeChild->end();
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

void idp_generator::writeMatrix2file(char **seedMatrix, int row, int col) {
    ofstream outFile("seedMatrix.txt");

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if(j != col - 1){
                outFile << seedMatrix[i][j];
                outFile << " ";
            }
            else{
                outFile << seedMatrix[i][j];
                outFile << "\r\n";
            }
        }
    }

    outFile.close();
}

//destructor
idp_generator::~idp_generator() {
    pruferCode->clear();
    vector<int>().swap((*pruferCode));
    delete pruferCode;
    delete speciesList;
    if(characterList != nullptr){
        characterList->clear();
        vector<int>().swap((*characterList));
    }
    delete characterList;
}