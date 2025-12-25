/**
 * @file asm-generic.cppm
 * @module stdx:os.linux.asmgeneric
 * @brief Module file for Unix asm-generic operations.
 *
 * This file contains the implementation of the Unix asm-generic operations from the POSIX libraries,
 * located in all <asm-generic/*.h> files.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.asmgeneric;
#else
export module stdlibx:os.linux.asmgeneric;
#endif

export import :os.linux.asmgeneric.errno;
