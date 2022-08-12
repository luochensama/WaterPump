//
// Created by luochensama on 22-6-1.
//

#ifndef UNTITLED_INFLUXDBSQLMANAGER_H
#define UNTITLED_INFLUXDBSQLMANAGER_H
#include "influxdb.h"
#include "../DBManager.h"

#include <utility>
#include <memory>
#include "iostream"
using namespace std;
class InfluxDBManager : public DBManager {
public:
    explicit InfluxDBManager(string bucket_, string measurement_, vector<string> type_=vector<string>()):
    DBManager(),
    bucket(std::move(bucket_)),
    measurement(move(measurement_)),
    dataType(move(type_))
    {};
    void connect() override;
    bool addFileToDB(string source) override;
    vector<vector<string>> queryData(const string& start_time, const string& end_time, const string& device_id) override;

private:
    shared_ptr<influxdb_cpp::server_info> con;
    string bucket;
    string measurement;
    vector<string> dataType;
    bool transToStd(vector<vector<string>> &&csvData,string& output);
};


#endif //UNTITLED_INFLUXDBSQLMANAGER_H
