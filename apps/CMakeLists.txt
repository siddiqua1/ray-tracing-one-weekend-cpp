

set(EXECUTABLE_NAME output_ppm)
add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_NAME}.cc)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC
    weekend
)

set(EXECUTABLE_NAME camera)
add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_NAME}.cc)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC
    weekend
)

set(EXECUTABLE_NAME single_sphere)
add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_NAME}.cc)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC
    weekend
)
