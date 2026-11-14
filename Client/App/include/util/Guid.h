#ifndef UTIL_GUID_H
#define UTIL_GUID_H

#include "placeholder.h"
#include "util/Name.h"

#include <boost/noncopyable.hpp>
#include <boost/thread/once.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <map>
#include <string>

namespace RBX {

class Guid : private boost::noncopyable
{
public:
	struct Data
	{
		bool operator<(const Data& other) const;

		std::string readableString(int scopeLength) const;

		const Name* scope;
		int index;
	};

	Guid();

	void assign(Data data);
	void extract(Data& result) const { result = data; }
	void copyDataFrom(const Guid& other) { assign(other.data); }

	std::string readableString(int scopeLength) const { return data.readableString(4); }

	static int compare(const Guid* a, const Guid* b);
	static int compare(const Guid* a0, const Guid* a1, const Guid* b0, const Guid* b1);

	static const Name& getLocalScope();
	static void generateGUID(std::string& result);

private:
	Data data;
};

template <class T>
class __declspec(novtable) GuidItem
{
	struct StaticData
	{

		typedef std::map<Guid::Data, T*> Map;

		Map map;
		boost::recursive_mutex mutex;
	};

	static StaticData* staticData;

	static void initStaticData() { staticData = new StaticData(); }

	static StaticData& getStaticData()
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(&initStaticData, flag);
		return *staticData;
	}

	mutable bool registeredGuid;
	Guid guid;

public:
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

	virtual void onGuidChanged() {}

private:
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
};

template <class T>
typename GuidItem<T>::StaticData* GuidItem<T>::staticData;

} // namespace RBX

#endif // UTIL_GUID_H
