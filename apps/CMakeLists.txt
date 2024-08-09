

set(EXECUTABLE_NAME output_ppm)
add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_NAME}.cc)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC
    weekend
)
