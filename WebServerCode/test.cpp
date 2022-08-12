////
//// Created by luochensama on 22-5-30.
////
//#include "include/json/json.h"
//#include "bits/stdc++.h"
//#include <fcntl.h>
//#include <sys/mman.h>
//#include <sys/stat.h>
//#include <unistd.h>
//
//using namespace std;
//std::unordered_map<std::string,std::string> params_;
//int main(){
//    struct stat sbuf;
//    string fileName_ = "checkPoint.json";
//    stat(fileName_.c_str(), &sbuf);
//    int src_fd = open(fileName_.c_str(), O_RDWR, 0);
//    void *mmapRet = mmap(NULL, sbuf.st_size, PROT_WRITE, MAP_PRIVATE, src_fd, 0);
//    close(src_fd);
//    char *src_addr = static_cast<char *>(mmapRet);
//    std::string file = std::string(src_addr, src_addr + sbuf.st_size);
//    if(fileName_ == "checkPoint.json" && params_.size() == 1){
//        if(params_["search"].size()){
//            Json::Reader reader;
//            Json::Value parseData;
//            if (reader.parse(file.c_str(), parseData))
//            {
//                for(auto iter = parseData.begin(); iter != parseData.end(); iter++){
//                    (*iter).asString();
//                }
//            }
//        }
//    }
//
//    return 0;
//}