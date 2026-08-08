#pragma once

/**
 * @namespace stdx::os::darwin::macho
 * @brief The dynamic loader's view of the loaded images - <mach-o/dyld.h>.
 */
export namespace stdx::os::darwin::macho {
    #if defined(__APPLE__) && __has_include(<mach-o/dyld.h>)
    using ::_dyld_image_count;
    using ::_dyld_get_image_header;
    using ::_dyld_get_image_vmaddr_slide;
    using ::_dyld_get_image_name;
    using ::_dyld_register_func_for_add_image;
    using ::_dyld_register_func_for_remove_image;
    using ::_NSGetExecutablePath;
    using ::NSVersionOfRunTimeLibrary;
    using ::NSVersionOfLinkTimeLibrary;
    #endif
}
