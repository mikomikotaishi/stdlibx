/**
 * @file windows.constants.cppm
 * @module stdx:os.win32.windows.constants
 * @brief Module file for constants of the Win32 operations.
 *
 * This file contains the implementation of the constants of the C-style signal operations in the standard library.
 */

module;

#include <cstddef>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.win32.windows.constants;
#else
export module stdlibx:os.win32.windows.constants;
#endif

/**
 * @namespace stdx::os::win32
 * @brief Wrapper namespace for Windows API operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::win32 {
#else 
export namespace stdlibx::os::win32 {
#endif
    constexpr size_t PATH_MAX = 260;
    constexpr size_t MAX_PATH = 260;

    constexpr char MINCHAR = 0x80;
    constexpr char MAXCHAR = 0x7f;
    constexpr short MINSHORT = 0x8000;
    constexpr short MAXSHORT = 0x7fff;
    constexpr short MINLONG = 0x80000000;
    constexpr short MAXLONG = 0x7fffffff;
    constexpr short MAXBYTE = 0xff;
    constexpr short MAXWORD = 0xffff;
    constexpr short MAXDWORD = 0xffffffff;

    constexpr long ERROR_SUCCESS = 0L;
    constexpr long ERROR_FILE_NOT_FOUND = 2L;
    constexpr long ERROR_PATH_NOT_FOUND = 3L;
    constexpr long ERROR_TOO_MANY_OPEN_FILES = 4L;
    constexpr long ERROR_ACCESS_DENIED = 5L;
    constexpr long ERROR_NO_MORE_FILES = 18L;
    constexpr long ERROR_SHARING_VIOLATION = 32L;
    constexpr long ERROR_FILE_EXISTS = 80L;
    constexpr long ERROR_INSUFFICIENT_BUFFER = 122L;
    constexpr long ERROR_ALREADY_EXISTS = 183L;
    constexpr long ERROR_MORE_DATA = 234L;

    constexpr unsigned short DLL_PROCESS_ATTACH = 1;
    constexpr unsigned short DLL_PROCESS_DETACH = 0;
    constexpr unsigned short DLL_THREAD_ATTACH = 2;
    constexpr unsigned short DLL_THREAD_DETACH = 3;

    constexpr long EXCEPTION_MAXIMUM_PARAMETERS = 15;
    constexpr long EXCEPTION_EXECUTE_HANDLER = 0x1;
    constexpr long EXCEPTION_CONTINUE_EXECUTION = 0xFFFFFFFF;
    constexpr long EXCEPTION_CONTINUE_SEARCH = 0x0;

    constexpr unsigned long EXCEPTION_ACCESS_VIOLATION = 0xC0000005UL;
    constexpr unsigned long EXCEPTION_DATATYPE_MISALIGNMENT = 0x80000002UL;
    constexpr unsigned long EXCEPTION_BREAKPOINT = 0x80000003UL;
    constexpr unsigned long EXCEPTION_SINGLE_STEP = 0x80000004UL;
    constexpr unsigned long EXCEPTION_ARRAY_BOUNDS_EXCEEDED = 0xC000008CUL;
    constexpr unsigned long EXCEPTION_FLT_DENORMAL_OPERAND = 0xC000008DUL;
    constexpr unsigned long EXCEPTION_FLT_DIVIDE_BY_ZERO = 0xC000008EUL;
    constexpr unsigned long EXCEPTION_FLT_INEXACT_RESULT = 0xC000008FUL;
    constexpr unsigned long EXCEPTION_FLT_INVALID_OPERATION = 0xC0000090UL;
    constexpr unsigned long EXCEPTION_FLT_OVERFLOW = 0xC0000091UL;
    constexpr unsigned long EXCEPTION_FLT_STACK_CHECK = 0xC0000092UL;
    constexpr unsigned long EXCEPTION_FLT_UNDERFLOW = 0xC0000093UL;
    constexpr unsigned long EXCEPTION_INT_DIVIDE_BY_ZERO = 0xC0000094UL;
    constexpr unsigned long EXCEPTION_INT_OVERFLOW = 0xC0000095UL;
    constexpr unsigned long EXCEPTION_PRIV_INSTRUCTION = 0xC0000096UL;
    constexpr unsigned long EXCEPTION_IN_PAGE_ERROR = 0xC0000006UL;
    constexpr unsigned long EXCEPTION_STACK_OVERFLOW = 0xC00000FDUL;
    constexpr unsigned long EXCEPTION_ILLEGAL_INSTRUCTION = 0xC000001DUL;
    constexpr unsigned long EXCEPTION_NONCONTINUABLE_EXCEPTION = 0xC0000025UL;
    constexpr unsigned long EXCEPTION_INVALID_DISPOSITION = 0xC0000026UL;
    constexpr unsigned long EXCEPTION_GUARD_PAGE = 0x80000001UL;
    constexpr unsigned long EXCEPTION_INVALID_HANDLE = 0xC0000008UL;
    constexpr unsigned long EXCEPTION_POSSIBLE_DEADLOCK = 0xC0000194UL;
    constexpr unsigned long CONTROL_C_EXIT = 0xC000013AUL;
    
    constexpr unsigned long EXCEPTION_ASSERTION = 0xEF000001UL;

    constexpr long TH32CS_SNAPTHREAD = 0x00000004;
    constexpr long IMAGE_FILE_MACHINE_I386 = 0x014c;
    constexpr long IMAGE_FILE_MACHINE_AMD64 = 0x8664;
    constexpr long CONTEXT_AMD64 = 0x100000;
    constexpr long CONTEXT_CONTROL = (CONTEXT_AMD64 | 0x1L);
    constexpr long CONTEXT_INTEGER = (CONTEXT_AMD64 | 0x2L);
    constexpr long CONTEXT_SEGMENTS = (CONTEXT_AMD64 | 0x4L);
    constexpr long CONTEXT_FLOATING_POINT = (CONTEXT_AMD64 | 0x8L);
    constexpr long CONTEXT_DEBUG_REGISTERS = (CONTEXT_AMD64 | 0x10L);
    constexpr long CONTEXT_FULL = (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_FLOATING_POINT);
    constexpr long CONTEXT_ALL = (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_SEGMENTS | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS);

    constexpr unsigned long SYMOPT_CASE_INSENSITIVE = 0x00000001;
    constexpr unsigned long SYMOPT_UNDNAME = 0x00000002;
    constexpr unsigned long SYMOPT_DEFERRED_LOADS = 0x00000004;
    constexpr unsigned long SYMOPT_NO_CPP = 0x00000008;
    constexpr unsigned long SYMOPT_LOAD_LINES = 0x00000010;
    constexpr unsigned long SYMOPT_OMAP_FIND_NEAREST = 0x00000020;
    constexpr unsigned long SYMOPT_LOAD_ANYTHING = 0x00000040;
    constexpr unsigned long SYMOPT_IGNORE_CVREC = 0x00000080;
    constexpr unsigned long SYMOPT_NO_UNQUALIFIED_LOADS = 0x00000100;
    constexpr unsigned long SYMOPT_FAIL_CRITICAL_ERRORS = 0x00000200;
    constexpr unsigned long SYMOPT_EXACT_SYMBOLS = 0x00000400;
    constexpr unsigned long SYMOPT_ALLOW_ABSOLUTE_SYMBOLS = 0x00000800;
    constexpr unsigned long SYMOPT_IGNORE_NT_SYMPATH = 0x00001000;
    constexpr unsigned long SYMOPT_INCLUDE_32BIT_MODULES = 0x00002000;
    constexpr unsigned long SYMOPT_PUBLICS_ONLY = 0x00004000;
    constexpr unsigned long SYMOPT_NO_PUBLICS = 0x00008000;
    constexpr unsigned long SYMOPT_AUTO_PUBLICS = 0x00010000;
    constexpr unsigned long SYMOPT_NO_IMAGE_SEARCH = 0x00020000;
    constexpr unsigned long SYMOPT_SECURE = 0x00040000;
    constexpr unsigned long SYMOPT_NO_PROMPTS = 0x00080000;
    constexpr unsigned long SYMOPT_OVERWRITE = 0x00100000;
    constexpr unsigned long SYMOPT_IGNORE_IMAGEDIR = 0x00200000;
    constexpr unsigned long SYMOPT_FLAT_DIRECTORY = 0x00400000;
    constexpr unsigned long SYMOPT_FAVOR_COMPRESSED = 0x00800000;
    constexpr unsigned long SYMOPT_ALLOW_ZERO_ADDRESS = 0x01000000;
    constexpr unsigned long SYMOPT_DISABLE_SYMSRV_AUTODETECT = 0x02000000;
    constexpr unsigned long SYMOPT_DEBUG = 0x80000000;

    constexpr int MAXIMUM_SUPPORTED_EXTENSION = 512;
}
