#ifndef UTIL_UTILITIES_H
#define UTIL_UTILITIES_H

#include "decomp.h"

#include <boost/shared_ptr.hpp>
#include <iosfwd>
#include <string>

namespace RBX {

class Debugable
{
public:
	enum AssertAction
	{
		CrashOnAssert = 0,
		IgnoreAssert = 1
	};

	static AssertAction assertAction;
	static bool validatingDebug;

	virtual ~Debugable() {}
	virtual void dump(std::ostream& stream);
};

template <class T>
class CopyOnWrite
{
public:
	const T* read() const { return object.get(); }

private:
	boost::shared_ptr<T> object; // 0x00
};

template <class T>
class StringConverter
{
public:
	static std::string convertToString(const T& value);
	static bool convertToValue(const std::string& text, T& value);
};

} // namespace RBX

#endif // UTIL_UTILITIES_H
