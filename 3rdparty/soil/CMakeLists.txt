project(soil)

cmake_minimum_required(VERSION 3.0)

if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)

include_directories(./)

aux_source_directory(. SRCFILE)

file(GLOB_RECURSE HEADFILE ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(${PROJECT_NAME} 
			${SRCFILE}
			${HEADFILE})
			
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
