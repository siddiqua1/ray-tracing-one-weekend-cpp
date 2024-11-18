# This is only meant to be included as a subdirectory in another project.
# It assumes that targets 'glfw' and 'webgpu' exist.
# Look at examples/CMakeLists.txt to see how to use it in a project.

# The glfw3webgpu target
project(glfw3webgpu)
add_library(glfw3webgpu STATIC glfw3webgpu.c)
target_include_directories(glfw3webgpu PUBLIC .)
target_link_libraries(glfw3webgpu PUBLIC glfw webgpu)

if (GLFW_BUILD_WAYLAND)
  target_compile_definitions(
    glfw3webgpu 
  PRIVATE 
    # WGPU_TARGET_LINUX_WAYLAND = 4
    _GLFW_WAYLAND=1
  )
endif()

if (APPLE)
  target_compile_options(glfw3webgpu PRIVATE -x objective-c)
  target_link_libraries(glfw3webgpu PRIVATE "-framework Cocoa" "-framework CoreVideo" "-framework IOKit" "-framework QuartzCore")
endif ()
