cmake_minimum_required(VERSION 3.13)
project(chip8)

set(CMAKE_CXX_STANDARD 14)

add_executable(chip8 main.cpp chip8.cpp chip8.h game.cpp game.h debugger.cpp debugger.h)

find_package(SFML REQUIRED system window graphics network audio)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio)