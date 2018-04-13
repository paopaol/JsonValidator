cmake_minimum_required(VERSION 3.6)


project(JsonValidator)


include_directories(include)

option(build_example "enable build example or not" ON)
option(build_test "enable build unit test or not" ON)
option(SUPPORT_JSONCPP "support jsoncpp [https://github.com/open-source-parsers/jsoncpp]" ON)
option(SUPPORT_QTJSON "support qt json" OFF)


if((SUPPORT_JSONCPP AND SUPPORT_QTJSON) OR 
    (NOT SUPPORT_JSONCPP AND NOT SUPPORT_QTJSON))
    message(FATAL_ERROR "Must define [SUPPORT_JSONCPP or SUPPORT_QTJSON] Only one of them can be defined")
endif()

if(SUPPORT_JSONCPP)
    add_definitions(-DUSE_JSON_CPP)
    include_directories(jsoncpp)
    add_subdirectory(jsoncpp)
else()
    add_definitions(-DUSE_QT_JSON)
endif()

if(build_example)
    add_subdirectory(example)
endif()

if(build_test)
    enable_testing()
    add_subdirectory(test)
endif()