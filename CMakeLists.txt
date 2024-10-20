# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(HelloWorld)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (WIN32)
    set(HELLO_WORLD_SOURCES
        src/main.cpp
    )
    set(SERVER_SOURCES
        src/server.cpp
    )
    set(CLIENT_SOURCES 
        src/client.cpp
    )
    add_executable(server ${SERVER_SOURCES})
    add_executable(client ${CLIENT_SOURCES})
else()
    set(HELLO_WORLD_SOURCES
        src/main.cpp
        src/hello.cpp
    )
    add_executable(HelloWorld ${HELLO_WORLD_SOURCES})
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/

# Specify the output directory for the executable
if (LINUX)
    target_include_directories(HelloWorld
        PRIVATE 
            ${PROJECT_SOURCE_DIR}/includes/win
            ${PROJECT_SOURCE_DIR}/includes/linux
            ${PROJECT_SOURCE_DIR}/includes/portable
    )
    set_target_properties(HelloWorld PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
endif()
if (WIN32)
    target_include_directories(server
        PRIVATE 
            ${PROJECT_SOURCE_DIR}/includes/win
            ${PROJECT_SOURCE_DIR}/includes/linux
            ${PROJECT_SOURCE_DIR}/includes/portable
    )
    target_include_directories(client
        PRIVATE 
            ${PROJECT_SOURCE_DIR}/includes/win
            ${PROJECT_SOURCE_DIR}/includes/linux
            ${PROJECT_SOURCE_DIR}/includes/portable
    )
    set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
    set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
endif()
