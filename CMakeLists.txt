cmake_minimum_required(VERSION 3.5)

project(
    RealmOfUbuntu
    VERSION 1.0
    LANGUAGES CXX
)

find_package(Qt5 REQUIRED COMPONENTS Core Quick Sql)
if(NOT Qt5_FOUND)
    message(FATAL_ERROR "Qt5 not found! Please install Qt5.")
endif()

add_executable(RealmOfUbuntu main.cpp)

target_sources(RealmOfUbuntu
    PRIVATE
        hero.cpp
        hero.h
        database.cpp
        database.h
        quest.cpp
        quest.h
)

target_link_libraries(RealmOfUbuntu
    PRIVATE
        Qt5::Core
        Qt5::Quick
        Qt5::Sql
)

configure_file(${CMAKE_SOURCE_DIR}/enemy_data.txt ${CMAKE_BINARY_DIR}/enemy_data.txt)
configure_file(${CMAKE_SOURCE_DIR}/quest_data.txt ${CMAKE_BINARY_DIR}/quest_data.txt)
