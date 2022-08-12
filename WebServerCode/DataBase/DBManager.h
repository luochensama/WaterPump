//
// Created by luochensama on 22-7-3.
//

#ifndef SHEEPWEBSERVER_DBMANAGER_H
#define SHEEPWEBSERVER_DBMANAGER_H

#include "iostream"
#include "vector"
class DBManager{
public:
    virtual bool addFileToDB(std::string path) = 0;
    virtual std::vector<std::vector<std::string>> queryData(const std::string& start_time, const std::string& end_time, const std::string& device_id) = 0;
    virtual void connect()=0;
};

#endif //SHEEPWEBSERVER_DBMANAGER_H
