add_subdirectory(core)

if(NOT STDLIBX_NO_ALLOC)
    add_subdirectory(alloc)
endif()

if(NOT STDLIBX_NO_STDX)
    add_subdirectory(stdx)
endif()

target_link_libraries(stdlibx INTERFACE stdlibx_core)

if(NOT STDLIBX_NO_ALLOC)
    target_link_libraries(stdlibx INTERFACE stdlibx_alloc)
endif()

if(STDLIBX_EXTENSIONS)
    target_link_libraries(stdlibx INTERFACE stdlibx_stdx)
endif()
