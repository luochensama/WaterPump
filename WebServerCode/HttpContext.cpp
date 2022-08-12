//
// Created by luochensama on 2022/3/9.
//

#include <unistd.h>
#include <cstring>
#include <iostream>
#include "Channel.h"
#include "HttpContext.h"
#include "EventLoop.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "Timer.h"
#include "Util.h"
#include "string"
#include "base/Logging.h"

std::unordered_map<std::string, std::string> MimeType::mime;
pthread_once_t MimeType::once_control;
static const int DEFAULT_EXPIRED_TIME = 2000; // ms
static const int DEFAULT_KEEPALIVE_EXPIRED_TIME = 5 * 60 * 1000;

void MimeType::init() {
    mime[".html"] = "text/html";
    mime[".avi"] = "video/x-msvideo";
    mime[".bmp"] = "image/bmp";
    mime[".c"] = "text/plain";
    mime[".doc"] = "application/msword";
    mime[".gif"] = "image/gif";
    mime[".gz"] = "application/x-gzip";
    mime[".htm"] = "text/html";
    mime[".ico"] = "image/x-icon";
    mime[".jpg"] = "image/jpeg";
    mime[".png"] = "image/png";
    mime[".txt"] = "text/plain";
    mime[".mp3"] = "audio/mp3";
    mime[".js"] = "text/js";
    mime[".css"] = "text/css";
    mime[".glb"] = "model/gltf-binary";
    mime[".json"] = "application/json";
    mime["default"] = "text/html";
}

std::string MimeType::getMime(const std::string &suffix) {
    pthread_once(&once_control, MimeType::init);
    if (mime.find(suffix) == mime.end())
        return mime["default"];
    else
        return mime[suffix];
}

HttpContext::HttpContext(EventLoop *loop, int fd)
        :
        loop_(loop),
        channel_(new Channel(loop,fd)),
        timer_(),
        fd_(fd),
        stringEndFlag_(std::string::npos),
        currentPosition_(0),
        lineEndPos_(0),
        connectionState_(CONNECTION_CONNECTED),
        processState_(STATE_PARSE_URI),
        methods_(METHOD_GET),
        httpVersion_(HTTP_11),
        parseHeaderState_(HEADER_LINE_READ),
        error_(false),
        keepAlive_(false),
        DBManager_(InfluxDBManagerFactory().produceDBManager()){
    channel_->setReadCallBack(std::bind(&HttpContext::handleRead,this));
    channel_->setWriteCallBack(std::bind(&HttpContext::handleWrite,this));
    channel_->setConnCallBack(std::bind(&HttpContext::handleConn,this));
}
HttpContext::~HttpContext() {
    close(fd_);
}
void HttpContext::reset() {
    fileName_.clear();
    currentPosition_ = 0;
    lineEndPos_ = 0;
    header_.clear();
    params_.clear();
    processState_ = STATE_PARSE_URI;
    parseHeaderState_ = HEADER_LINE_READ;
}

void HttpContext::newConnection() {
    channel_->setEvents(EPOLLIN | EPOLLET);
    loop_->addChannel(channel_.get(),DEFAULT_EXPIRED_TIME);
}

void HttpContext::handleRead() {
    if(error_ || connectionState_ != CONNECTION_CONNECTED) return;
    bool zero = false;
    ssize_t readBytes = readSocket(fd_,inputBuffer_,zero);
    if(readBytes < 0){ // 如果读取错误返回400.
        error_ = true;
        handleError(400,"Bad Request"); // handleError会关闭连接。
        return;
    }
    if(zero){
        // 读到0认为对方已经关闭连接，这边处理完最后的数据也关闭.
        // 有可能对方会关闭自身读写两端，但服务器只会处理一次，因为如果收到RST
        // 会直接调用channel的errorCallback。
        connectionState_ = CONNECTION_DISCONNECTING;
        if(!readBytes){
            return;
        } // 如果读到0又没读到数据，那么就不继续下面的过程，直接return
    }
    LOG << inputBuffer_;
    if(processState_ == STATE_PARSE_URI){
        UriState res = parseURI();
        if(res == PARSE_URI_AGAIN){
            return;
        }else if(res == PARSE_URI_ERROR){
            LOG << "FD = " << fd_ << " receive error request" << inputBuffer_ << "!!!!!!";
            error_ = true;
            handleError(400,"Bad Request");
            return;
        }else{
            processState_ = STATE_PARSE_HEADER;
        }
    }
    if(processState_ == STATE_PARSE_HEADER){
        HeaderState res = parseHeader();
        if(res == PARSE_HEADER_AGAIN){
            return;
        }else if(res == PARSE_HEADER_ERROR){
            LOG << "FD = " << fd_ << " receive error header" << inputBuffer_ << "!!!!!!";
            error_ = true;
            handleError(400,"Bad Request");
            return;
        }else{
            if(methods_ == METHOD_POST) processState_ = STATE_RECV_BODY;
            else processState_ = STATE_ANALYSIS;
        }
    }
    if(processState_ == STATE_RECV_BODY){
        int content_length = 0;
        if(header_["Content-length"].empty()){
            error_ = true;
            handleError(400, "Bad Request: Lack of argument (Content-length)");
            return;
        }else{
            content_length = std::stoi(header_["Content-length"]);
        }
        if(content_length > static_cast<int>(inputBuffer_.size())) return;
        processState_ = STATE_ANALYSIS;
    }
    if(processState_ == STATE_ANALYSIS){
        AnalysisState analysisState = analysisRequest();
        if(analysisState == ANALYSIS_SUCCESS){
            processState_ = STATE_FINISH;
        }else{
            return;
        }
    }
    if(processState_ == STATE_FINISH){
        if(!outputBuffer_.empty())
            handleWrite();
        if(!error_){
            reset();
            if(!inputBuffer_.empty()){
                handleRead();
            }
        }
    }
    if(error_) return; // write error
    uint32_t& events = channel_->getEvents();
    if(connectionState_ == CONNECTION_CONNECTED)
        events |= EPOLLIN;
}

void HttpContext::handleWrite() {
    if(error_ || connectionState_ == CONNECTION_DISCONNECTED) return;
    uint32_t& events = channel_->getEvents();
    ssize_t writen = writeSocket(fd_,outputBuffer_);
    if(writen == -1){
        error_ = true;
        events = 0;
        return;
    }
    if(!outputBuffer_.empty()) events |= EPOLLOUT;
}

void HttpContext::handleError(int errornum,std::string short_msg) {
    error_ = true;
    short_msg = " " + short_msg;
    char send_buff[4096];
    std::string body_buff, header_buff;
    body_buff += "<html><title>哎~出错了</title>";
    body_buff += "<body bgcolor=\"ffffff\">";
    body_buff += std::to_string(errornum) + short_msg;
    body_buff += "<hr><em> LuoChen</em>\n</body></html>";

    header_buff += "HTTP/1.1 " + std::to_string(errornum) + short_msg + "\r\n";
    header_buff += "Content-Type: text/html\r\n";
    header_buff += "Connection: Close\r\n";
    header_buff += "Content-Length: " + std::to_string(body_buff.size()) + "\r\n";
    header_buff += "Server: LuoChen\r\n";

    header_buff += "\r\n";
    // 错误处理不考虑writen不完的情况
    if(writeSocket(fd_, header_buff) < 0) {
//        handleClose();
        return;
    }
    writeSocket(fd_, body_buff);
//    handleClose();
}

void HttpContext::handleClose() {
    connectionState_ = CONNECTION_DISCONNECTED;
    std::shared_ptr<HttpContext> guard(shared_from_this());
    loop_->removeChannel(channel_.get());
}

void HttpContext::handleConn() {
    /*if(error_ || connectionState_ == CONNECTION_DISCONNECTED){
        std::shared_ptr<HttpContext> guard(shared_from_this()); // deleteTimer之后，HttpContext对象会被析构，所以需要guard一下
        deleteTimer();
        return;
    }*/
    deleteTimer();
    if(error_){
        handleClose();
        return;
    }
    uint32_t& events = channel_->getEvents();
    int timeout = DEFAULT_EXPIRED_TIME;
    if(keepAlive_) timeout = DEFAULT_KEEPALIVE_EXPIRED_TIME;
    if(connectionState_ == CONNECTION_DISCONNECTING){
        if(!(events & EPOLLOUT)) {
            handleClose();
            return;
        }else{
            events = 0;
            events |= EPOLLOUT;
        }
    }else{
        if((events & EPOLLOUT) && (events & EPOLLIN)){
            events = 0;
            events |= EPOLLOUT;
        }else if(events == 0) events |= EPOLLIN;
    }
    events |= EPOLLET;
    loop_->modChannel(channel_.get(),timeout);
}
void HttpContext::getParams(std::string& params){
    size_t stPos = 0;
    size_t andPos = 0;
    while(!isNotFound(andPos = params.find('&',stPos))){
        size_t ePos = params.find_first_of("=",stPos);
        if(ePos > andPos) break;
        std::string key = params.substr(stPos,ePos-stPos);
        std::string value = params.substr(ePos+1,andPos-ePos-1);
        params_[key] = value;
        stPos = andPos+1;
    }
    size_t ePos = params.find_first_of("=",stPos);
    if(!isNotFound(ePos)){
        std::string key = params.substr(stPos,ePos-stPos);
        std::string value = params.substr(ePos+1);
        params_[key] = value;
    }
}

UriState HttpContext::parseURI() {
    // 攒够一行数据再解析
    if(!receiveOneLine()) return PARSE_URI_AGAIN;

    if(!isNotFound(currentPosition_ = line_.find("GET", 0))){
        methods_ = METHOD_GET;
    }else if(!isNotFound(currentPosition_ = line_.find("POST", 0))){
        methods_ = METHOD_POST;
    }else if(!isNotFound(currentPosition_ = line_.find("HEAD", 0))){
        methods_ = METHOD_HEAD;
    }else{
        return PARSE_URI_ERROR;
    }

    currentPosition_ = line_.find('/', currentPosition_); // 找到路径的起点
    if(isNotFound(currentPosition_)) { // 没有就默认页面
        fileName_ = "index.html";
        httpVersion_ = HTTP_11;
        return PARSE_URI_SUCCESS;
    }
    currentPosition_++; // '/'之后第一个字符
    size_t endPos = line_.find(' ', currentPosition_); // 找到路径的终点
    if(isNotFound(endPos)) return PARSE_URI_ERROR;
    fileName_ = line_.substr(currentPosition_, endPos - currentPosition_);
    size_t temp = endPos;
    if(checkEmpty(fileName_)) fileName_ = "index.html";
    else{
        endPos = fileName_.find("?"); // ?之后的是参数
        if(!isNotFound(endPos)){ // 如果有
            std::string params = fileName_.substr(endPos+1);
            fileName_ = fileName_.substr(0,endPos);
            getParams(params);
        }
    }
    currentPosition_ = temp + 1; // 到了空格后一个
    if(!isNotFound(line_.find("HTTP/1.1", currentPosition_))){
        httpVersion_ = HTTP_11;
    }else if(!isNotFound(line_.find("HTTP/1.0", currentPosition_))){
        httpVersion_ = HTTP_10;
    }else{
        return PARSE_URI_ERROR;
    }

    return PARSE_URI_SUCCESS;
}

HeaderState HttpContext::parseHeader() {
    while(!inputBuffer_.empty()) {
        if (parseHeaderState_ == HEADER_LINE_READ) {
            if (!receiveOneLine()){
                return PARSE_HEADER_AGAIN;
            } // 没收到一行不处理
            currentPosition_ = line_.find(':', currentPosition_);
            if (isNotFound(currentPosition_))
                return PARSE_HEADER_ERROR;
            std::string key = std::string(line_, 0, currentPosition_);
            if (key.empty()) return PARSE_HEADER_ERROR;
            if (line_[++currentPosition_] != ' ') return PARSE_HEADER_ERROR;
            std::string value = std::string(line_, currentPosition_ + 1, lineEndPos_ - currentPosition_ - 1);
            if (value.empty()) return PARSE_HEADER_ERROR;
            header_[key] = value;
            parseHeaderState_ = HEADER_LINE_DONE;
        }
        if (parseHeaderState_ == HEADER_LINE_DONE) {
            if (inputBuffer_.size() > 1) {
                if (inputBuffer_[0] == '\r' && inputBuffer_[1] == '\n') {
                    inputBuffer_ = inputBuffer_.substr(2);
                    return PARSE_HEADER_SUCCESS;
                } else {
                    lineEndPos_ = 0;
                    parseHeaderState_ = HEADER_LINE_READ;
                }
            } else {
                return PARSE_HEADER_AGAIN;
            }
        }
    }

    return PARSE_HEADER_AGAIN;
}


AnalysisState HttpContext::analysisRequest() {
    std::string header;
    std::string body;
    if(methods_ == METHOD_POST){

    }else if(methods_ == METHOD_GET || methods_ == METHOD_HEAD){
        header += "HTTP/1.1 200 OK\r\n";
        if(header_["Connection"] == "Keep-Alive" || header_["Connection"] == "keep-alive"){
            keepAlive_ = true;
            header += std::string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=" +
                      std::to_string(DEFAULT_KEEPALIVE_EXPIRED_TIME) + "\r\n";
        }
        int dot_pos = fileName_.rfind('.');
        std::string filetype;
        if (dot_pos < 0)
            filetype = MimeType::getMime("default");
        else
            filetype = MimeType::getMime(fileName_.substr(dot_pos));

        if (fileName_.substr(0,3) == "api") {
            fileName_ = fileName_.substr(4);
        }

        struct stat sbuf;
        if (stat(fileName_.c_str(), &sbuf) < 0) {
            header.clear();
            handleError(fd_, "404 Not Found!");
            return ANALYSIS_ERROR;
        }
        int src_fd = open(fileName_.c_str(), O_RDWR, 0);
        if (src_fd < 0) {
            outputBuffer_.clear();
            handleError(fd_, "404 Not Found!");
            return ANALYSIS_ERROR;
        }
        void *mmapRet = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0);
        close(src_fd);
        if (mmapRet == (void *)-1 && fileName_ != "data.json") {
            munmap(mmapRet, sbuf.st_size);
            outputBuffer_.clear();
            handleError(fd_, "404 Not Found!");
            return ANALYSIS_ERROR;
        }
        char *src_addr = static_cast<char *>(mmapRet);
        std::string file = std::string(src_addr, src_addr + sbuf.st_size);
        if(fileName_ == "checkPoint.json" && params_.size() == 1){
            if(params_["search"].size()){
                std::string search = params_["search"];
                Json::Reader reader;
                Json::Value parseData,resultData;
                if (reader.parse(file.c_str(), parseData))
                {
                    for(int i = 0;i < parseData.size(); ++ i) {
                        for(auto name:parseData[i].getMemberNames()){
                            if(!isNotFound(parseData[i][name].asString().find(search))){
                                resultData.append(parseData[i]);
                                break;
                            }
                        }
                    }
                }
                file = resultData.toStyledString();
            }
        }
        if(fileName_ == "data.json" && params_.size() == 1 && !params_["device_id"].empty()){
            std::string device_id = params_["device_id"];
            auto res = DBManager_->queryData("2019-05-01T00:00:00Z","2019-05-01T00:20:00Z",device_id);
            size_t timePos = 0,valPos = 0;
            for(int i = 0;i < res[0].size(); ++ i){
                if(res[0][i] == "_time") timePos = i;
                else if(res[0][i] == "_value") valPos = i;
            }
            vector<string> valVec;
            vector<string> timeVec;
            size_t mx = max(valPos,timePos);
            for(int i = 1;i < res.size(); ++ i){
                if(res[i].size() < mx) continue;
                valVec.emplace_back(res[i][valPos]);
                timeVec.emplace_back(res[i][timePos]);
            }
            file += "{\n\"dataList\":[";
            for(int i = 0;i < valVec.size();++i){
                file += valVec[i];
                if(i != valVec.size()-1) file += ",";
            }
            file += "],\n\"timeList\":[";
            for(int i = 0;i < timeVec.size();++i){
                file += "\"" + timeVec[i] + "\"";
                if(i != timeVec.size()-1) file += ",";
            }
            file += "]\n}";
        }
        header += "Content-Type: " + filetype + "\r\n";
        header += "Content-Length: " + std::to_string(file.size()) + "\r\n";
        header += "Server: LuoChen's Web Server\r\n";
        // 头部结束
        header += "\r\n";
        outputBuffer_ += header;
        outputBuffer_ += file;

        munmap(mmapRet, sbuf.st_size);
        return ANALYSIS_SUCCESS;
    }

    return ANALYSIS_ERROR;
}


bool HttpContext::isNotFound(size_t pos) const {
    return pos == stringEndFlag_;
}

void HttpContext::removePre(size_t end) {
    if(end >= inputBuffer_.size()) inputBuffer_.clear();
    else inputBuffer_ = inputBuffer_.substr(end);
}

bool HttpContext::receiveOneLine() {
    bool res = !isNotFound(lineEndPos_ = inputBuffer_.find('\n', lineEndPos_));
    if(res){
        currentPosition_ = 0;
        line_ = inputBuffer_.substr(0, lineEndPos_);
        removePre(lineEndPos_ + 1);
    }else{
        lineEndPos_ = inputBuffer_.size();
    }
    return res;
}

void HttpContext::deleteTimer() {
    if(timer_.lock()){
        SP_Timer spTimer(timer_.lock());
        spTimer->deleteTimer();
        timer_.reset();
    }
}

bool HttpContext::checkEmpty(string name) {
    if(name.empty()) return true;
    if(name == "SQL" || name == "WaterPump") return true;
    return false;
}

