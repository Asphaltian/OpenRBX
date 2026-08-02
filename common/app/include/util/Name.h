#ifndef UTIL_NAME_H
#define UTIL_NAME_H

#include <boost/noncopyable.hpp>
#include <string>

namespace RBX {

// SIZE 0x20
class Name : public boost::noncopyable
{
public:
	static const Name& declare(const char* name, int dictionaryIndex);
	static const Name& lookup(const char* name);
	static const Name& lookup(const std::string& name);
	static const Name& getNullName();

	bool empty() const;

private:
	// TEMPLATE: WEBSERVICE 0x1000b530
	// RBX::Name::doDeclare<&RBX::sWorkspace>
	template <char* name>
	static const Name& doDeclare()
	{
		static const Name& n = declare(name, -1);

		return n;
	}

	template <char* name>
	static void callDoDeclare()
	{
		doDeclare<name>();
	}

	int dictionaryIndex; // 0x00
	std::string name;    // 0x04
};

} // namespace RBX

#endif // UTIL_NAME_H
