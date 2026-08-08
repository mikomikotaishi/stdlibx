#pragma once

/**
 * @namespace stdx::os::darwin::sys
 * @brief Bulk attribute retrieval - <sys/attr.h>.
 */
export namespace stdx::os::darwin::sys {
    #if defined(__APPLE__) && __has_include(<sys/attr.h>)
    using AttributeList = ::attrlist;
    using AttributeSet = ::attribute_set_t;
    using AttributeReference = ::attrreference_t;
    using DiskInterval = ::diskextent;
    using VolumeCapabilitiesAttribute = ::vol_capabilities_attr_t;
    using VolumeAttributesAttribute = ::vol_attributes_attr_t;

    using ::getattrlist;
    using ::setattrlist;
    using ::fgetattrlist;
    using ::fsetattrlist;
    using ::getattrlistat;
    using ::setattrlistat;
    using ::getattrlistbulk;
    #endif
}
