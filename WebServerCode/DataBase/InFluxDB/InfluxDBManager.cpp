//
// Created by luochensama on 22-6-1.
//

#include "InfluxDBManager.h"
#include "CsvReader.h"
#include "vector"
#include "iostream"
#include "influxdb.h"
#include "date/date.h"
#include "AnnotatedCSVTransferFactory.h"

using namespace std;
using namespace date;

bool InfluxDBManager::addFileToDB(string source) {
    string fileType;
    size_t pos = source.rfind(".");
    if(pos == string::npos) return false;
    fileType = source.substr(pos+1);
    string target_ = source.substr(0,pos) + "_.csv";
    AnnotatedCSVTransferFactory::produceAnnotatedCSVTransfer(fileType)->transfer(source,target_);
    CsvReader reader = CsvReader::getCsvReader();

    auto input = reader.parseCsv(target_);
    if(input[0].size() != dataType.size()) return false;
    vector<string> dataName;
    for(int i = 0; i < input[0].size(); ++ i){
        dataName.push_back(input[0][i]);
    }
    int tagPos = 0,timePos = 0,firstPos = -1;
    for(int i = 0;i < dataType.size(); ++i){
        if(dataType[i] == "tag"){
            tagPos = i;
        }else if(dataType[i].substr(0,8) == "dateTime"){
            timePos = i;
        }else if(firstPos == -1){
            firstPos = i;
        }
    }
    if(firstPos == -1) return false;

    istringstream infile;
    sys_seconds tp;
    string&& pre = "";
    unsigned long long timeStamp = 0;
    int count = 0;
    for(int i = 1;i < input.size(); ++i){
        auto row = input[i];
        if(row[timePos] != pre){
            pre = row[timePos];
            infile = istringstream(pre);
            infile >> parse("%FT%TZ", tp);
            timeStamp = tp.time_since_epoch().count()*1000000000;
        }
        influxdb_cpp::builder().meas(measurement).tag(dataName[tagPos],row[tagPos]).field(dataName[firstPos],row[firstPos]).timestamp(timeStamp).post_http(*con);
    }

    return false;
}
void InfluxDBManager::connect(){
//    con = make_shared<influxdb_cpp::server_info>(influxdb_cpp::server_info("localhost", 8086, "influx", "Bxr11vo-J7S18adr8VlYx1qC3sQ5vnZpbJcHwvGrZ8zULU6m6a3AQ_OjHfzT33AOlYMAHr1RY9NSLyimP5QDCg==", bucket));
    con = make_shared<influxdb_cpp::server_info>(influxdb_cpp::server_info("localhost", 8086, "influx", "qb9aOYvJBGj8B3DJvahFo5n_A-vRxD3yr0urWhLpFaPhnBhOPNgm1T3owPrwdorzxkAJeFdjNKcjvUPlSFgkNw==", bucket));
}
vector<vector<string>> InfluxDBManager::queryData(const string& start_time, const string& end_time, const string& device_id) {
    string query = "from(bucket:\\\"" + bucket + "\\\") |> range(start:" + start_time + ", stop: " + end_time + ") |> filter(fn: (r) => r.device_id == \\\"" + device_id  + "\\\")";
    string resp;
    influxdb_cpp::flux_query(resp,query,*con);
    CsvReader csvReader = CsvReader::getCsvReader();
    auto str = csvReader.parseStringCsv(resp);

    return str;
}

bool InfluxDBManager::transToStd(vector<vector<string>> &&csvData, string& output) {
    vector<string> dataName;
    for(int i = 0; i < csvData[0].size(); ++ i){
        dataName.push_back(csvData[0][i]);
    }
    if(dataName.size() != dataType.size()) return false;
    output += "#constant measurement," + measurement + "\n";
    output += "#datatype ";
    for(int i = 0;i < dataType.size(); ++ i){
        output += dataType[i];
        if(i == dataType.size()-1) output += "\n";
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

    return true;
}
