cmake_minimum_required(VERSION 3.1)

project(coxiterweb)

add_definitions("-std=c++11")
if( CYGWIN )
	add_definitions("-std=gnu++11")
endif()

add_definitions("-Wall")
add_definitions("-Wno-reorder")
add_definitions("-Wno-unknown-pragmas")
add_definitions("-msse3")

set(CMAKE_BUILD_TYPE Release)

add_executable(coxiterweb main)


install(TARGETS coxiterweb RUNTIME DESTINATION bin)
