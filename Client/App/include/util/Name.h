#ifndef UTIL_NAME_H
#define UTIL_NAME_H

#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/once.hpp>
#include <map>
#include <string>

namespace RBX {

class Name : public boost::noncopyable
{
	static boost::mutex& mutex();

public:
	static const Name& declare(const char* name, int dictionaryIndex);
	static const Name& lookup(const std::string& name);
	static const Name& lookup(const char* name);
	static const Name& lookup(int);
	static const Name& getNullName();

private:
	class NamMap;

	static NamMap& namMap();
	static std::map<int, Name*>& dictionary();

	Name(const char* name, int dictionaryIndex);
	Name& operator=(const Name& other);

public:
	template <const char* name>
	static const Name& declare()
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;

		boost::call_once(&callDoDeclare<name>, flag);

		return doDeclare<name>();
	}

	bool empty() const;

	const std::string& toString() const { return name; }

	const char* c_str() const { return name.c_str(); }

	int compare(const Name& other) const
	{
		if (this == &other) {
			return 0;
		}
		return name.compare(other.name);
	}

	static int compare(const Name* a, const Name* b);

	bool operator<(const Name& other) const;
	bool operator>(const Name& other) const;

	bool operator==(const Name& other) const { return this == &other; }
	bool operator==(const std::string& other) const { return name.compare(other) == 0; }
	bool operator==(const char* other) const { return name.compare(other) == 0; }

	bool operator!=(const Name& other) const { return this != &other; }
	bool operator!=(const std::string& other) const;
	bool operator!=(const char* other) const { return name.compare(other) != 0; }
	int getDictionaryIndex() const;

private:
	template <const char* name>
	static const Name& doDeclare()
	{
		static const Name& n = declare(name, -1);

		return n;
	}

	template <const char* name>
	static void callDoDeclare()
	{
		doDeclare<name>();
	}

	int dictionaryIndex;

public:
	std::string name;
};

class INamed
{
public:
	virtual const Name& getName() const = 0;
};

template <class Base, const char* nameValue>
class __declspec(novtable) Named : public Base
{
public:
	template <class A>
	Named(A a) : Base(a)
	{
	}

	template <class A, class B, class C>
	Named(A a, B b, C c) : Base(a, b, c)
	{
	}

	static const Name& name() { return Name::declare<nameValue>(); }

	virtual const Name& getName() const { return name(); }
};

} // namespace RBX

#endif // UTIL_NAME_H
