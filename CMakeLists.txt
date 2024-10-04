cmake_minimum_required(VERSION 3.29)
project(MyJsonServer)

set(CMAKE_CXX_STANDARD 20)
find_package(Boost 1.86.0 REQUIRED COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIRS})

set(JSONCPP_INC "/Users/yinyanjiang/jsoncpp-1.9.6/include")
set(JSONCPP_LIB "/Users/yinyanjiang/jsoncpp-1.9.6/build/lib")
link_directories(${JSONCPP_LIB})
include_directories(${JSONCPP_INC})

add_executable(MyJsonServer main.cpp
        CServer.cpp
        CServer.h
        CSession.cpp
        CSession.h
        MsgNode.cpp
        MsgNode.h
        RecvNode.cpp
        RecvNode.h
        SendNode.cpp
        SendNode.h
        const.h)
target_link_libraries(MyJsonServer libjsoncpp.26.dylib)
