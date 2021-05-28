target_sources(${TEST_BINARY} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ttodo.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tbackground.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tcontextFactory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/twithCancel.cpp
        ${CMAKE_CURRENT_LIST_DIR}/twithDeadline.cpp
        ${CMAKE_CURRENT_LIST_DIR}/twithValue.cpp
        ${CMAKE_CURRENT_LIST_DIR}/combinedTest.cpp)
