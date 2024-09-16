# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(HelloWorld)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (WIN32)
    set(SOURCES
        src/main.cpp
    )
else()
    set(SOURCES
        src/main.cpp
        src/hello.cpp
    )
endif()

add_executable(HelloWorld ${SOURCES})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(HelloWorld
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/includes/win
        ${PROJECT_SOURCE_DIR}/includes/linux
        ${PROJECT_SOURCE_DIR}/includes/portable
)

# Specify the output directory for the executable
set_target_properties(HelloWorld PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
