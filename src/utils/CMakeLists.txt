target_sources(${SRC_LIBRARY}
               PRIVATE ${CMAKE_CURRENT_LIST_DIR}/customExceptions.cpp)

target_include_directories(${SRC_LIBRARY}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/utils>
    $<INSTALL_INTERFACE:include/utils>
)
