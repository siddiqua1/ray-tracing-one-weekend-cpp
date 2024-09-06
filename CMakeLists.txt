cmake_minimum_required(VERSION 3.15..3.30 FATAL_ERROR)

project(ray_tracing_one_weekend
  VERSION 1.0
  DESCRIPTION "Ray Tracing in One Weekend"
  LANGUAGES CXX
)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" CACHE PATH "Output directory for applications")

# Only build docs if this is the main project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) 
  find_package(Doxygen)
  if(Doxygen_FOUND)
    add_subdirectory(docs)
  else()
    message(STATUS "Doxygen not found, not building docs")
  endif()
endif()

# Enable testing if this is the main project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) 
  include(CTest)
endif()



add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_20)


# compile library code
add_subdirectory(src)

# create executable code
add_subdirectory(apps)

# Only build tests if this is the main project
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTING) 
  add_subdirectory(tests)
endif()
