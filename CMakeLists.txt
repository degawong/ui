
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

add_subdirectory(
	algorithm
)

set(
	opengl_library
	glm
	glad
	glfw
	soil
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
	module_file
	${CMAKE_CURRENT_SOURCE_DIR}/module/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/module/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/module/*.hpp
)

source_group(
	"glsl"
	FILES
	${shader_file}
)

file(
	GLOB_RECURSE
	dll_library
	${CMAKE_CURRENT_SOURCE_DIR}/ui/dll/*.dll
)

add_executable(
	${PROJECT_NAME}
	io.cpp
	io.hpp
	${opengl_file}
	${shader_file}
	${module_file}
	harpocrates.cpp
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/algorithm
)

target_link_libraries(
	${PROJECT_NAME}
	base_algorithm
	${opengl_library}
)

set_property(
	GLOBAL PROPERTY
	USE_FOLDERS on
)

set_property(
	TARGET
	base_algorithm
	PROPERTY FOLDER "algorithm"
)

set_property(
	TARGET
	${opengl_library}
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

# set_Target_Properties(
	# ${PROJECT_NAME}
	# PROPERTIES
	# LINK_FLAGS_DEBUG
	# "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
	# LINK_FLAGS_RELEASE
	# "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
# )

# # copy your dll library to workspace
# add_custom_command(
	# TARGET ${PROJECT_NAME} POST_BUILD
	# COMMAND ${CMAKE_COMMAND} -E copy_if_different 
	# ${dll_library}
	# ${CMAKE_CURRENT_SOURCE_DIR}/workspace/
	# COMMENT "${CMAKE_SYSTEM_NAME} platform : copying algorithm dll library to execute program directory..."
# )
