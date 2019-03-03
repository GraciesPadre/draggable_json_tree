cmake_minimum_required(VERSION 3.10)
project(json_tree)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/usr/lib64/qt5/mkspecs/features/data/cmake")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_executable(json_tree main.cpp qjsonmodel.cpp qjsonmodel.h pet_tree_item.cpp pet_tree_item.cpp pet_tree_model.cpp pet_tree_model.h)

target_link_libraries(json_tree Qt5::Core)
target_link_libraries(json_tree Qt5::Widgets)


