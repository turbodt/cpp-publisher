cmake_minimum_required(VERSION 3.16)
project(
  "cpp-publisher" LANGUAGES CXX
  VERSION 0.0.1
  DESCRIPTION "Implements the publisher pattern"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# before add any add_subdirectory
enable_testing()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)
