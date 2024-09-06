file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${ray_tracing_one_weekend_SOURCE_DIR}/include/weekend/*.hpp")

set(SOURCE_LIST 
  smoke.cc
)

add_library(
  weekend
STATIC 
  ${SOURCE_LIST} 
  ${HEADER_LIST}
)

# make headers visible to users
target_include_directories(weekend PUBLIC ../include)

# All users of this library will need at least C++20 (file io)
target_compile_features(weekend PUBLIC cxx_std_20)


# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
