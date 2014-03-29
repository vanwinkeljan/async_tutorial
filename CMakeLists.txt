project (lttng_async_tutorial)

cmake_minimum_required (VERSION 2.8)

# workaround for bug in saucy (Ubuntu 13.10) gcc
# https://bugs.launchpad.net/ubuntu/+source/gcc-defaults/+bug/1228201
set ( CMAKE_EXE_LINKER_FLAGS -Wl,--no-as-needed )

# just make sure that we do not strip to much from shared tracepoint libs
set ( CMAKE_SHARED_LINKER_FLAGS -Wl,--no-as-needed )

# Add project dir to module search path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}")

#Dependencies
find_package(Threads REQUIRED) 

find_package(PkgConfig)
include(UseLttngGenTp)
pkg_check_modules(LTTNG REQUIRED lttng-ust)
include_directories(${LTTNG_INCLUDE_DIRS})
link_directories(${LTTNG_LIBRARY_DIRS})

# include dir
include_directories(${PROJECT_SOURCE_DIR}/include) 

# Source files
set ( base_SOURCES src/PerlinNoise src/ppm )

# Libs 
set ( base_LIBS ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

# Generate trace points

set ( LLTNG_HEADERS src/lttng_test tp_lttng_test.h tp_ppm.h )
add_lttng_gen_tp(NAME lttng_test)
add_lttng_gen_tp(NAME ppm)

#lltng tracepoint libs
add_library (tp_lttng_test SHARED tp_lttng_test.c)
target_link_libraries(tp_lttng_test ${LTTNG_LIBRARIES})

add_library (tp_ppm SHARED tp_ppm.c)
target_link_libraries(tp_ppm ${LTTNG_LIBRARIES})

#Exec lttng_test
add_executable (lttng_test ${base_SOURCES} ${LLTNG_HEADERS} )
target_link_libraries(lttng_test ${base_LIBS} )

# Check for C++11 support
include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11 -std=gnu++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x -std=gnu++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -std=gnu++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -std=gnu++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

