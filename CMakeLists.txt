cmake_minimum_required(VERSION 3.10)
project(Bayesian_Phylo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)
file(GLOB SOURCES "src/*.cpp")
add_executable(main ${SOURCES})

# Link libraries
# target_link_libraries(main.exe libname)