cmake_minimum_required(VERSION 3.16)
project(Alberich)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/AggressiveOptimizations.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/provideLibrarys.cmake)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "/(thirdparty|thirdParty|external|build|CMakeFiles)/")

set(INCLUDES "")
set(CPPOBJS "")
set(LINKLIBS "")

provideLib("${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/magic_enum" "https://github.com/Neargye/magic_enum.git" "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/magic_enum/include" "" "" "" "" "" "" "")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/Alberich/ByteSequence")

add_library(alberich ${SOURCES})
target_link_libraries(alberich PRIVATE ByteSequence)

apply_aggressive_optimizations(alberich)

target_include_directories(alberich PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/Alberich ${INCLUDES})