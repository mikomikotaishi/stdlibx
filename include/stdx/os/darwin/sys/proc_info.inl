#pragma once

/**
 * @namespace stdx::os::darwin::sys
 * @brief The structures proc_pidinfo(3) fills in - <sys/proc_info.h>.
 */
export namespace stdx::os::darwin::sys {
    #if defined(__APPLE__) && __has_include(<sys/proc_info.h>)
    using ProcessBsdInfo = ::proc_bsdinfo;
    using ProcessBsdShortInfo = ::proc_bsdshortinfo;
    using ProcessTaskInfo = ::proc_taskinfo;
    using ProcessTaskAllInfo = ::proc_taskallinfo;
    using ProcessThreadInfo = ::proc_threadinfo;
    using ProcessRegionInfo = ::proc_regioninfo;
    using ProcessRegionWithPathInfo = ::proc_regionwithpathinfo;
    using ProcessVnodePathInfo = ::proc_vnodepathinfo;
    using ProcessWorkQueueInfo = ::proc_workqueueinfo;
    using ProcessFileDescriptorInfo = ::proc_fdinfo;
    using ProcessFilePortInfo = ::proc_fileportinfo;
    using ProcessArchInfo = ::proc_archinfo;
    using VnodeInfoPath = ::vnode_info_path;
    using SocketFileDescriptorInfo = ::socket_fdinfo;
    #endif
}
