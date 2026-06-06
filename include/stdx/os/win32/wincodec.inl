#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Windows Imaging Component (WIC, wincodec.h) wrappers.
 *
 * WIC is a COM-based API. Most of the public surface is COM interfaces
 * accessed via IWICImagingFactory. Only the most-used interfaces and
 * value types are aliased here; everything is reachable through
 * ::IWICXxx names too.
 */
export namespace stdx::os::win32 {
    #if defined(_WIN32) && __has_include(<wincodec.h>)
    // Factory / top-level
    using WicImagingFactory = ::IWICImagingFactory;
    using WicImagingFactory2 = ::IWICImagingFactory2;

    // Bitmap sources / operations
    using WicBitmapSource = ::IWICBitmapSource;
    using WicBitmap = ::IWICBitmap;
    using WicBitmapLock = ::IWICBitmapLock;
    using WicBitmapFlipRotator = ::IWICBitmapFlipRotator;
    using WicBitmapScaler = ::IWICBitmapScaler;
    using WicBitmapClipper = ::IWICBitmapClipper;
    using WicFormatConverter = ::IWICFormatConverter;
    using WicColorTransform = ::IWICColorTransform;
    using WicColorContext = ::IWICColorContext;

    // Palette
    using WicPalette = ::IWICPalette;

    // Decoder / encoder
    using WicBitmapDecoder = ::IWICBitmapDecoder;
    using WicBitmapEncoder = ::IWICBitmapEncoder;
    using WicBitmapFrameDecode = ::IWICBitmapFrameDecode;
    using WicBitmapFrameEncode = ::IWICBitmapFrameEncode;
    using WicBitmapDecoderInfo = ::IWICBitmapDecoderInfo;
    using WicBitmapEncoderInfo = ::IWICBitmapEncoderInfo;
    using WicBitmapCodecInfo = ::IWICBitmapCodecInfo;

    // Component info
    using WicComponentInfo = ::IWICComponentInfo;
    using WicPixelFormatInfo = ::IWICPixelFormatInfo;
    using WicPixelFormatInfo2 = ::IWICPixelFormatInfo2;
    using WicFormatConverterInfo = ::IWICFormatConverterInfo;

    // Stream / metadata
    using WicStream = ::IWICStream;
    using WicMetadataQueryReader = ::IWICMetadataQueryReader;
    using WicMetadataQueryWriter = ::IWICMetadataQueryWriter;

    // Value types
    using WicRect = ::WICRect;
    using WicPixelFormatGuid = ::WICPixelFormatGUID;
    using WicColor = ::WICColor;
    using WicBitmapPattern = ::WICBitmapPattern;

    // Helper
    using ::WICCreateImagingFactory_Proxy;
    #endif
}
