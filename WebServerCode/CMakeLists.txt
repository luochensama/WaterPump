cmake_minimum_required(VERSION 3.10)
project(SheepWebServer)
find_package(InfluxDB)
find_package(date)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(WebServer main.cpp base/Mutex.h base/noncopyable.h base/Condition.h base/Thread.cpp base/Thread.h base/CurrentThread.h base/CountDownLatch.h base/CountDownLatch.cpp base/FileUtil.cpp base/FileUtil.h base/LogFile.cpp base/LogFile.h base/LogStream.cpp base/LogStream.h base/AsyncLogging.cpp base/AsyncLogging.h base/Logging.cpp base/Logging.h Channel.cpp Channel.h EventLoop.cpp EventLoop.h Epoll.cpp Epoll.h EventLoopThread.cpp EventLoopThread.h Server.cpp Server.h Util.cpp Util.h EventLoopThreadPool.cpp EventLoopThreadPool.h Timer.cpp Timer.h HttpContext.cpp HttpContext.h test.cpp DataBase/InFluxDB/influxdb.h DataBase/InFluxDB/CsvReader.cpp DataBase/InFluxDB/CsvReader.h DataBase/InFluxDB/InfluxDBManager.cpp DataBase/InFluxDB/InfluxDBManager.h DataBase/DBManager.h DataBase/DBManagerFactory.h DataBase/InfluxDBManagerFactory.h DataBase/InFluxDB/AnnotatedCSVTransfer.h DataBase/InFluxDB/TransferAlgorithm.h DataBase/InFluxDB/CsvTransferAlgorithm.h DataBase/InFluxDB/AnnotatedCSVTransferFactory.h)
target_link_libraries(WebServer libjsoncpp.a InfluxData::InfluxDB date::date date::date-tz)
