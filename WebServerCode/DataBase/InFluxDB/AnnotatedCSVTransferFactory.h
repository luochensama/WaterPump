//
// Created by luochensama on 22-7-3.
//

#ifndef SHEEPWEBSERVER_ANNOTATEDCSVTRANSFERFACTORY_H
#define SHEEPWEBSERVER_ANNOTATEDCSVTRANSFERFACTORY_H
#include "AnnotatedCSVTransfer.h"
#include "iostream"
#include "CsvTransferAlgorithm.h"
class AnnotatedCSVTransferFactory{
public:
    static AnnotatedCSVTransfer* produceAnnotatedCSVTransfer(std::string fileType){
        if(fileType == "csv"){
            AnnotatedCSVTransfer* res = new AnnotatedCSVTransfer(new CsvTransferAlgorithm("waterpump"));
            return res;
        }
        return nullptr;
    }
};

#endif //SHEEPWEBSERVER_ANNOTATEDCSVTRANSFERFACTORY_H
