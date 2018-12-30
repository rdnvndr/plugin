include("${CMAKE_SOURCE_DIR}/cmake/main.cmake")

# Места нахождения бинарных файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BINARY_DIR}/${TESTPATH}/plugins")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BINARY_DIR}/${TESTPATH}/plugins")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BINARY_DIR}/${TESTPATH}/plugins")

# Сборка проекта
include("${CMAKE_SOURCE_DIR}/cmake/rpath.cmake")
add_library(${PROJECT_NAME} MODULE ${HEADERS} ${SOURCES} ${FORMS} ${RESOURCES})
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
