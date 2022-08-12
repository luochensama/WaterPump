//
// Created by luochensama on 22-7-3.
//

#ifndef SHEEPWEBSERVER_ANNOTATEDCSVTRANSFER_H
#define SHEEPWEBSERVER_ANNOTATEDCSVTRANSFER_H
#include <utility>

#include "TransferAlgorithm.h"

class AnnotatedCSVTransfer{
public:
    explicit AnnotatedCSVTransfer(TransferAlgorithm* algorithm):algorithm_(algorithm){

    }
    void transfer(std::string source_,std::string target_){
        algorithm_->transfer(std::move(source_),std::move(target_));
    }
private:
    TransferAlgorithm* algorithm_;
};
#endif //SHEEPWEBSERVER_ANNOTATEDCSVTRANSFER_H
