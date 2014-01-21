project(test)
cmake_minimum_required(VERSION 2.6)

include_directories($ENV{JSONCPP_HOME}/include $ENV{CURL_HOME}/include)
link_directories($ENV{JSONCPP_HOME}/libs/ $ENV{CURL_HOME}/build/lib)
add_definitions(-std=c++0x)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(source
	translate.cc)
add_executable(translate ${source})
target_link_libraries(translate json_linux-gcc)
target_link_libraries(translate curl)
