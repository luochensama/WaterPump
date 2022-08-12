//
// Created by luochensama on 2022/3/6.
//

#include <sys/time.h>
#include "Logging.h"

std::string Logger::filename_ = "./WebServer.log";

Logger::Impl::Impl(const char *filename, int line):
stream_(),
filename_(filename),
line_(line){

}

void Logger::Impl::formatTime() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    time_t time = tv.tv_sec;
    auto tm = localtime(&time);
    char str[26] = {0};
    size_t len = strftime(str,sizeof str,"%Y-%m-%d %H:%M:%S",tm);
    sprintf(str+len,".%ld\n",tv.tv_usec);
    stream_ << str;
}

Logger::Logger(const char *filename, int line) : impl_(filename,line), asyncLogging_(AsyncLogging::getInstance()){
    impl_.formatTime();
}

Logger::~Logger() {
    impl_.stream_ << " -- " << impl_.filename_ << ":" << impl_.line_ << "\n";
    const LogStream::Buffer& buf(impl_.stream_.buffer());
    asyncLogging_.append(buf.data(),buf.length());
}
