cmake_minimum_required(VERSION 3.7)
project(LuaLib C)

set(LUA_RUNTIME_SOURCES
    "${PROJECT_SOURCE_DIR}/src/lapi.c"
    "${PROJECT_SOURCE_DIR}/src/lauxlib.c"
    "${PROJECT_SOURCE_DIR}/src/lbaselib.c"
    "${PROJECT_SOURCE_DIR}/src/lcode.c"
    "${PROJECT_SOURCE_DIR}/src/lcorolib.c"
    "${PROJECT_SOURCE_DIR}/src/lctype.c"
    "${PROJECT_SOURCE_DIR}/src/ldblib.c"
    "${PROJECT_SOURCE_DIR}/src/ldebug.c"
    "${PROJECT_SOURCE_DIR}/src/ldo.c"
    "${PROJECT_SOURCE_DIR}/src/ldump.c"
    "${PROJECT_SOURCE_DIR}/src/lfunc.c"
    "${PROJECT_SOURCE_DIR}/src/lgc.c"
    "${PROJECT_SOURCE_DIR}/src/linit.c"
    "${PROJECT_SOURCE_DIR}/src/liolib.c"
    "${PROJECT_SOURCE_DIR}/src/llex.c"
    "${PROJECT_SOURCE_DIR}/src/lmathlib.c"
    "${PROJECT_SOURCE_DIR}/src/lmem.c"
    "${PROJECT_SOURCE_DIR}/src/loadlib.c"
    "${PROJECT_SOURCE_DIR}/src/lobject.c"
    "${PROJECT_SOURCE_DIR}/src/lopcodes.c"
    "${PROJECT_SOURCE_DIR}/src/loslib.c"
    "${PROJECT_SOURCE_DIR}/src/lparser.c"
    "${PROJECT_SOURCE_DIR}/src/lstate.c"
    "${PROJECT_SOURCE_DIR}/src/lstring.c"
    "${PROJECT_SOURCE_DIR}/src/lstrlib.c"
    "${PROJECT_SOURCE_DIR}/src/ltable.c"
    "${PROJECT_SOURCE_DIR}/src/ltablib.c"
    "${PROJECT_SOURCE_DIR}/src/ltm.c"
    "${PROJECT_SOURCE_DIR}/src/lundump.c"
    "${PROJECT_SOURCE_DIR}/src/lutf8lib.c"
    "${PROJECT_SOURCE_DIR}/src/lvm.c"
    "${PROJECT_SOURCE_DIR}/src/lzio.c"
    # "${PROJECT_SOURCE_DIR}/src/lua.c"  # Excluded to avoid multiple definition of main
    # "${PROJECT_SOURCE_DIR}/src/luac.c"
)

# Collect header files for installation
file(GLOB LuaLib_INC "${PROJECT_SOURCE_DIR}/src/*.h")

# Include the source directory
include_directories("${PROJECT_SOURCE_DIR}/src")

# Add the shared library target
add_library(LuaLib_static STATIC ${LUA_RUNTIME_SOURCES})

# Specify include directories for the target
target_include_directories(LuaLib_static PUBLIC "${PROJECT_SOURCE_DIR}/src")

# Set properties for the shared library
set_target_properties(LuaLib_static PROPERTIES OUTPUT_NAME LuaLib)


# Install the library and headers
install(TARGETS LuaLib_static DESTINATION "lib/lua-5.4.7")
install(FILES ${LuaLib_INC} DESTINATION "include/LuaLib")
