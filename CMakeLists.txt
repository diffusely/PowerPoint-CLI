cmake_minimum_required(VERSION 3.16)
project(PowerPoint-CLI VERSION 1.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(${PROJECT_SOURCE_DIR}/includes)


set(SOURCES
    src/main.cpp
    src/cli.cpp
    src/Lexer/lexer.cpp
    src/Lexer/token.cpp
    src/Parser/parse.cpp
)


add_executable(PowerPoint-CLI ${SOURCES})


if (MSVC)
    target_compile_options(PowerPoint-CLI PRIVATE /W4)
else()
    target_compile_options(PowerPoint-CLI PRIVATE -Wall -Wextra -Wpedantic)
endif()


message(STATUS "PowerPoint CLI - Building with ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Source files: ${SOURCES}")
