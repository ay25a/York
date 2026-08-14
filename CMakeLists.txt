cmake_minimum_required(VERSION 3.10...3.30)

project(York VERSION 0.01 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Build Type: " ${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

option(YORK_BUILD_TESTS "Build engine test executable" ON)
option(PLATFORM_X11 "select x11 as the target platform" OFF)
option(PLATFORM_WAYLAND "select wayland as a display platform" ON)

if(NOT UNIX)
message(FATAL_ERROR "Only Unix based platforms is supported for now!")
endif()

if(UNIX AND PLATFORM_X11)
  message(STATUS "Platform: X11")
  set(PLATFORM_WAYLAND OFF)
elif(UNIX AND PLATFORM_WAYLAND)
  message(STATUS "Platform: Wayland")
endif()

set(YORK_SOURCES)
set(YORK_HEADERS)

include(utils/utils.cmake)
include(core/core.cmake)
include(servers/servers.cmake)
include(vendor/vendor.cmake)

add_library(york STATIC ${YORK_SOURCES} ${YORK_HEADERS})
target_include_directories(york PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
target_link_libraries(york PUBLIC spdlog)

if(NOT MSVC)
  target_compile_options(york PRIVATE -fno-exceptions)
else()
  target_compile_options(york PRIVATE /EHs-c-)
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  target_compile_definitions(york PUBLIC NODEBUG YE_RELEASE)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  target_compile_definitions(york PUBLIC DEBUG YE_DEBUG)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
  target_compile_definitions(york PUBLIC YE_REL_WITH_DEB_INFO)
else()
  message(FATAL_ERROR "Build type is undefined! Can only be 'Release', 'Debug', or 'RelWithDebInfo'.")
endif()


if(YORK_BUILD_TESTS)
message(STATUS "Building Engine Tests is enabled")
enable_testing()
add_subdirectory(tests)
endif()

add_subdirectory(sandbox)

if(UNIX)
  find_package(PkgConfig REQUIRED)
endif()

if(UNIX AND PLATFORM_WAYLAND)
  pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)
  target_include_directories(york PUBLIC ${WAYLAND_CLIENT_INCLUDE_DIRS})
  target_link_libraries(york PUBLIC ${WAYLAND_CLIENT_LIBRARIES})

  include(platform/wayland/wayland.cmake)
else()
message(FATAL_ERROR "only wayland is supported for now!")
endif()
