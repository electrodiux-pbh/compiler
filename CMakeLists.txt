cmake_minimum_required(VERSION 3.16)

# Set the project name and version
project(compiler VERSION 1.0.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Force Clang compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Generate compile_commands.json for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

# Include directories
include_directories(include)

# Collect all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h")

# Create the executable
if(SOURCES)
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
    
    # Set target properties
    set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
    
    # Target include directories
    target_include_directories(${PROJECT_NAME} PRIVATE include)
    
    # Compiler-specific flags for the target
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall -Wextra -Wpedantic -Wno-unused-parameter
        )
    endif()
else()
    message(WARNING "No source files found in src/ directory")
endif()

# Print some information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")