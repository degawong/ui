
project(
	base_algorithm
)

cmake_minimum_required(VERSION 3.0)

set(
	CMAKE_CXX_STANDARD
	17
)


file(
	GLOB_RECURSE
	src_file
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

add_library(
	${PROJECT_NAME}
	${src_file}
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)