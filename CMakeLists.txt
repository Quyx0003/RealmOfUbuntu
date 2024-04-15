cmake_minimum_required(VERSION 3.5)

project(
    MUNCH
    VERSION 1.0
    LANGUAGES CXX
)

find_package(Qt5 REQUIRED COMPONENTS Core Quick Sql)
if(NOT Qt5_FOUND)
    message(FATAL_ERROR "Qt5 not found! Please install Qt5.")
endif()
    
add_executable(MUNCH main.cpp)

target_link_libraries(MUNCH Qt5::Core Qt5::Quick Qt5::Sql)

