include("${PROJECT_SOURCE_DIR}/cmake/Dependencies.cmake")

add_subdirectory(stdx)

target_link_libraries(stdlibx INTERFACE stdlibx_stdx)
