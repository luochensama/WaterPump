//
// Created by luochensama on 22-6-1.
//

#ifndef UNTITLED_CSVREADER_H
#define UNTITLED_CSVREADER_H

#include <sstream>
#include "iostream"
#include "fstream"
#include "vector"
using namespace std;
class CsvReader {
public:
    CsvReader()= default;
    static CsvReader getCsvReader(){
        static CsvReader globalCsvReader;
        return globalCsvReader;
    }
    vector<vector<string>> parseCsv(string path);
    vector<vector<string>> parseStringCsv(string& rawData);
};


#endif //UNTITLED_CSVREADER_H
