//MIT License
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

#include <iostream>
#include "idp_inputReader.h"
#include "idp_A_fast.h"
#include "idp_generator.h"
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {
    //initialize random variable
    srand((unsigned int)time(nullptr));

    if(strcmp(argv[1],"-g") == 0){
        //invoke generator, either generate the seed matrix, or generate a bunch of matrices and their transformed
        //phylogenetic trees
        if(strcmp(argv[2],"-sd") == 0){
            //generate a seed matrix
            //./IDPP -g -sd --s 100 --c 20 --h/--m/--l
            if(argc == 8){
                string strSpecies = argv[4];
                string strTimes = argv[6];
                string strDensity = argv[7];

                stringstream valueSpecies(strSpecies);
                int species = 0;
                valueSpecies >> species;

                stringstream valueTimes(strTimes);
                int times = 0;
                valueTimes >> times;

                int initial_char = 2 * species;
                int characters = times * species;


                idp_generator generator;
                if(strDensity == "--l"){
                    char **unchangedMatrix = generator.generateNumberSequence(species, initial_char, characters);
                    generator.writeMatrix2file(unchangedMatrix, species, characters);
                }
                else if(strDensity == "--m"){
                    char **unchangedMatrix = generator.generateNumberSequenceForIntermediateRatio(species, initial_char, characters);
                    generator.writeMatrix2file(unchangedMatrix, species, characters);
                }
                else if(strDensity == "--h"){
                    char **unchangedMatrix = generator.generateNumberSequenceForHighRatio(species, initial_char, characters);
                    generator.writeMatrix2file(unchangedMatrix, species, characters);
                }
                else{
                    cout << "Error! The command is not correct!" << endl;
                    exit(0);
                }
            }
            else{
                cout << "Error! The command is not correct!" << endl;
                exit(0);
            }
        }
        else if(strcmp(argv[2], "-t") == 0){
            //./IDPP -g -t --s 100 --c 20 --p 95 --l/--h --f 30
            //generate a list of matrices, and convert each matrix to a list of phylogenetic trees
            if(argc == 12){
                string strSpecies = argv[4];
                string strTimes = argv[6];
                string strPercentageOfZeroRemained = argv[8];
                string strDensity = argv[9];
                string strFrequency = argv[11];

                stringstream valueSpecies(strSpecies);
                int species = 0;
                valueSpecies >> species;
                stringstream valueTimes(strTimes);
                int times = 0;
                valueTimes >> times;
                int initial_char = 2 * species;
                int characters = times * species;
                stringstream valuePercentageOfZeroRemained(strPercentageOfZeroRemained);
                int percentageOfZeroRemained = 0;
                valuePercentageOfZeroRemained >> percentageOfZeroRemained;
                stringstream valueFrequency(strFrequency);
                int frequency = 0;
                valueFrequency >> frequency;

                if(strDensity == "--l"){
                    for(int i = 0; i < frequency; i++){
                        idp_generator generator;
                        char **unchangedMatrix = generator.generateNumberSequenceForTreeCompatibilityLowDensity(species, initial_char, characters);
                        char **matrix = generator.randomConvertE0toEq(percentageOfZeroRemained, unchangedMatrix, species, characters);
                        generator.idpp2TreeCompatibility(matrix, species, characters, i + 1);
                    }
                }
                else if(strDensity == "--h"){
                    for(int i = 0; i < frequency; i++){
                        idp_generator generator;
                        char **unchangedMatrix = generator.generateNumberSequenceForTreeCompatibilityHighDensity(species, initial_char, characters);
                        char **matrix = generator.randomConvertE0toEq(percentageOfZeroRemained, unchangedMatrix, species, characters);
                        generator.idpp2TreeCompatibilityHighDensity(matrix, species, characters, i + 1);
                    }
                }
                else{
                    cout << "Error! The command is not correct!" << endl;
                    exit(0);
                }
            }
            else{
                cout << "Error! The command is not correct!" << endl;
                exit(0);
            }
        }
        else{
            cout << "Error! The command is not correct!" << endl;
            exit(0);
        }
    }
    else if(strcmp(argv[1],"-r") == 0){
        //invoke the algorithms, there are two types:
        //1. taking seed matrix as inputs, and convert it to 0-1-? matrix for IDPP
        //2. taking matrix as inputs, no need to convert 0 to ? as the input matrices already have them
        if(argc == 10){
            //./IDPP -r -sd --p 95 --ep 0 --f 30 filepath
            string strPercentageOfZeroRemained = argv[4];
            string strEdgePromotion = argv[6];
            string strFrequency = argv[8];
            string strFilePath = argv[9];

            bool enablePromotion = false;
            if(strEdgePromotion == "1"){
                enablePromotion = true;
            }

            stringstream valuePercentageOfZeroRemained(strPercentageOfZeroRemained);
            int percentageOfZeroRemained = 0;
            valuePercentageOfZeroRemained >> percentageOfZeroRemained;
            stringstream valueFrequency(strFrequency);
            int frequency = 0;
            valueFrequency >> frequency;

            time_t execution = 0;
            long numberOfEdge = 0;

            for(int i = 0; i < frequency; i++){
                cout << "Test " << i + 1 << " is executing!" << endl;
                //read input files
                string path = strFilePath;

                idp_inputReader inputReader;
                char** seedMatrix = inputReader.readInputMatrix(path);
                int columnSize = inputReader.getColumnSize();
                int rowSize = inputReader.getRowSize();

                idp_generator generator;
                char **matrix = generator.randomConvertE1E0ToEQ(percentageOfZeroRemained, seedMatrix, rowSize, columnSize);
                columnSize = generator.getColumnSize();
                rowSize = generator.getRowSize();
                cout << columnSize << "," << rowSize << endl;

                //setup the inputs as inputs of the IDP algorithm
                idp_inputSetup inputSetup;
                unordered_map<int, fancyKStructure*> *fancyKlist = inputSetup.constructFancyKAndEdgeLists(matrix, columnSize, rowSize);

                numberOfEdge += inputSetup.getE1()->size();
                //execute the IDP algorithm
                cout << "idp is in progress" << endl;
                time_t exeStart = clock();
                idp_A_fast idpfast(fancyKlist, inputSetup.getE0(), inputSetup.getE1(), inputSetup.getEq());
                idpfast.idpProcessing(matrix, columnSize, rowSize, enablePromotion);
                time_t exeEnd = clock();
                execution += exeEnd - exeStart;

                //write results to files
                idpfast.write2file(i+1);

                //destruct original 2d matrix
                for(int j = 0 ; j < rowSize; j++){
                    delete[] matrix[j];
                }
                delete[] matrix;

                //destruct original 2d matrix
                for(int j = 0 ; j < rowSize; j++){
                    delete[] seedMatrix[j];
                }
                delete[] seedMatrix;
                cout << "Test " << i + 1 << " is done!" << endl;
            }
            cout << "idp execution time: " << (execution/(double)frequency)/CLOCKS_PER_SEC << " seconds" << endl;
            cout << "Execution finish!" << endl;
        }
        else if(argc == 8){
            //./IDPP -r -t --ep 0 --f 30 filepath

            string strEdgePromotion = argv[4];
            string strFrequency = argv[6];
            string strFilePath = argv[7];

            time_t execution = 0;
            stringstream valueFrequency(strFrequency);
            int frequency = 0;
            valueFrequency >> frequency;
            bool enablePromotion = false;
            if(strEdgePromotion == "1"){
                enablePromotion = true;
            }
            long numberOfEdge = 0;
            string basePath = strFilePath;
            for(int i = 0; i < frequency; i++){
                cout << "Test " << i + 1 << " is executing!" << endl;
                string path = basePath + to_string(i + 1);

                idp_inputReader inputReader;
                char** matrix = inputReader.readInputMatrix(path);
                int columnSize = inputReader.getColumnSize();
                int rowSize = inputReader.getRowSize();

                //setup the inputs as inputs of the IDP algorithm
                idp_inputSetup inputSetup;
                unordered_map<int, fancyKStructure*> *fancyKlist = inputSetup.constructFancyKAndEdgeLists(matrix, columnSize, rowSize);

                numberOfEdge += inputSetup.getE1()->size();
                //execute the IDP algorithm
                time_t exeStart = clock();
                idp_A_fast idpfast(fancyKlist, inputSetup.getE0(), inputSetup.getE1(), inputSetup.getEq());
                idpfast.idpProcessing(matrix, columnSize, rowSize, enablePromotion);
                time_t exeEnd = clock();
                execution += exeEnd - exeStart;

                cout << "Test " << i + 1 << " is done!" << endl;
            }

            cout << "idp execution time: " << (execution/(double)frequency)/CLOCKS_PER_SEC << " seconds" << endl;

            cout << "Execution finish!" << endl;
        }
        else{
            cout << "Error! The command is not correct!" << endl;
            exit(0);
        }

    }
    else{
        cout << "Error! The command is not correct!" << endl;
        exit(0);
    }

    return 0;
}