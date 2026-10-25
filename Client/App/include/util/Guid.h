#ifndef UTIL_GUID_H
#define UTIL_GUID_H

#include "decomp.h"

#include <boost/noncopyable.hpp>
#include <boost/thread/once.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <map>
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

// SIZE 0x10
template <class T>
class __declspec(novtable) GuidItem
{
	// SIZE 0x18
	struct StaticData
	{
		// SYNTHETIC: WEBSERVICE 0x1004a660
		// RBX::GuidItem<RBX::Instance>::StaticData::StaticData

		typedef std::map<Guid::Data, T*> Map;

		Map map;                      // 0x00
		boost::recursive_mutex mutex; // 0x0c
	};

	static StaticData* staticData;

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1004b3f0
	// RBX::GuidItem<RBX::Instance>::initStaticData
	// clang-format on
	static void initStaticData() { staticData = new StaticData(); }

	static StaticData& getStaticData()
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(&initStaticData, flag);
		return *staticData;
	}

	mutable bool registeredGuid; // 0x04
	Guid guid;                   // 0x08

public:
	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1004ce20
	// RBX::GuidItem<RBX::Instance>::~GuidItem<RBX::Instance>
	// clang-format on
	~GuidItem() { unregisterGuid(); }
	void assignGuid(const Guid::Data& guidData);
	const Guid& getGuid() const { return guid; }

	static bool lookupByGuid(const Guid::Data& data, T*& result);
	static T* getByGuid(const Guid::Data& data);

protected:
	GuidItem() : registeredGuid(false) {}

	virtual void onGuidChanged() {} // vtable+0x00

private:
	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1004cbc0
	// RBX::GuidItem<RBX::Instance>::unregisterGuid
	// clang-format on
	void unregisterGuid() const
	{
		if (registeredGuid) {
			Guid::Data data = guid.data;
			StaticData& s = getStaticData();
			boost::recursive_mutex::scoped_lock lock(s.mutex);
			s.map.erase(data);
		}
		registeredGuid = false;
	}

	void registerGuid() const
	{
		if (!registeredGuid) {
			Guid::Data data = guid.data;
			StaticData& s = getStaticData();
			boost::recursive_mutex::scoped_lock lock(s.mutex);
			s.map[data] = const_cast<T*>(static_cast<const T*>(this));
		}
		registeredGuid = true;
	}
};

template <class T>
typename GuidItem<T>::StaticData* GuidItem<T>::staticData;

} // namespace RBX

#endif // UTIL_GUID_H
