project (lttng_async_tutorial)

cmake_minimum_required (VERSION 2.8)

#Dependencies
find_package( Threads )

# full async
add_executable (movie_async PerlinNoise movie_async ppm)
target_link_libraries(movie_async ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} )

# 2 async
add_executable (movie_async_ctrl_threads PerlinNoise ppm movie_async_ctrl_threads)
target_link_libraries(movie_async_ctrl_threads ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} )

# serial 
add_executable (movie_serial PerlinNoise ppm movie_serial)
target_link_libraries(movie_serial ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} )

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

