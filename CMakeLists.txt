cmake_minimum_required(VERSION 3.10...3.21)

set(YORK_SOURCES)
set(YORK_HEADERS)

include(utils/utils.cmake)
include(core/core.cmake)

project(York VERSION 0.01 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Build Type: " ${CMAKE_BUILD_TYPE})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

add_library(york STATIC ${YORK_SOURCES} ${YORK_HEADERS})
target_include_directories(york PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")

if(NOT MSVC)
  target_compile_options(york PRIVATE -fno-exceptions)
else()
  target_compile_options(york PRIVATE /EHs-c-)
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  target_compile_definitions(york PUBLIC NODEBUG)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  target_compile_definitions(york PUBLIC DEBUG YE_DEBUG)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
  target_compile_definitions(york PUBLIC YE_REL_WITH_DEB_INFO)
endif()

include(vendor/vendor.cmake)
add_subdirectory(sandbox)
