include(FetchContent)

find_package(GTest QUIET)

if(NOT GTEST_FOUND)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0)

    FetchContent_MakeAvailable(googletest)
endif()

add_executable(${TEST_BINARY})
target_sources(${TEST_BINARY} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

include(${CMAKE_CURRENT_LIST_DIR}/context/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/utils/CMakeLists.txt)

target_link_libraries(${TEST_BINARY} gtest gtest_main ${SRC_LIBRARY})

enable_testing()

add_test(NAME google_test COMMAND $<TARGET_FILE:${TEST_BINARY}>)
