include(${CMAKE_CURRENT_LIST_DIR}/context/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/utils/CMakeLists.txt)

add_executable(${MAIN_BINARY})
target_sources(${MAIN_BINARY} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
target_link_libraries(${MAIN_BINARY} PRIVATE ${SRC_LIBRARY})
