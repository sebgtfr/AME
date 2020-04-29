cmake_minimum_required(VERSION 3.8)
project(AME)

set(CMAKE_CXX_STANDARD 17)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()
if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/test)

find_package (Threads)

set(SRCS_AME
    srcs/Core.cpp
    srcs/IntervalService.cpp
    srcs/ThreadManager.cpp
    srcs/Worker.cpp
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)

add_library(AME SHARED ${SRCS_AME})
target_link_libraries(AME PUBLIC ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(AME PUBLIC include)
target_compile_definitions(AME PUBLIC AME_EXPORT)
