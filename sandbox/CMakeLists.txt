cmake_minimum_required(VERSION 3.10...3.21)

project(Sandbox VERSION 0.01 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

add_executable(sandbox main.cpp)
target_link_libraries(sandbox PRIVATE york)

set_target_properties(sandbox PROPERTIES
DEBUG_POSTFIX          "_debug"
RELEASE_POSTFIX        "_release"
RELWITHDEBINFO_POSTFIX "_relwithdebinfo")
