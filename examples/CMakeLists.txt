# Find all example source files
file(GLOB_RECURSE EXAMPLE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Create an executable for each test file
foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
    # Get the filename without extension
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
    
    # Create executable
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})

    target_link_libraries(${EXAMPLE_NAME} PRIVATE stdlibx)
    
    # Force test to wait for stdx and its dependencies to be completely built
    add_dependencies(${EXAMPLE_NAME} stdlibx::stdx)
    
    # Set C++ standard and properties
    set_target_properties(${EXAMPLE_NAME} PROPERTIES
        CXX_STANDARD ${STDLIBX_CXX_STANDARD}
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )

    # Serialize the heavy stdexec-coroutine consumer TU against the exec tests to
    # dodge the clang-22 concurrent-module-AST-reader crash (see top CMakeLists).
    if(EXAMPLE_NAME STREQUAL "FlowExample")
        set_target_properties(${EXAMPLE_NAME} PROPERTIES JOB_POOL_COMPILE stdexec_serial)
    endif()
    
    # Configure as a C++ module target if the function is available
    if(COMMAND configure_cpp_module_target)
        configure_cpp_module_target(${EXAMPLE_NAME})
    endif()
    
    # Add sanitizers if enabled
    if(COMMAND add_sanitizers)
        add_sanitizers(${EXAMPLE_NAME})
    endif()

    # Add to CTest
    add_test(NAME ${EXAMPLE_NAME} COMMAND ${EXAMPLE_NAME})
    
    message(STATUS "Added example executable: ${EXAMPLE_NAME}")
endforeach()

# Optional: Add a custom target to build all examples
add_custom_target(build_all_examples
    DEPENDS ${EXAMPLE_SOURCES}
    COMMENT "Building all example executables"
)
