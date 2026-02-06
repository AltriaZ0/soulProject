cmake_minimum_required(VERSION 3.15)
project(TouhouProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML CONFIG REQUIRED COMPONENTS Graphics Window System Main)

include_directories(include)

file(GLOB_RECURSE SOURCES "game_src/*.cpp")

add_executable(soulProject ${SOURCES})

target_link_libraries(soulProject PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Main)