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

#include "idp_inputReader.h"


using namespace std;

//get matrix from the .txt file
char** idp_inputReader::readInputMatrix(string path) {
    ifstream file;
    file.open(path, ios::in);
    string tmp_line;
    string matrixString;
    while(!file.eof()){
        getline(file,tmp_line);
        if(tmp_line.length() > 0){
            rowSize++;
            if(columnSize == 0){
                columnSize = readMatrixColumnSize(tmp_line);
            }
            matrixString += tmp_line;
            matrixString += ";";
        }
    }
    file.close();
    return convertString2CharMatrix(matrixString);
}

//get column size of matrix
int idp_inputReader::readMatrixColumnSize(string matrixRow) {
    int column_size = 0;
    for(int i = 0; i < matrixRow.length(); i++){
        if(matrixRow[i] == '1' || matrixRow[i] == '?' || matrixRow[i] == '0'){
            column_size++;
        }
    }
    return column_size;
}

//convert matrix string to char matrix
char** idp_inputReader::convertString2CharMatrix(string matrixStr) {
    //create a 2d matrix
    char **matrix = new char*[rowSize];
    for(int i = 0; i < rowSize; i++){
        matrix[i] = new char[columnSize];
    }

    int index_column = 0;
    int index_row = 0;
    for(int i = 0; i < matrixStr.length(); i++){
        if(matrixStr[i] == '1' || matrixStr[i] == '0' || matrixStr[i] == '?'){
            matrix[index_row][index_column++] = matrixStr[i];
        }
        else if(matrixStr[i] == ';'){
            index_column = 0;
            index_row++;
        }
    }

    return matrix;
}

//print method, print out the matrix
void idp_inputReader::printMatrix(char **mat, int column, int row) {
    cout << "matrix: " << endl;
    for(int i = 0; i < column; i++){
        cout << "c_" + to_string(i + 1) << "\t";
    }
    cout << endl;
    for(int i = 0; i < row; i++){
        for(int j = 0; j <= column; j++){
            if(j != column){
                cout << mat[i][j] << "\t";
            }
            else{
                cout << "s_" + to_string(i+1) << "\t";
            }
        }
        cout << endl;
    }
    cout << "column size: " << column << endl;
    cout << "row size: " << row << endl;
}
