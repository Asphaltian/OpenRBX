#ifndef COMPAT_H
#define COMPAT_H

// Various macros to enable compiling with other/newer compilers.

// Visual C++ 8.0, shipped with Visual Studio 2005. The original RBXGS binaries were built with
// the SP1 revision of it, cl.exe 14.00.50727.762.
#define MSVC800_VERSION 1400

#if defined(__MINGW32__) || defined(__clang__) || defined(__GNUC__) || (defined(_MSC_VER) && _MSC_VER > MSVC800_VERSION)
#define COMPAT_MODE
#endif

// Disable "identifier was truncated to '255' characters" warning.
// Impossible to avoid this if using STL map or set.
#pragma warning(disable : 4786)

// We use `override` so newer compilers can tell us our vtables are valid, however this keyword
// was added in C++11, so we define it as empty for compatibility with older compilers.
#if __cplusplus < 201103L
#define override
#define static_assert(expr, msg)
#else
#define override override
#endif

#endif // COMPAT_H
