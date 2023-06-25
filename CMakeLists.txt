cmake_minimum_required(VERSION 3.18)

add_compile_options(-W2)
set(CMAKE_C_FLAGS_RELEASE "/Oi /Ot /Oy /Ox /GL /Gz /Qpar /WX /TC /std:c17")

project (Invoker)

option(USE_LTO "Use link time optimization" TRUE)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION USE_LTO)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/src")
set(RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.rc")
add_executable (${CMAKE_PROJECT_NAME}
                WIN32
                src/main.c
                ${RESOURCE})

add_definitions(-DSOURCE_DIR=${CMAKE_SOURCE_DIR})

execute_process(
  COMMAND git rev-parse --short HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE)
add_definitions(-DHASH_CMAKE=${GIT_COMMIT_HASH})

target_precompile_headers(${CMAKE_PROJECT_NAME}
  PRIVATE
    src/stdafx.h
)
