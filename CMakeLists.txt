cmake_minimum_required(VERSION 3.10)

project(MagneticFluidFormAlgorithm VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

# using shared library on Windows needs dancing with a tambourine
if (WIN32)
    set(LIBRARY_TYPE STATIC)
else (WIN32)
    set(LIBRARY_TYPE SHARED)
endif (WIN32)

set(LIBS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs)

set(RAPIDJSON_ROOT ${LIBS_ROOT}/rapidjson)

include_directories(${RAPIDJSON_ROOT}/include)

file(GLOB_RECURSE proj_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(MagneticFluidFormAlgorithm ${LIBRARY_TYPE} ${proj_sources})

install(TARGETS MagneticFluidFormAlgorithm
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
