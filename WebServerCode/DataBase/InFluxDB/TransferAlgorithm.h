//
// Created by luochensama on 22-7-3.
//

#ifndef SHEEPWEBSERVER_TRANSFERALGORITHM_H
#define SHEEPWEBSERVER_TRANSFERALGORITHM_H
#include "vector"
#include "iostream"

class TransferAlgorithm {
public:
    virtual bool transfer(std::string source_,std::string target_)=0;
};


#endif //SHEEPWEBSERVER_TRANSFERALGORITHM_H
