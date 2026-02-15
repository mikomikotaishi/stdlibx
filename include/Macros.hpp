/**
 * @file Macros.hpp
 * @brief Macros used in the library
 *
 * This file contains all macros that are used in this library.
 */

#pragma once

#define THROWS_FALSE noexcept
#define THROWS_TRUE noexcept(false)

#define THROWS_SELECT(_1, NAME, ...) NAME
#define THROWS_CHOOSE(...) THROWS_SELECT(__VA_ARGS__, THROWS_TRUE, THROWS_FALSE)

#define throws(...) THROWS_CHOOSE(__VA_ARGS__)

/**
 * @brief Automatically exports the symbols in the stdx::core namespace.
 */
#define STDLIBX_STDX_MODULE_EXPORT_CORE() export using namespace stdx::core;

/**
 * @brief Automatically exports the symbols in the alloc namespace.
 */
#define STDLIBX_ALLOC_MODULE_EXPORT_CORE() export using namespace alloc;

/**
 * @brief Automatically exports the symbols in the core namespace.
 */
#define STDLIBX_CORE_MODULE_EXPORT_CORE() export using namespace core;

/**
 * @brief Automatically generate an implicit constructor for a class Derived
 * which extends Base.
 *
 * @param Derived The derived class
 * @param Base The base class 
 */
#define STDLIBX_GENERATE_DERIVED_CONSTRUCTOR(Derived, Base) \
    Derived(const Base& x): \
        Base(x) {}

/**
 * @brief Utility macro to set a class that cannot be instantiated, copied, or moved.
 * Use this for static utility classes that should only contain static methods.
 *
 * @param ClassName The name of the class to apply the macro to.
 */
#define UTILITY_CLASS(ClassName) \
public: \
    /** @brief Deleted constructor to prevent instantiation. */ \
    ClassName() = delete; \
    /** @brief Deleted destructor to prevent destruction. */ \
    ~ClassName() = delete; \
    /** @brief Deleted copy constructor to prevent copying. */ \
    ClassName(const ClassName&) = delete; \
    /** @brief Deleted copy assignment operator to prevent copying. */ \
    ClassName& operator=(const ClassName&) = delete; \
    /** @brief Deleted move constructor to prevent copying. */ \
    ClassName(ClassName&&) = delete; \
    /** @brief Deleted move assignment operator to prevent copying. */ \
    ClassName& operator=(ClassName&&) = delete; \
private:

/**
 * @brief Utility macro to set a class with defaulted Big Five operations.
 * This automatically generates default constructor, destructor, copy constructor,
 * copy assignment operator, move constructor, and move assignment operator.
 *
 * @param ClassName The name of the class to apply the macro to.
 */
#define DEFAULT_BIG_FIVE(ClassName) \
public: \
    /** @brief Default constructor. */ \
    ClassName() = default; \
    /** @brief Default destructor. */ \
    ~ClassName() = default; \
    /** @brief Default copy constructor. */ \
    ClassName(const ClassName&) = default; \
    /** @brief Default copy assignment operator. */ \
    ClassName& operator=(const ClassName&) = default; \
    /** @brief Default move constructor. */ \
    ClassName(ClassName&&) = default; \
    /** @brief Default move assignment operator. */ \
    ClassName& operator=(ClassName&&) = default;

/**
 * @brief Utility macro to set a class that is copyable but not movable.
 * This allows copying operations but prevents move operations.
 *
 * @param ClassName The name of the class to apply the macro to.
 */
#define COPYABLE_ONLY(ClassName) \
    /** @brief Default constructor. */ \
    ClassName() = default; \
    /** @brief Default destructor. */ \
    ~ClassName() = default; \
    /** @brief Default copy constructor. */ \
    ClassName(const ClassName&) = default; \
    /** @brief Default copy assignment operator. */ \
    ClassName& operator=(const ClassName&) = default; \
public: \
    /** @brief Deleted move constructor to prevent moving. */ \
    ClassName(ClassName&&) = delete; \
    /** @brief Deleted move assignment operator to prevent moving. */ \
    ClassName& operator=(ClassName&&) = delete; \
private:

/**
 * @brief Utility macro to set a class that is movable but not copyable.
 * This allows move operations but prevents copy operations. Useful for resource-managing classes.
 *
 * @param ClassName The name of the class to apply the macro to.
 */
#define MOVABLE_ONLY(ClassName) \
    /** @brief Default constructor. */ \
    ClassName() = default; \
    /** @brief Default destructor. */ \
    ~ClassName() = default; \
public: \
    /** @brief Deleted copy constructor to prevent copying. */ \
    ClassName(const ClassName&) = delete; \
    /** @brief Deleted copy assignment operator to prevent copying. */ \
    ClassName& operator=(const ClassName&) = delete; \
private: \
    /** @brief Default move constructor. */ \
    ClassName(ClassName&&) = default; \
    /** @brief Default move assignment operator. */ \
    ClassName& operator=(ClassName&&) = default;

/**
 * @brief Utility macro to set a class that cannot be copied or moved.
 * This prevents all copy and move operations while allowing default construction.
 * Useful for singleton base classes or unique resource holders.
 *
 * @param ClassName The name of the class to apply the macro to.
 */
#define NON_COPYABLE_NON_MOVABLE(ClassName) \
    /** @brief Default constructor. */ \
    ClassName() = default; \
    /** @brief Default destructor. */ \
    ~ClassName() = default; \
public: \
    /** @brief Deleted copy constructor to prevent copying. */ \
    ClassName(const ClassName&) = delete; \
    /** @brief Deleted copy assignment operator to prevent copying. */ \
    ClassName& operator=(const ClassName&) = delete; \
    /** @brief Deleted move constructor to prevent moving. */ \
    ClassName(ClassName&&) = delete; \
    /** @brief Deleted move assignment operator to prevent moving. */ \
    ClassName& operator=(ClassName&&) = delete; \
private:

/**
 * @brief Utility macro to set a singleton class pattern.
 * This provides the standard singleton implementation with thread-safe getInstance().
 * The class cannot be copied, moved, or instantiated directly.
 *
 * @param ClassName The name of the singleton class.
 */
#define SINGLETON_CLASS(ClassName) \
    /** @brief Private default constructor for singleton pattern. */ \
    ClassName() = default; \
    /** @brief Default destructor. */ \
    ~ClassName() = default; \
public: \
    /** @brief Deleted copy constructor to prevent copying. */ \
    ClassName(const ClassName&) = delete; \
    /** @brief Deleted copy assignment operator to prevent copying. */ \
    ClassName& operator=(const ClassName&) = delete; \
    /** @brief Deleted move constructor to prevent moving. */ \
    ClassName(ClassName&&) = delete; \
    /** @brief Deleted move assignment operator to prevent moving. */ \
    ClassName& operator=(ClassName&&) = delete; \
public: \
    /** @brief Get the singleton instance. @return Reference to the singleton instance. */ \
    [[nodiscard]] \
    static ClassName& getInstance() { \
        static ClassName instance; \
        return instance; \
    } \
private:

/**
 * @brief A utility to specialise a type in std::formatter. 
 * Requires that Formatter is imported into the current scope.
 * Used as a hack to allow stdx::fmt::Formatter to be used by stdx::fmt::format().
 * 
 * @param Typename The type to specialise into the formatter.
 */
#define SPECIALIZE_FORMATTER(Typename) \
    template <> \
    struct stdx::fmt::formatter<Typename> : public Formatter<Typename> {};

/**
 * @brief A utility to specialise a type in std::hash. 
 * Requires that Hash is imported into the current scope.
 * Used as a hack to allow stdx::core::Hash to be used by hash().
 * 
 * @param Typename The type to specialise into the hash.
 */
#define SPECIALIZE_HASH(Typename) \
    template <> \
    struct stdx::core::hash<Typename> : public Hash<Typename> {};
