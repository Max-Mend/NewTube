cmake_minimum_required(VERSION 3.28)
project(NewTube)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui WebEngineWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(NewTube
        main.cpp
        core/MainWindow.cpp
        core/MainWindow.h
        resources.qrc
)

target_link_libraries(NewTube
        Qt6::Widgets
        Qt6::Core
        Qt6::Gui
        Qt6::WebEngineWidgets
)