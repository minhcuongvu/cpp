# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(HelloWorld)

set(SOURCES
    src/main.cpp
    src/hello.cpp
)

add_executable(HelloWorld ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(HelloWorld
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/includes
)

# Specify the output directory for the executable
set_target_properties(HelloWorld PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
