/**
 * @file ParsedArguments.cppm
 * @module stdx:util.ParsedArguments
 * @brief Implementation of the ParsedArguments class.
 *
 * This file contains the implementation of the ParsedArguments class, which is used to represent
 * the container of parsed arguments.
 * Adapted from p-ranav/argparse (https://github.com/p-ranav/argparse)
 */

module;

export module stdx:util.ParsedArguments;

import :core;
import :collections;

using stdx::collections::HashMap;
using stdx::collections::Vector;

using namespace stdx::core;

#if 0

/**
 * @namespace stdx::util
 * @brief Wrapper namespace for standard library extension utility operations.
 */
export namespace stdx::util {

/**
 * @class ParsedArguments
 * @brief Container for parsed argument values with type-safe access.
 */
class ParsedArguments {
private:
    HashMap<String, Any> values; ///< Values
    HashMap<String, Vector<String>> multiValues; ///< Multiple values
public:
    /**
     * @brief Get a single value with type checking.
     *
     * @tparam T The expected type of the value.
     * @param name The argument name.
     * @return The value cast to type T.
     *
     * @throws ArgumentParserException if argument not found or type mismatch.
    */
    template <typename T>
    T get(const String& name) const;

    /**
     * @brief Get a value with a default fallback.
     *
     * @tparam T The expect ed type of the value.
     * @param name The argument name.
     * @param defaultValue The default value if argument is not found.
     * @return The value or default value.
     */
    template <typename T>
    T get_or(const String& name, const T& defaultValue) const;

    /**
     * @brief Get multiple values for multi-value arguments.
     *
     * @param name The argument name.
     * @return Vector of string values.
     */
    Vector<String> get_multiple(const String& name) const;

    /**
     * @brief Check if an argument was provided.
     *
     * @param name The argument name.
     * @return true if argument exists, false otherwise.
     */
    bool has(const String& name) const;

    /**
     * @brief Set a single value.
     *
     * @param name The argument name.
     * @param value The value to set.
     */
    void set(const String& name, const Any& value);

    /**
     * @brief Set multiple values.
     *
     * @param name The argument name.
     * @param values The values to set.
     */
    void set_multiple(const String& name, const Vector<String>& values);

    /**
     * @brief Get all argument names that were parsed.
     *
     * @return Vector of argument names.
     */
    Vector<String> get_argument_names() const;
};

}

#endif
