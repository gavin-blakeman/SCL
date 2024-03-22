# SCL Library CMAKE file

CMAKE_MINIMUM_REQUIRED(VERSION 3.14 FATAL_ERROR)

PROJECT(SCL)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

set(SOURCES    
  source/common.cpp
  source/config.cpp
  )
set(HEADERS
  include/bimap.hpp
  include/config.h
  include/hierarchyBuilder.hpp
  include/mapAsVector.hpp
  include/multiVectorSorted.hpp
  include/vectorSorted.hpp
  )

set(OTHER
  SCL
  )
  
set(DIRECTORIES  
  ${CMAKE_SOURCE_DIR}/GCL
  ${CMAKE_SOURCE_DIR}/MCL
  ${CMAKE_SOURCE_DIR}/fmt/include
  ${CMAKE_SOURCE_DIR}/xlnt
  ${Boost_INCLUDE_DIRS}
  )


add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS} ${OTHER})

target_include_directories(${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${DIRECTORIES} )

ADD_SUBDIRECTORY(test)
