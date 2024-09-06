set(LIBRARY_NAME weekend)

file(GLOB_RECURSE LIBRARY_INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE LIBRARY_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

add_library(${LIBRARY_NAME} STATIC
    ${LIBRARY_INCLUDE_FILES}
    ${LIBRARY_SRC_FILES}
)

target_include_directories(
    ${LIBRARY_NAME}
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${LIBRARY_NAME} PUBLIC compiler_flags)


add_subdirectory(tests)