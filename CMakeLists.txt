cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(ray_tracing_one_weekend)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" CACHE PATH "Output directory for applications")

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_17)

# library code
add_subdirectory(weekend)

# applications to be built
add_subdirectory(apps)