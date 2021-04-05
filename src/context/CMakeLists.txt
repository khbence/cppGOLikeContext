target_sources(${SRC_LIBRARY} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/context.cpp
        ${CMAKE_CURRENT_LIST_DIR}/todo.cpp
        ${CMAKE_CURRENT_LIST_DIR}/background.cpp
        ${CMAKE_CURRENT_LIST_DIR}/contextFactory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/withCancel.cpp
        ${CMAKE_CURRENT_LIST_DIR}/withDeadline.cpp)

target_include_directories(${SRC_LIBRARY} PUBLIC include/context)
