# SCL Library CMAKE file

CMAKE_MINIMUM_REQUIRED(VERSION 3.14 FATAL_ERROR)

PROJECT(SCL)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

set(SOURCES
  source/array1DP.cpp
  source/array2DP.cpp
  source/common.cpp
  source/config.cpp
  source/valarray2DP.cpp
  )
set(HEADERS
  include/array1DP.h
  include/array2DP.h
  include/config.h
  include/hierarchyBuilder.hpp
  include/valarray2DP.h
  SCL
  )


add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE "../GCL" "../MCL" )
