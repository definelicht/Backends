cmake_minimum_required(VERSION 2.8.8)
project(backends)

if (NOT BACKEND)
  set(BACKEND "Vc")
endif()

set(BACKEND_INTERNAL FALSE)

if (BACKEND STREQUAL "Vc")
  set(Vc TRUE)
  set(BACKEND_INTERNAL "Vc")
endif()

if (BACKEND STREQUAL "Eigen")
  set(Eigen TRUE)
  set(BACKEND_INTERNAL "Eigen")
endif()

if (BACKEND STREQUAL "Scalar")
  set(Scalar TRUE)
  set(BACKEND_INTERNAL "Scalar")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(GNU TRUE)
endif()

if (NOT BACKEND_INTERNAL)
  message(FATAL_ERROR "Invalid backend \"${BACKEND}\".")
endif()

message(STATUS "Compiling for backend \"${BACKEND}\".")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -march=native -O2 -Wall -ffast-math")
if (SIMD)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m${SIMD}")
endif()
if (Vc AND GNU)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fabi-version=6")
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if (Vc)
  find_package(Vc REQUIRED)
  set(LIBS ${LIBS} ${Vc_LIBRARIES})
  include_directories(${Vc_INCLIDE_DIR})
endif()

if (Eigen)
  find_package(Eigen3 REQUIRED)
  include_directories(${EIGEN3_INCLUDE_DIR})
endif()

set(SRC_CPP ${CMAKE_SOURCE_DIR}/box.cc)
if (Vc)
  set(SRC_CPP ${SRC_CPP} ${CMAKE_SOURCE_DIR}/vc.cc)
endif()
if (Scalar)
  set(SRC_CPP ${SRC_CPP} ${CMAKE_SOURCE_DIR}/scalar.cc)
endif()
if (Eigen)
  set(SRC_CPP ${SRC_CPP} ${CMAKE_SOURCE_DIR}/eigen.cc)
endif()
set_source_files_properties(
  ${SRC_CPP}
  PROPERTIES COMPILE_FLAGS -std=c++11
)

message(STATUS "Compiling with C++ flags: ${CMAKE_CXX_FLAGS}")

add_library(backends ${SRC_CPP})
target_link_libraries(backends ${LIBS})
set(LIBS ${LIBS} backends)

set(SRC_TEST ${CMAKE_SOURCE_DIR}/benchmark.cc)
set_source_files_properties(${SRC_TEST} PROPERTIES COMPILE_FLAGS -std=c++11)
add_executable(benchmark ${CMAKE_SOURCE_DIR}/benchmark.cc)
target_link_libraries(benchmark ${LIBS})