//
// Created by luochensama on 22-7-3.
//

#ifndef SHEEPWEBSERVER_DBMANAGERFACTORY_H
#define SHEEPWEBSERVER_DBMANAGERFACTORY_H
#include "DBManager.h"
class DBManagerFactory{
public:
    virtual DBManager* produceDBManager()=0;
};
#endif //SHEEPWEBSERVER_DBMANAGERFACTORY_H
