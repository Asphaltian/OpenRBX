#ifndef UTIL_ASSOCIATION_H
#define UTIL_ASSOCIATION_H

#include "decomp.h"

#include <vector>

namespace RBX {

// clang-format off
// TEMPLATE: WEBSERVICE 0x1004a1e0
// TEMPLATE: ROBLOX 0x00577e10
// RBX::Association<RBX::Instance>::~Association<RBX::Instance>
// TEMPLATE: WEBSERVICE 0x1005f2f0
// RBX::Association<RBX::Instance>::Item::~Item
// TEMPLATE: WEBSERVICE 0x10061860
// TEMPLATE: ROBLOX 0x0056ea90
// RBX::Association<RBX::Instance>::contains<boost::shared_ptr<RBX::Lua::ThreadRef::Node> >
// TEMPLATE: WEBSERVICE 0x10061910
// TEMPLATE: ROBLOX 0x0056eb40
// RBX::Association<RBX::Instance>::remove<boost::shared_ptr<RBX::Lua::ThreadRef::Node> >
// SYNTHETIC: WEBSERVICE 0x10061a10
// SYNTHETIC: ROBLOX 0x0056ec40 FOLDED
// RBX::Association<RBX::Instance>::TItem<boost::shared_ptr<RBX::Lua::ThreadRef::Node> >::~TItem<boost::shared_ptr<RBX::Lua::ThreadRef::Node> >
// SYNTHETIC: WEBSERVICE 0x10061a80
// SYNTHETIC: ROBLOX 0x0056ecb0
// RBX::Association<RBX::Instance>::TItem<boost::shared_ptr<RBX::Lua::ThreadRef::Node> >::`scalar deleting destructor'
// TEMPLATE: WEBSERVICE 0x10064150
// STUB: ROBLOX 0x00571780
// RBX::Association<RBX::Instance>::get<boost::shared_ptr<RBX::Lua::ThreadRef::Node> >
// TEMPLATE: WEBSERVICE 0x1019c630
// TEMPLATE: ROBLOX 0x004c2a00
// RBX::Association<RBX::Instance>::TItem<RBX::Network::ClientPhysics>::TItem<RBX::Network::ClientPhysics><RBX::PartInstance *>
// SYNTHETIC: WEBSERVICE 0x1019c690
// SYNTHETIC: ROBLOX 0x004c2de0
// RBX::Association<RBX::Instance>::TItem<RBX::Network::ClientPhysics>::~TItem<RBX::Network::ClientPhysics>
// SYNTHETIC: WEBSERVICE 0x1019d160
// SYNTHETIC: ROBLOX 0x004c3c10
// RBX::Association<RBX::Instance>::Item::`scalar deleting destructor'
// SYNTHETIC: WEBSERVICE 0x1019d4b0
// SYNTHETIC: ROBLOX 0x004c4020
// RBX::Association<RBX::Instance>::TItem<RBX::Network::ClientPhysics>::`scalar deleting destructor'
// TEMPLATE: WEBSERVICE 0x101a2b80
// STUB: ROBLOX 0x004c9630
// RBX::Association<RBX::Instance>::get<TimeStamp>
// TEMPLATE: WEBSERVICE 0x101a2cb0
// STUB: ROBLOX 0x004c9760
// RBX::Association<RBX::Instance>::get<RBX::Network::ClientPhysics,RBX::PartInstance *>
// clang-format on
template <class T>
// SIZE 0x10
class Association
{
public:
	class Item
	{
	public:
		virtual ~Item() {}
	};

	template <class U>
	class TItem : public Item
	{
	public:
		U t;

		TItem() {}

		template <class Arg>
		TItem(Arg param0) : t(param0)
		{
		}
	};

private:
	std::vector<Item*> items; // 0x00

	static unsigned int& count()
	{
		static unsigned int c = 0;
		return c;
	}

	template <class U>
	unsigned int index() const
	{
		static const unsigned int i = count()++;
		return i;
	}

public:
	~Association()
	{
		typename std::vector<Item*>::iterator iter = items.begin();
		typename std::vector<Item*>::iterator end = items.end();
		while (iter != end) {
			delete *iter;
			++iter;
		}
	}

	template <class U>
	bool contains() const
	{
		return items.size() > index<U>() && items[index<U>()] != NULL;
	}

	template <class U>
	void remove()
	{
		if (items.size() > index<U>()) {
			delete items[index<U>()];
			items[index<U>()] = NULL;
		}
	}

	template <class U>
	U& get()
	{
		while (items.size() <= index<U>()) {
			items.push_back(NULL);
		}
		Item*& item = items[index<U>()];
		if (!item) {
			item = new TItem<U>();
		}
		return static_cast<TItem<U>*>(item)->t;
	}

	template <class U, class Arg>
	U& get(Arg param0)
	{
		while (items.size() <= index<U>()) {
			items.push_back(NULL);
		}
		Item*& item = items[index<U>()];
		if (!item) {
			item = new TItem<U>(param0);
		}
		return static_cast<TItem<U>*>(item)->t;
	}
};

} // namespace RBX

#endif // UTIL_ASSOCIATION_H
