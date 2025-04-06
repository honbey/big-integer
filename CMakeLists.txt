# CMake Version
cmake_minimum_required(VERSION 4.0)
# Project Name
project(BigInteger)

# Export compile_commands.json for Neovim IDE
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_BUILD_TYPE "Debug")
# set(SRC_LIST src/main.cpp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# Add Source Files
aux_source_directory(${PROJECT_SOURCE_DIR}/src DIR_SRCS)
# Add header files
include_directories(${PROJECT_SOURCE_DIR}/include)
# Add link library
# link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(${PROJECT_NAME} ${DIR_SRCS})
