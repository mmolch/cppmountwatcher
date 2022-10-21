cmake_minimum_required(VERSION 3.0)
project(mmolch_mountwatcher)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

add_library(mmolch_mountwatcher STATIC
    src/MountWatcher.cpp
    src/Pipe.cpp
)
add_library(mmolch::mountwatcher ALIAS mmolch_mountwatcher)

target_include_directories(mmolch_mountwatcher PUBLIC
    include
)

target_link_libraries(mmolch_mountwatcher PUBLIC
    pthread
)

if(CMAKE_CURRENT_LIST_DIR STREQUAL ${CMAKE_SOURCE_DIR})
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()
