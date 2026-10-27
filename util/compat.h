#ifndef COMPAT_H
#define COMPAT_H

#define MSVC800_VERSION 1400

#if defined(__MINGW32__) || defined(__clang__) || defined(__GNUC__) || (defined(_MSC_VER) && _MSC_VER > MSVC800_VERSION)
#define COMPAT_MODE
#endif

#pragma warning(disable : 4786)

#if __cplusplus < 201103L && !defined(COMPAT_MODE)
#define override
#define static_assert(expr, msg)
#endif

#endif // COMPAT_H
