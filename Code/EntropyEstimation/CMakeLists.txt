cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11 -fopenmp -msse2 -ffloat-store -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../../Bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../../Bin")

include_directories(SP800-90B_EntropyAssessment/cpp/iid)
include_directories(SP800-90B_EntropyAssessment/cpp/non_iid)
include_directories(SP800-90B_EntropyAssessment/cpp/shared)
include_directories(SP800-90B_EntropyAssessment/cpp/)

link_libraries(crypto)
link_libraries(ssl)

project(EntropyEstimation)

#add_executable(ea_non_iid
#        SP800-90B_EntropyAssessment/cpp/non_iid_main.cpp)
add_library(non_iid_main SHARED SP800-90B_EntropyAssessment/cpp/non_iid_main.cpp SP800-90B_EntropyAssessment/cpp/non_iid_main.h)
target_link_libraries(non_iid_main divsufsort)

add_executable(EntropyEstimation main.cpp)
target_link_libraries(EntropyEstimation non_iid_main)