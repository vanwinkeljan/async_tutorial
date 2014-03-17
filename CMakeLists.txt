project (lttng_async_tutorial)

cmake_minimum_required (VERSION 2.8)

# Add project dir to module search path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}")

#Dependencies
find_package(Threads REQUIRED) 

# include dir
include_directories(${PROJECT_SOURCE_DIR}/include ${LIBBENCHMARK_INCLUDE_DIRS})

# Source files
set ( base_SOURCES src/PerlinNoise src/ppm )

# Libs 
set ( base_LIBS ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

#lttng_test.cpp
add_executable (lttng_test ${base_SOURCES} src/lttng_test)
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

