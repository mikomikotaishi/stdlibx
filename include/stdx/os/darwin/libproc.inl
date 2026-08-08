#pragma once

/**
 * @namespace stdx::os::darwin
 * @brief Process introspection - <libproc.h>.
 */
export namespace stdx::os::darwin {
    #if defined(__APPLE__) && __has_include(<libproc.h>)
    using ::proc_listpids;
    using ::proc_listallpids;
    using ::proc_listpgrppids;
    using ::proc_listchildpids;
    using ::proc_pidinfo;
    using ::proc_pidfdinfo;
    using ::proc_pidfileportinfo;
    using ::proc_pidpath;
    using ::proc_name;
    using ::proc_regionfilename;
    using ::proc_kmsgbuf;
    using ::proc_libversion;
    using ::proc_pid_rusage;
    using ::proc_setpcontrol;
    #endif
}
