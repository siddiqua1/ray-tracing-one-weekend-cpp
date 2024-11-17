function(create_weekend_executable app_name)
    add_executable(${app_name} ${app_name}.cc)
    target_link_libraries(${app_name} PRIVATE weekend)
endfunction()

create_weekend_executable(output_ppm)
create_weekend_executable(camera)
create_weekend_executable(single_sphere)
create_weekend_executable(rendered_normals)
create_weekend_executable(hittable_scene)
create_weekend_executable(metal_scene)
create_weekend_executable(dielectric_scene)
create_weekend_executable(wide_angle_fov)
create_weekend_executable(dielectric_alt_view)
create_weekend_executable(depth_of_field)
create_weekend_executable(final_scene)

add_executable(wgpu_setup wgpu_setup.cc)
target_link_libraries(wgpu_setup PRIVATE webgpu)

target_copy_webgpu_binaries(wgpu_setup)

if (EMSCRIPTEN)
	# Generate a full web page rather than a simple WebAssembly module
	set_target_properties(wgpu_setup PROPERTIES SUFFIX ".html")

    # Enable the use of emscripten_sleep()
    target_link_options(wgpu_setup PRIVATE -sASYNCIFY)
endif()


