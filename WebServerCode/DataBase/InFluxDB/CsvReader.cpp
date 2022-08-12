//
// Created by luochensama on 22-6-1.
//

#include "CsvReader.h"

vector<vector<string>> CsvReader::parseCsv(string path) {
    ifstream inputFile( path ,ios::in);
    vector<vector<string>> res;
    string line;
    while(true){
        getline(inputFile,line);
        if(line.empty()) break;
        vector<string> lineVec;
        size_t current = -1,pre = 0;
        while(true){
            pre = current + 1;
            current = line.find(',',current + 1);
            if(current == string::npos){
                lineVec.emplace_back(line.substr(pre));
                break;
            }
            lineVec.emplace_back(line.substr(pre,current-pre));
        }
        res.emplace_back(move(lineVec));
    }

    return res;
}

vector<vector<string>> CsvReader::parseStringCsv(string &rawData) {
    vector<vector<string>> res;
    stringstream inputFile(rawData);
    string line;
    while(true){
        getline(inputFile,line);
        if(line.empty()) break;
        vector<string> lineVec;
        size_t current = -1,pre = 0;
        while(true){
            pre = current + 1;
            current = line.find(',',current + 1);
            if(current == string::npos){
                lineVec.emplace_back(line.substr(pre));
                break;
            }
            lineVec.emplace_back(line.substr(pre,current-pre));
        }
        res.emplace_back(move(lineVec));
    }

    return res;
}