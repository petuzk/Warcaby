cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(draughts)

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_STANDARD 11)

find_package(raylib 3.0 QUIET)

# This part downloads raylib and builds it if it's not installed
if (NOT raylib_FOUND)
  include(FetchContent)

  FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/releases/download/3.0.0/raylib-noexamples-3.0.0.tar.gz
  )

  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)

    # don't build the supplied examples
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

    # build raylib
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})

  endif()

endif()

include_directories(.)
file(GLOB Sources src/main.cpp src/*/*.cpp src/*/*/*.cpp)
add_executable(${CMAKE_PROJECT_NAME} ${Sources})
target_link_libraries(${CMAKE_PROJECT_NAME} raylib)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/resources)