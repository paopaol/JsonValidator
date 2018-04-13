

include(ExternalProject)

set(path_of_googletest ${CMAKE_CURRENT_BINARY_DIR}/thrd/googletest)
ExternalProject_Add(
    google
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest
    BUILD_IN_SOURCE 0
    INSTALL_DIR ${path_of_googletest}
    CMAKE_ARGS -DBUILD_GTEST=ON -Dgtest_force_shared_crt=ON -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    BUILD_COMMAND cmake
    
)


set(GTEST_LIBRARY ${path_of_googletest}/lib/gtest)
set(GTEST_INCLUDE_DIR ${path_of_googletest}/include)
set(GTEST_MAIN_LIBRARY ${path_of_googletest}/lib/gtest_main)
set(GTEST_ROOT ${path_of_googletest})
find_package(GTest REQUIRED)


add_executable(jsoncpp_test jsoncpp_test.cpp)
target_link_libraries(jsoncpp_test jsoncpp GTest::GTest GTest::Main)
add_test(AllTestsInFoo jsoncpp_test)
#GTEST_ADD_TESTS(jsoncpp_test "" AUTO)


