#pragma once

#include "core/log.hpp"

#include "debugbreak.hpp"

namespace acid
{

/// DO NOT wrap functions in the macro 
#ifdef ACID_ENABLE_ASSERTS
    #define AC_ASSERT(x) if (!(x)) \
        do { \
            AC_LOG_FATAL("\x1b[31;1mAssertion failed!\x1b[0m"); \
            debug_break(); \
        } while (0)

    #define AC_ASSERT_MSG(x, ...) \
        do { \
            if (!(x)) \
            { \
                AC_LOG_FATAL("\x1b[31;1mAssertion failed!\x1b[0m: ", __VA_ARGS__); \
                debug_break(); \
            } \
        } while (0)
#else
    #define AC_ASSERT(x)
    #define AC_ASSERT_MSG(x, ...)
#endif


} // namespace acid
