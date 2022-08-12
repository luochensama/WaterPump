//
// Created by luochensama on 2022/3/4.
//

#pragma once
#include <cstring>
#include "string"
#include "noncopyable.h"

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

template <int MAX_SIZE>
class FixedBuffer{
public:
    FixedBuffer():cur_(data_){};

    void append(const char* buf,size_t len){
        if(avail() > static_cast<int>(len)) {
            memcpy(cur_, buf, len);
            cur_ += len;
        }
    }
    const char * data() const {return data_;};
    char* current(){return cur_;};
    int length() const {return static_cast<int>(cur_ - data_);};

    void add(size_t len){cur_+=len;};
    int avail(){return static_cast<int>(end()-cur_);};
    void reset(){cur_ = data_;};
    void bzero(){memset(data_,0,sizeof data_);}

private:
    const char* end(){return data_ + sizeof data_;}

    char data_[MAX_SIZE];
    char* cur_;
};

class LogStream : noncopyable{
public:
    typedef FixedBuffer<kSmallBuffer> Buffer;

    LogStream& operator<<(bool v) {
        buffer_.append(v?"1":"0",1);
        return *this;
    }

    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int);
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);

    LogStream& operator<<(float v) {
        *this << static_cast<double>(v);
        return *this;
    }
    LogStream& operator<<(double);
    LogStream& operator<<(long double);

    LogStream& operator<<(char v) {
        buffer_.append(&v,1);
        return *this;
    }

    LogStream& operator<<(const char* str) {
        if(str == nullptr)
            buffer_.append("(null)",6);
        else
            buffer_.append(str,strlen(str));
        return *this;
    }

    LogStream& operator<<(const unsigned char* str) {
        return operator<<(reinterpret_cast<const char*>(str));
    }

    LogStream& operator<<(const std::string& v) {
        buffer_.append(v.c_str(),v.size());
        return *this;
    }
    void append(const char*data,int len){buffer_.append(data,len);}
    const Buffer& buffer(){return buffer_;}
    void reset(){buffer_.reset();}

private:
    template <typename T>
    void formatInteger(T);

    Buffer buffer_;
    static const int kMaxNumericSize = 32;

};


