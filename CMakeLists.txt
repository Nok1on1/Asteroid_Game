cmake_minimum_required(VERSION 3.28)
project(Asteroids)

set(CMAKE_CXX_STANDARD 17)

# Find SFML
find_package(SFML 2.5 REQUIRED graphics window system)

add_executable(Asteroids main.cpp Headers/Asteroid.h
        Headers/Player.h
        Headers/extendedmath.h
        Headers/UI.h
        Headers/bullet.h)

target_link_libraries(Asteroids sfml-graphics sfml-window sfml-system)
