#ifndef UTIL_UTILITIES_H
#define UTIL_UTILITIES_H

#include <boost/shared_ptr.hpp>
#include <iosfwd>

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
	const T* read() const { return value.get(); }

private:
	boost::shared_ptr<T> value; // 0x00
};

} // namespace RBX

#endif // UTIL_UTILITIES_H
