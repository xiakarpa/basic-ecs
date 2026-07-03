cmake_minimum_required(VERSION 3.28)
project(basic-ecs LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


set(SOURCES 
	src/lib/ecs.cpp
	src/systems/movement.cpp
	src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)