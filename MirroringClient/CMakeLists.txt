cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
include("tools/gate/cmake/HunterGate.cmake")
HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.314.tar.gz"
        SHA1 "95c47c92f68edb091b5d6d18924baabe02a6962a"
        #FILEPATH "/home/crestonix/CLionProjects/MirroringClient/tools/gate/cmake/configs/config.cmake"
)



project(MirroringClient)

hunter_add_package(Boost COMPONENTS date_time system thread filesystem regex)
hunter_add_package(OpenCV)
find_package(Boost CONFIG REQUIRED date_time system thread filesystem regex)
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
add_executable(MirroringClient main.cpp)
target_link_libraries (MirroringClient pthread)
target_link_libraries(MirroringClient ${OpenCV_LIBS})