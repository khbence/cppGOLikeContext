target_sources(${SRC_LIBRARY} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/customExceptions.cpp)

target_include_directories(${SRC_LIBRARY} PUBLIC include/utils)
