#ifndef UTIL_UTILITIES_H
#define UTIL_UTILITIES_H

#include "decomp.h"

#include <boost/noncopyable.hpp>
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

	static void forceBadTypeId();
	static void doCrash();

	static void dump(void* object, std::ostream& stream);
	virtual void dump(std::ostream& stream); // vtable+0x00
};

template <class T>
class CopyOnWrite : public boost::noncopyable
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
