cmake_minimum_required(VERSION 3.28)
project(CPP3_Tetris LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

include_directories(../../../brick_game/tetris)

FILE(COPY ../../../../misc DESTINATION .)

add_executable(CPP3_Tetris
        main.cpp
        mainwindowtetris.cpp mainwindowtetris.h mainwindowtetris.ui
        ../../../brick_game/tetris/tetris.c
        ../../../brick_game/tetris/tetris.h
)

target_link_libraries(CPP3_Tetris PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

set_target_properties(CPP3_Tetris PROPERTIES RUNTIME_OUTPUT_DIRECTORY ".")
set_target_properties(CPP3_Tetris PROPERTIES RUNTIME_OUTPUT_NAME "Tetris_GUI")
#TODO : Change the destination to the correct path
install(TARGETS CPP3_Tetris DESTINATION .)
