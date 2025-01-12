cmake_minimum_required(VERSION 3.10)

#set(CMAKE_C_COMPILER "path/to/your/c/compiler")
#set(CMAKE_CXX_COMPILER "path/to/your/cxx/compiler")

project(GamepadValues VERSION 0.1.0 LANGUAGES C CXX)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_GENERATOR_PLATFORM x64)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_GENERATOR_PLATFORM x64)
endif()

#set_property(TARGET gamepad-values PROPERTY CXX_STANDARD 20)

include (CTest)
enable_testing()

add_executable(gamepad-values src/main.cpp src/gamepad.cpp)
target_include_directories(gamepad-values PRIVATE include)

# Link against the XInput library
target_link_libraries(gamepad-values XInput)

#include_directories(include)

#file(GLOB SOURCES "src/*.cpp")

#add_executable(gamepad-values ${SOURCES})