cmake_minimum_required(VERSION 3.10)
project(Bayesian_Phylo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/ncl)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/lua-5.4.7)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/LuaBridge3-master)

include_directories(${PROJECT_SOURCE_DIR}/lib/ncl/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/lua-5.4.7/src)
include_directories(${PROJECT_SOURCE_DIR}/lib/LuaBridge3-master/Source)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(main ${SOURCES})

target_link_libraries(main 
    PRIVATE 
        ncl_shared 
        LuaLib_static 
        LuaBridge
)

target_include_directories(main 
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src
)

set(CMAKE_VERBOSE_MAKEFILE ON)