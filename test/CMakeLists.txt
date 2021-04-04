include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    # prevent it to override our compiler/linker
    set(gtest_force_shared_crt
        ON
        CACHE BOOL "" FORCE)
    # prevent it from using PThreads
    set(gtest_disable_pthreads
        ON
        CACHE BOOL "" FORCE)

    foreach(dir ${googletest_SOURCE_DIR})
        add_subdirectory(${dir})
    endforeach()

    foreach(dir ${googletest_BINARY_DIR})
        add_subdirectory(${dir})
    endforeach()

    # Silence std::trl warning
    if(MSVC)
        foreach(
            _tgt
            gtest
            gtest_main
            gmock
            gmock_main)
            target_compile_definitions(${_tgt} PRIVATE "_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING")
        endforeach()
    endif()
    add_executable(${TEST_BINARY})
    target_sources(${TEST_BINARY} PRIVATE main.cpp)
    target_link_libraries(${TEST_BINARY} PRIVATE ${SRC_LIBRARY} gtest gtest_main)

    enable_testing()
    add_test(NAME google_test COMMAND $<TARGET_FILE:${TEST_BINARY}>)
endif()
