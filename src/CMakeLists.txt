include(${CMAKE_CURRENT_LIST_DIR}/context/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/utils/CMakeLists.txt)

if(BUILD_PY_MODULE)
    find_package(PythonInterp REQUIRED)

    find_package(pybind11 QUIET)
    if(NOT pybind11_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            pybind
            GIT_REPOSITORY https://github.com/pybind/pybind11.git)

        FetchContent_MakeAvailable(pybind)
    endif(NOT pybind11_FOUND)

    find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

    pybind11_add_module(context ${CMAKE_CURRENT_LIST_DIR}/module.cpp)
    target_link_libraries(context PRIVATE ${SRC_LIBRARY})
else(BUILD_PY_MODULE)
    add_executable(${MAIN_BINARY})
    target_sources(${MAIN_BINARY} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
    target_link_libraries(${MAIN_BINARY} PRIVATE ${SRC_LIBRARY})
endif(BUILD_PY_MODULE)

include(GNUInstallDirs)

set_target_properties(context
PROPERTIES
    POSITION_INDEPENDENT_CODE 1
)

set_target_properties(context PROPERTIES
                                SKIP_BUILD_RPATH OFF
                                BUILD_WITH_INSTALL_RPATH ON
                                INSTALL_RPATH $ORIGIN/${CMAKE_INSTALL_LIBDIR}                                
                                INSTALL_RPATH_USE_LINK_PATH OFF)

# set_target_properties(fmt PROPERTIES INSTALL_RPATH ${CMAKE_INSTALL_LIBDIR})
