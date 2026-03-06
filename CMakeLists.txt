cmake_minimum_required(VERSION 3.16)
project(cpp_gui_postgres)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(app
    src/main.cpp
    gui/mainwindow.cpp
    database/db.cpp
    config/env.cpp
    services/userservice.cpp
    repositories/userrepository.cpp
)

target_link_libraries(app Qt6::Widgets pqxx)