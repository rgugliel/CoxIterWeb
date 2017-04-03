cmake_minimum_required(VERSION 3.1)

project(coxiterweb)

add_definitions("-std=c++11")
if (CYGWIN)
	add_definitions("-std=gnu++11")
endif()

add_definitions("-Wall")
add_definitions("-Wno-reorder")
add_definitions("-Wno-unknown-pragmas")
add_definitions("-msse3")

set(CMAKE_BUILD_TYPE Release)

add_executable(coxiterweb main.cpp app.cpp coxiter/lib/string.cpp coxiter/lib/regexp.cpp)

# ------------------------------------------------------------------
# External libraries
find_package( OpenMP )
if( OPENMP_FOUND )
	set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" )
	
	add_definitions("-fopenmp")
	add_definitions("-lomp")
	add_definitions(-D_COMPILE_WITH_OPENMP_)
endif()

find_library(PCRE_LIBRARY pcre)
target_link_libraries(coxiterweb ${PCRE_LIBRARY})

# ------------------------------------------------------------------
# Final
install(TARGETS coxiterweb RUNTIME DESTINATION bin)
