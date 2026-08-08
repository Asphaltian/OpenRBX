#ifndef UTIL_GUID_H
#define UTIL_GUID_H

#include "decomp.h"

#include <boost/noncopyable.hpp>
#include <string>

namespace RBX {

class Name;

// SIZE 0x08
class Guid : private boost::noncopyable
{
public:
	struct Data
	{
		bool operator<(const Data& other) const;

		std::string readableString(int scopeLength) const;

		const Name* scope; // 0x00
		int index;         // 0x04
	};

	Guid();

	void assign(Data data);

	static int compare(const Guid* a, const Guid* b);
	static int compare(const Guid* a0, const Guid* a1, const Guid* b0, const Guid* b1);

	static const Name& getLocalScope();
	static void generateGUID(std::string& result);

	Data data; // 0x00
};

DECOMP_SIZE_ASSERT(Guid, 0x08)

template <class T>
class GuidItem
{
public:
	virtual ~GuidItem() {}

private:
	bool registeredGuid; // 0x04
	Guid guid;           // 0x08
};

} // namespace RBX

#endif // UTIL_GUID_H
