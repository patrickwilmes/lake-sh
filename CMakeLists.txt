cmake_minimum_required(VERSION 3.16)
project(lake_sh)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# As wsl seems to behave differently than native linux
# we have an option to enable the wsl compatibility or
# disable it. It's disabled by default.
# Use -DEnableWslCompatibility=ON to enable
option(EnableWslCompatibility "EnableWslCompatibility" OFF)

add_subdirectory(AK)
add_subdirectory(Libraries)
add_subdirectory(Shell)
