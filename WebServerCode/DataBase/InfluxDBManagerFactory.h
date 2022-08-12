//
// Created by luochensama on 22-7-3.
//

#ifndef SHEEPWEBSERVER_INFLUXDBMANAGERFACTORY_H
#define SHEEPWEBSERVER_INFLUXDBMANAGERFACTORY_H
#include "DBManagerFactory.h"
#include "InFluxDB/InfluxDBManager.h"

class InfluxDBManagerFactory : public DBManagerFactory{
public:
    DBManager* produceDBManager() override{
        InfluxDBManager* res = new InfluxDBManager("waterpump","checkpoint");
        res->connect();

        return res;
    }
};
#endif //SHEEPWEBSERVER_INFLUXDBMANAGERFACTORY_H
