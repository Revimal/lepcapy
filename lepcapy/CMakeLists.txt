INCLUDE_DIRECTORIES (include)
FILE(GLOB SRC_FILES src/*.c)
FILE(GLOB HEADER_FILES include/*.h)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE})
ADD_EXECUTABLE(lepcapy ${SRC_FILES} ${HEADER_FILES})
TARGET_LINK_LIBRARIES(lepcapy ${CMAKE_THREAD_LIBS_INIT})
