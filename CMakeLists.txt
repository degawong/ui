
PROJECT(harpocrates)

set(CMAKE_CXX_STANDARD 20)

cmake_minimum_required(VERSION 3.0)

add_definitions("-g")

add_definitions(
	-DSTB_IMAGE_IMPLEMENTATION
)

add_subdirectory(
	3rdparty
)

set(
	opengl_library
	glm
	glad
	glfw
	freetype
)

file(
	GLOB_RECURSE
	opengl_file
	${CMAKE_CURRENT_SOURCE_DIR}/opengl/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/opengl/*.hpp
)

file(
	GLOB_RECURSE
	shader_file
	${CMAKE_CURRENT_SOURCE_DIR}/shader/*.glsl
)

file(
	GLOB_RECURSE
	algorithm_file
	${CMAKE_CURRENT_SOURCE_DIR}/algorithm/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/algorithm/*.hpp
)

file(
	GLOB_RECURSE
	dll_library
	${CMAKE_CURRENT_SOURCE_DIR}/ui/dll/*.dll
)

message(STATUS "${algorithm_file}")

add_executable(
	${PROJECT_NAME}
	base.h
	io.cpp
	io.hpp
	${opengl_file}
	${shader_file}
	harpocrates.cpp
	path_walker.hpp
	scope_guard.hpp
	thread_pool.hpp
	${algorithm_file}
	singleton_pattern.hpp
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
	${PROJECT_NAME}
	${opengl_library}
)
set_property(
	GLOBAL PROPERTY
	USE_FOLDERS on
)

set_property(
	TARGET
	glm
	glad
	glfw
	freetype
	PROPERTY FOLDER "OpenGL"
)

set_directory_properties(
	PROPERTIES VS_STARTUP_PROJECT
	${PROJECT_NAME}
)

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
	"${CMAKE_CURRENT_SOURCE_DIR}/workspace/"
)

# # copy your dll library to workspace
# add_custom_command(
	# TARGET ${PROJECT_NAME} POST_BUILD
	# COMMAND ${CMAKE_COMMAND} -E copy_if_different 
	# ${dll_library}
	# ${CMAKE_CURRENT_SOURCE_DIR}/workspace/
	# COMMENT "${CMAKE_SYSTEM_NAME} platform : copying algorithm dll library to execute program directory..."
# )
