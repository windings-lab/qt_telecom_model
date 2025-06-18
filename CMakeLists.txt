cmake_minimum_required(VERSION 3.16)
project(telecom_tree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	find_package(Qt6 REQUIRED COMPONENTS Core CorePrivate Widgets WidgetsPrivate Sql)
else()
	find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)
endif()

qt_standard_project_setup()

add_subdirectory(src)
