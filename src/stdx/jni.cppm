/**
 * @file jni.cppm
 * @module stdx:jni
 * @brief Implementation of the JNI extension library.
 *
 * This file contains the implementation of the JNI extension library, which
 * embeds a Java virtual machine and exposes the generated java::* bindings.
 *
 * jni.h is included in the global module fragment rather than the module
 * purview: it is a C header, so its declarations must stay attached to the
 * global module rather than to stdx:jni.
 */

module;

#include <version>

#ifdef STDLIBX_EXTENSIONS_COMPILE_JNI_LIBRARY
#include <jni.h>
#endif

#include "Macros.hpp"

export module stdx:jni;

import :main;

using stdx::collections::Vector;

#ifdef STDLIBX_EXTENSIONS_COMPILE_JNI_LIBRARY
#include "stdx/jni/runtime.inl"
#include "stdx/jni/text.inl"
#include "stdx/jni/errors.inl"
#include "stdx/jni/object.inl"
#include "stdx/jni/string.inl"
#include "stdx/jni/array.inl"

#include "stdx/jni/lang.generated.inl"
#endif
