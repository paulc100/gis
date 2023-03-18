cmake_minimum_required(VERSION 3.20)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# Change 'algoalgo' if you want the project to have a different name
project(algoalgo
	VERSION 1.0.0
	LANGUAGES CXX)

# Change 'include' to wherever your .h files are
include_directories(GLOB_RECURSE 
	${CMAKE_SOURCE_DIR}/include/)

# Change 'src' to wherever your .cpp files are
file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.cpp)

set(EXECUTABLE_NAME GIS)
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
