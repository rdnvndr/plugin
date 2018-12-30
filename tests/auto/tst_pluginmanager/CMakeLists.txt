cmake_minimum_required(VERSION 3.10)
project(tst_pluginmanager VERSION 1.0.0)

set(PRODUCTNAME "TST_PLUGINMANAGER")
set(DESCRIPTION "TST_PLUGINMANAGER")
set(COMPANY     "RTPTechGroup")
set(COPYRIGHT   "Copyright (C) RTPTechGroup")
set(LIBRARIES
    plugin
)
include(../auto.cmake)

find_package(Qt5Core)
target_link_libraries(${PROJECT_NAME} Qt5::Core)

find_package(Qt5Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

find_package(Qt5Test)
target_link_libraries(${PROJECT_NAME} Qt5::Test)
