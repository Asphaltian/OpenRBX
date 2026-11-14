#ifndef UTIL_ASSOCIATION_H
#define UTIL_ASSOCIATION_H

#include "placeholder.h"

#include <vector>

namespace RBX {

template <class T>
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
	std::vector<Item*> items;

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
