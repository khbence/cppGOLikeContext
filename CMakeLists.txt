cmake_minimum_required(VERSION 3.15)

# Set the project name to your project name, my project isn't very descriptive
project(context CXX)
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_LIBRARY "contextLib")
set(MAIN_BINARY "contextMain")
set(TEST_BINARY "testContext")
set(PWARNINGS, "project_warnings_${PROJECT_NAME}")
set(POPTIONS, "project_options_${PROJECT_NAME}")

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options_${PROJECT_NAME} INTERFACE)
# target_compile_features(project_options INTERFACE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
    if(ENABLE_BUILD_WITH_TIME_TRACE)
        add_compile_definitions(project_options INTERFACE -ftime-trace)
    endif()
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(PWARNINGS INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(PWARNINGS)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(POPTIONS)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" OFF)
option(ENABLE_FUZZING "Enable Fuzzing Builds" OFF)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if(ENABLE_PCH)
    # This sets a global PCH parameter, each project will build its own PCH, which is a good idea if any #define's change
    #
    # consider breaking this out per project as necessary
    target_precompile_headers(POPTIONS INTERFACE <iostream>)
endif()

if(ENABLE_FUZZING)
    message("Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html")
    add_subdirectory(fuzz_test)
endif()

add_library(${SRC_LIBRARY})
add_library(${SRC_LIBRARY}::${SRC_LIBRARY} ALIAS ${SRC_LIBRARY})

find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

target_link_libraries(${SRC_LIBRARY} PRIVATE ${POPTIONS} ${PWARNINGS} Threads::Threads)

option(ENABLE_UNITY "Enable Unity builds of projects" OFF)
if(ENABLE_UNITY)
  # Add for any project you want to apply unity builds for
  set_target_properties(${SRC_LIBRARY} PROPERTIES UNITY_BUILD ON)
endif()

option(ENABLE_LTO "Enable Link Time Optimization" OFF)
if(ENABLE_LTO)
  set_target_properties(${SRC_LIBRARY} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

option(BUILD_PY_MODULE "Build Python module" OFF)
include(src/CMakeLists.txt)

if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
