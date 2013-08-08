#pragma once
// # MSVC (2010) specific code
#ifdef _MSC_VER

#include <stdarg.h>
#include <stdio.h>

/// Snprintf is not part of C89.
/// It's standard only in C99. Microsoft has no plan supporting C99.
#define snprintf c99_snprintf
#define vsnprintf c99_vsnprintf

/// MSVC implementation of the C99 vsnprintf function.
inline int c99_vsnprintf( char* str, size_t size, const char* format, va_list ap )
{
    int count = -1;
    if ( size != 0 )
    {
        count = _vsnprintf_s( str, size, _TRUNCATE, format, ap );
    }
    if ( count == -1 )
    {
        count = _vscprintf( format, ap );
    }
    return count;
}


/// MSVC implementation of the C99 snprintf function.
inline int c99_snprintf( char* str, size_t size, const char* format, ... )
{
    int count;
    va_list ap;
    va_start( ap, format );
    count = c99_vsnprintf( str, size, format, ap );
    va_end( ap );
    return count;
}

// Macro Overrides
// ---------------
// 
// Ugly hack to disable warnings for macro overrides.
__pragma(warning(push))
__pragma(warning(disable:4005))

/// Disable the compiler warning C4127 for the do {} while(0) loops.
#define MAKE_DELETE(a, T, p) \
    __pragma(warning(push)) \
    __pragma(warning(disable:4127)) \
    do {if (p) {(p)->~T(); a.deallocate(p);}} while (0) \
    __pragma(warning(pop))

// Restore the macro override warning.
__pragma(warning(pop))

#endif // _MSC_VER