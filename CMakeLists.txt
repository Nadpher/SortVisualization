cmake_minimum_required(VERSION 3.17)

project ("SortVisualization")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} "src/main.cpp" "src/Engine.cpp" "src/Engine.h" "src/Coord.h")

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${SDL2_LIBRARY})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIR})
