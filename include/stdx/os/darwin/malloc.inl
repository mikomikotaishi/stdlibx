#pragma once

/**
 * @namespace stdx::os::darwin
 * @brief The Darwin malloc zone interface - <malloc/malloc.h>.
 */
export namespace stdx::os::darwin {
    #if defined(__APPLE__) && __has_include(<malloc/malloc.h>)
    using MallocZone = ::malloc_zone_t;
    using MallocStatistics = ::malloc_statistics_t;

    using ::malloc_default_zone;
    using ::malloc_create_zone;
    using ::malloc_destroy_zone;
    using ::malloc_zone_from_ptr;
    using ::malloc_zone_malloc;
    using ::malloc_zone_calloc;
    using ::malloc_zone_valloc;
    using ::malloc_zone_realloc;
    using ::malloc_zone_free;
    using ::malloc_zone_memalign;
    using ::malloc_size;
    using ::malloc_good_size;
    using ::malloc_zone_statistics;
    using ::malloc_zone_check;
    using ::malloc_zone_log;
    using ::malloc_zone_print;
    using ::malloc_printf;
    #endif
}
