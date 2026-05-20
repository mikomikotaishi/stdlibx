#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Desktop Window Manager (dwmapi.h) wrappers.
 */
export namespace stdx::os::win32 {
    #ifdef _WIN32
    // Handles / scalars
    using ThumbnailHandle = ::HTHUMBNAIL;
    using ThumbnailHandlePointer = ::PHTHUMBNAIL;
    using DwmFrameCount = ::DWM_FRAME_COUNT;
    using QpcTime = ::QPC_TIME;

    // Blur / glass / thumbnail / timing
    using DwmBlurBehind = ::DWM_BLURBEHIND;
    using DwmThumbnailProperties = ::DWM_THUMBNAIL_PROPERTIES;
    using DwmTimingInfo = ::DWM_TIMING_INFO;
    using DwmPresentParameters = ::DWM_PRESENT_PARAMETERS;
    using DwmUnsignedRatio = ::UNSIGNED_RATIO;
    using DwmMilMatrix3x2D = ::MilMatrix3x2D;

    // Composition / glass
    using ::DwmEnableComposition;
    using ::DwmIsCompositionEnabled;
    using ::DwmEnableBlurBehindWindow;
    using ::DwmExtendFrameIntoClientArea;
    using ::DwmDefWindowProc;

    // Color / attributes
    using ::DwmGetColorizationColor;
    using ::DwmGetWindowAttribute;
    using ::DwmSetWindowAttribute;

    // Thumbnail
    using ::DwmRegisterThumbnail;
    using ::DwmUnregisterThumbnail;
    using ::DwmUpdateThumbnailProperties;
    using ::DwmQueryThumbnailSourceSize;

    // Timing
    using ::DwmGetCompositionTimingInfo;
    using ::DwmFlush;
    using ::DwmModifyPreviousDxFrameDuration;
    using ::DwmSetDxFrameDuration;
    using ::DwmGetTransportAttributes;

    // Icons / textures
    using ::DwmSetIconicThumbnail;
    using ::DwmSetIconicLivePreviewBitmap;
    using ::DwmInvalidateIconicBitmaps;
    #endif
}
