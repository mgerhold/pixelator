cmake_minimum_required(VERSION 3.28)
project(pixelator)

set(CMAKE_CXX_STANDARD 26)

include(${PROJECT_SOURCE_DIR}/cmake/lto.cmake)
try_enable_link_time_optimizations()

include(${PROJECT_SOURCE_DIR}/dependencies.cmake)
setup_dependencies()

include(${PROJECT_SOURCE_DIR}/project_options.cmake)

add_subdirectory(vendor)
add_subdirectory(src bin)
