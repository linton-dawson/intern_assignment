set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 2.8)
project( InkersApp )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/filter_vids)
add_executable(InkersApp main.cpp )
target_link_libraries( InkersApp ${OpenCV_LIBS} stdc++fs)

