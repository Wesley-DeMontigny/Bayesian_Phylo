# Bayesian_Phylo
Implementing Bayesian inference of phylogeny from scratch.

# Instructions
To get this to work, you will need to add the Nexus Class Library to the lib directory. Modify the CMakeLists.txt in the /lib/ncl directory and /lib/ncl/ncl directory to look like the following:

/lib/ncl/CMakeLists.txt
```
cmake_minimum_required (VERSION 3.1)

project(ncl)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  add_definitions(/D_CRT_SECURE_NO_WARNINGS)
else()
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
endif()
add_subdirectory (ncl)
```

/lib/ncl/ncl/CMakeLists.txt
```
file(GLOB ncl_INC "*.h")
file(GLOB ncl_SRC "*.cpp")

# Adjust to include the current source directory and its parent
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(ncl_shared SHARED ${ncl_SRC})
add_library(ncl_static STATIC ${ncl_SRC})

target_include_directories(ncl_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(ncl_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

set_target_properties(ncl_shared PROPERTIES OUTPUT_NAME ncl)
set_target_properties(ncl_static PROPERTIES OUTPUT_NAME ncl)

install(TARGETS ncl_shared DESTINATION "lib/ncl")
install(TARGETS ncl_static DESTINATION "lib/ncl")
install(FILES ${ncl_INC} DESTINATION "include/ncl")
```