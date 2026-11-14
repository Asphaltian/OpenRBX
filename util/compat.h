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

#include <boost/cstdint.hpp>
#include <ios>
#include <string>

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

inline std::string::iterator begin(std::string& s)
{
	return s.begin();
}

inline std::string::iterator end(std::string& s)
{
	return s.end();
}

inline std::string::const_iterator begin(const std::string& s)
{
	return s.begin();
}

inline std::string::const_iterator end(const std::string& s)
{
	return s.end();
}
} // namespace boost
#endif

#endif // COMPAT_H
