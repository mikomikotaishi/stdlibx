# Find all test source files
file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Create an executable for each test file
foreach(TEST_SOURCE ${TEST_SOURCES})
    # Get the filename without extension
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    
    # Create executable
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    
    # Link against the stdlibx library
    target_link_libraries(${TEST_NAME} PRIVATE stdlibx::stdlibx)
    
    # Set C++ standard and properties
    set_target_properties(${TEST_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
    
    # Configure as a C++ module target if the function is available
    if(COMMAND configure_cpp_module_target)
        configure_cpp_module_target(${TEST_NAME})
    endif()
    
    # Add sanitizers if enabled
    if(COMMAND add_sanitisers)
        add_sanitisers(${TEST_NAME})
    endif()
    
    # Add to CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    
    message(STATUS "Added test executable: ${TEST_NAME}")
endforeach()

# Optional: Add a custom target to build all tests
add_custom_target(build_all_tests 
    DEPENDS ${TEST_SOURCES}
    COMMENT "Building all test executables"
)
