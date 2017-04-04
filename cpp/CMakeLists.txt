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
find_package(OpenMP)
if(OPENMP_FOUND )
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	
	add_definitions("-fopenmp")
	add_definitions("-lomp")
	add_definitions(-D_COMPILE_WITH_OPENMP_)
endif()

find_library(PCRE_LIBRARY pcre)
target_link_libraries(coxiterweb ${PCRE_LIBRARY})

# ------------------------------------------------------------------
# CoxIter library
add_library(coxiter_graphs STATIC coxiter/graphs.product.set.cpp coxiter/graphs.product.cpp coxiter/graphs.list.n.cpp coxiter/graphs.list.iterator.cpp coxiter/graphs.list.cpp coxiter/graph.cpp)
add_library(coxiter STATIC coxiter/coxiter.cpp)
add_library(coxitermaths STATIC coxiter/lib/math_tools.cpp coxiter/lib/polynomials.cpp coxiter/lib/numbers/number_template.cpp coxiter/lib/numbers/mpz_rational.cpp)
target_link_libraries(coxiter coxitermaths)


target_link_libraries(coxiter coxiter_graphs)

find_library(PARI_LIBRARY pari)
if(PARI_LIBRARY)
	add_library(coxiter_pari STATIC coxiter/lib/paripolynomials.cpp coxiter/growthrate.cpp coxiter/signature.cpp)
	target_link_libraries(coxiter_pari ${GMP_LIBRARY})
	target_link_libraries(coxiter_pari ${GMP_LIBRARYXX})
	target_link_libraries(coxiter coxiter_pari)
	
	target_link_libraries( coxiter ${PARI_LIBRARY})
	add_definitions(-D_COMPILE_WITH_PARI_)
else()
	message(WARNING "Warning: PARI library was not found. CoxIter won't be able to compute growth rate and signature.")
endif()

find_library(GMP_LIBRARY gmp)
target_link_libraries(coxiter ${GMP_LIBRARY})

find_library(GMP_LIBRARYXX gmpxx)
target_link_libraries(coxiter ${GMP_LIBRARYXX})

target_link_libraries(coxiterweb coxiter)

# ------------------------------------------------------------------
# Final
install(TARGETS coxiterweb RUNTIME DESTINATION bin)
