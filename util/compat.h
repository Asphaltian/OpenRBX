#ifndef COMPAT_H
#define COMPAT_H

#define MSVC800_VERSION 1400

#if defined(__MINGW32__) || defined(__clang__) || defined(__GNUC__) || (defined(_MSC_VER) && _MSC_VER > MSVC800_VERSION)
#define COMPAT_MODE
#endif

#pragma warning(disable : 4786)

#ifdef COMPAT_MODE
template <class T>
T& temporaryRef(const T& value)
{
	return const_cast<T&>(value);
}
#define RBX_TEMP_REF(x) temporaryRef(x)
#define RBX_INLINE
#else
#define RBX_TEMP_REF(x) x
#define RBX_INLINE inline
#endif

#ifdef COMPAT_MODE
inline int normalizedOrder(int order)
{
	return order < 0 ? -1 : order > 0 ? 1 : 0;
}
#define RBX_ORDER(x) normalizedOrder(x)
#else
#define RBX_ORDER(x) (x)
#endif

#ifdef _MSC_VER
#define RBX_NONVOID_CALL(R) void
#define RBX_VOID_CALL(name)                                                                                            \
	template <>                                                                                                        \
	void name<void>
#else
#include <boost/type_traits/is_void.hpp>
#include <boost/utility/enable_if.hpp>
#define RBX_NONVOID_CALL(R) typename boost::disable_if<boost::is_void<R> >::type
#define RBX_VOID_CALL(name)                                                                                            \
	template <class R>                                                                                                 \
	typename boost::enable_if<boost::is_void<R> >::type name
#endif

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

#include <boost/cstdint.hpp>
#include <ios>

#define BOOST_IOSTREAMS_POSITIONING_HPP_INCLUDED

namespace boost {
namespace iostreams {
typedef boost::intmax_t stream_offset;

inline std::streamoff stream_offset_to_streamoff(stream_offset off)
{
	return static_cast<std::streamoff>(off);
}

inline std::streampos offset_to_position(stream_offset off)
{
	return std::streampos(static_cast<std::streamoff>(off));
}

template <typename PosType>
inline stream_offset position_to_offset(PosType pos)
{
	return std::streamoff(pos);
}
} // namespace iostreams
} // namespace boost
#endif

#endif // COMPAT_H
