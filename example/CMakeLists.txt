

add_executable(example_jsoncpp example_jsoncpp.cpp)

if(SUPPORT_JSONCPP)
    
    target_link_libraries(example_jsoncpp jsoncpp)
else()
    
    #fixme qt
endif()