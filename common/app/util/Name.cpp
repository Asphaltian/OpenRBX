#include "util/Name.h"

#include "decomp.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/once.hpp>

namespace RBX {

DECOMP_SIZE_ASSERT(Name, 0x20)

class Name::NamMap : public std::map<std::string, Name*>
{
public:
	~NamMap() {}
};

static boost::once_flag flag = BOOST_ONCE_INIT;

// FUNCTION: WEBSERVICE 0x10057ed0
DECOMP_NOINLINE boost::mutex& moo2()
{
	static boost::mutex mutex2;

	return mutex2;
}

// FUNCTION: WEBSERVICE 0x10057f30
DECOMP_NOINLINE void initMoo()
{
	moo2();
}

// FUNCTION: WEBSERVICE 0x10058640
DECOMP_NOINLINE std::map<int, Name*>& Name::dictionary()
{
	static std::map<int, Name*> d;

	return d;
}

// FUNCTION: WEBSERVICE 0x100587b0
DECOMP_NOINLINE Name::NamMap& Name::namMap()
{
	static NamMap n;

	return n;
}

// STUB: WEBSERVICE 0x10058840
const Name& Name::declare(const char* name, int dictionaryIndex)
{
	if (name == NULL) {
		return getNullName();
	}

	boost::call_once(initMoo, flag);

	boost::mutex::scoped_lock lock(moo2());
	NamMap::iterator found = namMap().find(std::string(name));

	if (found != namMap().end()) {
		if (dictionaryIndex != -1) {
			found->second->dictionaryIndex = dictionaryIndex;
			dictionary()[dictionaryIndex] = found->second;
		}

		return *found->second;
	}

	Name* declared = new Name(name, dictionaryIndex);
	namMap()[std::string(name)] = declared;

	if (dictionaryIndex != -1) {
		dictionary()[dictionaryIndex] = declared;
	}

	return *declared;
}

// FUNCTION: WEBSERVICE 0x10058a30
const Name& Name::getNullName()
{
	boost::call_once(initMoo, flag);

	boost::mutex::scoped_lock lock(moo2());
	static const Name* nullName = &declare("", 0);

	return *nullName;
}

// FUNCTION: WEBSERVICE 0x10058ad0
const Name& Name::lookup(const std::string& name)
{
	boost::call_once(initMoo, flag);

	boost::mutex::scoped_lock lock(moo2());
	NamMap::iterator found = namMap().find(name);

	if (found != namMap().end()) {
		return *found->second;
	}

	return getNullName();
}

// FUNCTION: WEBSERVICE 0x10058ba0
const Name& Name::lookup(const char* name)
{
	if (name == NULL) {
		return getNullName();
	}

	return lookup(std::string(name));
}

// FUNCTION: WEBSERVICE 0x10058c70
bool Name::empty() const
{
	return this == &getNullName();
}

Name::Name(const char* name, int dictionaryIndex) : dictionaryIndex(dictionaryIndex), name(name)
{
}

} // namespace RBX
