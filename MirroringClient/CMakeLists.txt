cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

project(MirroringClient)

find_package(Boost CONFIG REQUIRED date_time system thread filesystem regex)
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
add_executable(MirroringClient main.cpp)
target_link_libraries (MirroringClient pthread)
target_link_libraries(MirroringClient ${OpenCV_LIBS})
