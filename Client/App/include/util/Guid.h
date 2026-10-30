#ifndef UTIL_GUID_H
#define UTIL_GUID_H

#include "decomp.h"
#include "util/Name.h"

#include <boost/noncopyable.hpp>
#include <boost/thread/once.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <map>
#include <string>

namespace RBX {

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
	void extract(Data& result) const { result = data; }
	void copyDataFrom(const Guid& other) { assign(other.data); }

	// FUNCTION: WEBSERVICE 0x1019bbd0
	// FUNCTION: ROBLOX 0x004c24e0
	std::string readableString(int scopeLength) const { return data.readableString(4); }

	static int compare(const Guid* a, const Guid* b);
	static int compare(const Guid* a0, const Guid* a1, const Guid* b0, const Guid* b1);

	static const Name& getLocalScope();
	static void generateGUID(std::string& result);

private:
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
	// STUB: ROBLOX 0x004b3010
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
	void assignGuid(const Guid::Data& guidData)
	{
		unregisterGuid();
		guid.assign(guidData);
		registerGuid();
		onGuidChanged();
	}
	const Guid& getGuid() const
	{
		registerGuid();
		return guid;
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10194230
	// TEMPLATE: ROBLOX 0x004b5130
	// RBX::GuidItem<RBX::Instance>::lookupByGuid
	// clang-format on
	static bool lookupByGuid(const Guid::Data& data, T*& result)
	{
		if (Name::getNullName() == *data.scope) {
			result = NULL;
			return true;
		}

		StaticData& s = getStaticData();
		boost::recursive_mutex::scoped_lock lock(s.mutex);
		typename StaticData::Map::const_iterator iter = s.map.find(data);

		if (iter != s.map.end()) {
			result = iter->second;
			return true;
		}

		result = NULL;
		return false;
	}

	static T* getByGuid(const Guid::Data& data);

protected:
	GuidItem() : registeredGuid(false) {}

	virtual void onGuidChanged() {} // vtable+0x00

private:
	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1004cbc0
	// STUB: ROBLOX 0x004c99c0
	// RBX::GuidItem<RBX::Instance>::unregisterGuid
	// clang-format on
	void unregisterGuid() const
	{
		if (registeredGuid) {
			Guid::Data data;
			guid.extract(data);
			StaticData& s = getStaticData();
			boost::recursive_mutex::scoped_lock lock(s.mutex);
			s.map.erase(data);
		}
		registeredGuid = false;
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1009de40
	// STUB: ROBLOX 0x004b5260
	// RBX::GuidItem<RBX::Instance>::registerGuid
	// clang-format on
	void registerGuid() const
	{
		if (!registeredGuid) {
			Guid::Data data;
			guid.extract(data);
			StaticData& s = getStaticData();
			boost::recursive_mutex::scoped_lock lock(s.mutex);
			s.map[data] = const_cast<T*>(static_cast<const T*>(this));
		}
		registeredGuid = true;
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x101a3820
	// TEMPLATE: ROBLOX 0x004cbc50
	// RBX::GuidItem<RBX::Instance>::assignGuid
	// clang-format on
};

template <class T>
typename GuidItem<T>::StaticData* GuidItem<T>::staticData;

} // namespace RBX

#endif // UTIL_GUID_H
