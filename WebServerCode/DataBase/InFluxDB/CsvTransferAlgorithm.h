//
// Created by luochensama on 22-7-3.
//

#ifndef SHEEPWEBSERVER_CSVTRANSFERALGORITHM_H
#define SHEEPWEBSERVER_CSVTRANSFERALGORITHM_H
#include "TransferAlgorithm.h"

#include <utility>
#include <fstream>
#include "vector"
#include "iostream"
#include "CsvReader.h"

using namespace std;
class CsvTransferAlgorithm:public TransferAlgorithm{
public:
    CsvTransferAlgorithm(string measurement,vector<string> dataType=vector<string>()):
    dataType_(std::move(dataType)),
    measurement_(std::move(measurement)){}
    bool transfer(std::string source_,std::string target_) override{
        string output;
        CsvReader reader = CsvReader::getCsvReader();
        auto csvData = reader.parseCsv(source_);
        vector<string> dataName;
        for(int i = 0; i < csvData[0].size(); ++ i){
            dataName.push_back(csvData[0][i]);
        }
        output += "#constant measurement," + measurement_ + "\n";
        output += "#datatype ";
        for(int i = 0;i < dataType_.size(); ++ i){
            output += dataType_[i];
            if(i == dataType_.size()-1) output += "\n";
            else output += ",";
        }
        for(int i = 0;i < dataName.size(); ++ i){
            output += dataName[i];
            if(i == dataName.size()-1) output += "\n";
            else output += ",";
        }
        for(int i = 1;i < csvData.size(); ++i){
            for(int j = 0;j < csvData[i].size(); ++ j){
                output += csvData[i][j];
                if(j == csvData[i].size()) output += "\n";
                else output += ',';
            }
        }
        ofstream outFile;
        outFile.open(target_);
        outFile << output;
        outFile.close();

        return true;
    };
private:
    vector<string> dataType_;
    string measurement_;
};

#endif //SHEEPWEBSERVER_CSVTRANSFERALGORITHM_H
