cmake_minimum_required(VERSION 3.16)

project(MathWorker VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MATHWORKER_BUILD_SHARED "Build MathWorker as a shared library (otherwise static)" OFF)

set(MATHWORKER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE MATHWORKER_SRC CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

if(MATHWORKER_SRC)
    add_library(MathWorker
        $<IF:$<BOOL:${MATHWORKER_BUILD_SHARED}>,SHARED,STATIC>
        ${MATHWORKER_SRC}
    )
    target_include_directories(MathWorker PUBLIC "${MATHWORKER_INCLUDE_DIR}")
else()
    add_library(MathWorker INTERFACE)
    target_include_directories(MathWorker INTERFACE "${MATHWORKER_INCLUDE_DIR}")
endif()

add_library(MathWorker::MathWorker ALIAS MathWorker)