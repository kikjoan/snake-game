cmake_minimum_required(VERSION 3.28)
project(CPP3_BrickGame_v2_0_1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

include_directories(../../../brick_game/snake)

FILE(COPY ../../../../misc DESTINATION .)

add_executable(CPP3_BrickGame_v2_0_1
        ../../../brick_game/snake/snake_model.cpp
        ../../../brick_game/snake/snake_model.h
        main.cpp
        mainwindow.h mainwindow.cpp mainwindow.ui
        ../../../controller/snake_controller.h
        snake_segment.h
)

target_link_libraries(CPP3_BrickGame_v2_0_1 PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

set_target_properties(CPP3_BrickGame_v2_0_1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ".")
set_target_properties(CPP3_BrickGame_v2_0_1 PROPERTIES RUNTIME_OUTPUT_NAME "Snake_GUI")

install(TARGETS CPP3_BrickGame_v2_0_1 DESTINATION .)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/misc DESTINATION .)
