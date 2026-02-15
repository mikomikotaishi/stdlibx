/**
 * @file ArgumentParser.Details.cppm
 * @internal
 * @module stdx:util.ArgumentParser.Details
 * @brief Internals used by ArgumentParser
 *
 * This file contains detail/implementation internals used by the ArgumentParser class.
 * Adapted from p-ranav/argparse (https://github.com/p-ranav/argparse)
 */

module;

#include "Macros.hpp"

export module stdx:util.ArgumentParser.Details;

import :core;
import :collections;
import :io;
import :iter;
import :meta;
import :ranges;
import :text;

using stdx::collections::TreeMap;
using stdx::collections::Vector;
using stdx::io::StringStream;
using stdx::iter::InputIterator;
using stdx::meta::FalseType;
using stdx::meta::IsConvertibleValue;
using stdx::meta::IsSameValue;
using stdx::meta::RemoveReferenceType;
using stdx::meta::TrueType;
using stdx::ranges::IotaView;
using stdx::ranges::views::Take;
using stdx::text::CharsFormat;

using namespace stdx::core;
using namespace stdx::literals;

#if 0

/**
 * @internal
 * @namespace _detail
 * @brief Wrapper namespace for standard library extension utility operations.
 */
export namespace _detail {



#endif
