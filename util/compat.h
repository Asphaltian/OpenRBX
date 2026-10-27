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

#ifdef COMPAT_MODE
#include <iterator>

namespace std {
struct _Outit
{
	typedef output_iterator_tag iterator_category;
	typedef void value_type;
	typedef void difference_type;
	typedef void pointer;
	typedef void reference;
};
} // namespace std
#endif

#endif // COMPAT_H
