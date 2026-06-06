# Find all test source files
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Create an executable for each test file
foreach(TEST_SOURCE ${TEST_SOURCES})
    # Get the filename without extension
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    
    # Create executable
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    
    target_link_libraries(${TEST_NAME} PRIVATE stdlibx::stdx)
    
    # Force test to wait for stdx and its dependencies to be completely built
    add_dependencies(${TEST_NAME} stdlibx::stdx)
    
    # Set C++ standard and properties
    set_target_properties(${TEST_NAME} PROPERTIES
        CXX_STANDARD ${STDLIBX_CXX_STANDARD}
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

    # Propagate the optional-extension flags so tests can #ifdef on them and
    # become no-ops cleanly when the extension is not built into the library.
    # The src/stdx/CMakeLists.txt already sets these PRIVATE on the library
    # target itself; they're not propagated to consumers, so we re-apply here.
    foreach(EXT
            STDLIBX_EXTENSIONS_COMPILE_ARCH_LIBRARY
            STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY
            STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
            STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY
            STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY
            STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY
            STDLIBX_EXTENSIONS_COMPILE_MICROSOFT_GUIDELINES_SUPPORT_LIBRARY)
        if(${${EXT}})
            target_compile_definitions(${TEST_NAME} PRIVATE ${EXT})
        endif()
    endforeach()

    # Add to CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    
    message(STATUS "Added test executable: ${TEST_NAME}")
endforeach()

# Optional: Add a custom target to build all tests
add_custom_target(build_all_tests 
    DEPENDS ${TEST_SOURCES}
    COMMENT "Building all test executables"
)
