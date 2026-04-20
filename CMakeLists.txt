cmake_minimum_required(VERSION 3.16)
project(Alberich)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/AggressiveOptimizations.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/provideLibrarys.cmake)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES "src/*.cpp")

# Filter die thirdparty files rausfiltern
# Funktioniert mit überprüfung ob terme wie 'thirdparty' im RELATIVEN Pfad zum current source dir sind
set(FILTERED_SOURCES "")

foreach(source ${SOURCES})

    file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}" "${source}")
    
    #
    if(NOT rel_path MATCHES "^src/.*(thirdParty|thirdparty|external|pfr|test|example|compile-fail|build|CMakeFiles)/")
        list(APPEND FILTERED_SOURCES "${source}")
    endif()

endforeach()
set(SOURCES ${FILTERED_SOURCES})

message(STATUS "Alberich SOURCES (${SOURCES}): ${SOURCES}")

set(INCLUDES "")
set(CPPOBJS "")
set(LINKLIBS "")

provideLib("${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/magic_enum" "https://github.com/Neargye/magic_enum.git" "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/magic_enum/include" "" "" "" "" "" "" "")
provideLib("${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lsp-framework" "https://github.com/leon-bckl/lsp-framework.git" "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lsp-framework" "" "lsp" "" "" "" "" "")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/Alberich/ByteSequence")

add_library(alberich STATIC ${SOURCES})
target_link_libraries(alberich PUBLIC ByteSequence)

apply_aggressive_optimizations(alberich)

target_include_directories(alberich PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/Alberich ${INCLUDES})
target_link_libraries(alberich PUBLIC ${LINKLIBS})