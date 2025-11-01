cmake_minimum_required(VERSION 3.16)
project(PowerPoint-CLI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/includes)
include_directories(${PROJECT_SOURCE_DIR}/src)


file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/**/*.cpp
)


add_executable(PowerPoint-CLI ${SOURCES})


if (MSVC)
    target_compile_options(PowerPoint-CLI PRIVATE /W4 /WX)
else()
    target_compile_options(PowerPoint-CLI PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
