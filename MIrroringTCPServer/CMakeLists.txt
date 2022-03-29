cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

project(MIrroringTCPServer)

find_package(Boost CONFIG REQUIRED date_time system thread filesystem regex)
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
add_executable(MIrroringTCPServer main.cpp session_logger.cpp session_logger.h
                                  logging_queue.cpp logging_queue.h
                                  LoggerInfo.h)
target_link_libraries (MIrroringTCPServer pthread)
target_link_libraries(MIrroringTCPServer ${OpenCV_LIBS})
