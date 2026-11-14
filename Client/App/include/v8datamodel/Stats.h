#ifndef V8DATAMODEL_STATS_H
#define V8DATAMODEL_STATS_H

#include "placeholder.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <string>

namespace G3D {
class Stopwatch;
}

namespace RBX {

namespace Profiling {
class Profiler;
}

namespace Stats {

extern char sStats[];

extern char sStatsItem[];

class Item : public DescribedNonCreatable<Item, Instance, sStatsItem>
{
protected:
	double val;
	std::string sValue;

	virtual void update() {}

public:
	Item() : val(0) {}
	Item(const char* name) : DescribedNonCreatable<Item, Instance, sStatsItem>(name), val(0) {}

	const std::string& getStringValue()
	{
		update();
		return sValue;
	}

	double getValue()
	{
		update();
		return val;
	}

	std::string getStringValue2()
	{
		update();
		return sValue;
	}

	void setValue(double val, const std::string& sValue)
	{
		this->val = val;
		this->sValue = sValue;
	}

	void formatMem(unsigned int bytes);

	void formatValue(double val, const char* fmt, ...);

	template <class T>
	void formatValue(const T& value);

	void formatPercent(double value);

	Item* createChildItem(const char* name);

	template <class T>
	Item* createChildItem(const char* name, boost::function0<T> func);

	Item* createBoundChildItem(const Profiling::Profiler& p);

	template <class T>
	Item* createBoundChildItem(const char* name, const T& v);

	Item* createBoundMemChildItem(const char* name, const unsigned int& v);

protected:
	virtual bool askAddChild(const Instance* instance) const { return dynamic_cast<const Item*>(instance) != NULL; }
};

template <>
void Item::formatValue<double>(const double& value);
template <>
void Item::formatValue<int>(const int& value);
template <>
void Item::formatValue<unsigned int>(const unsigned int& value);
template <>
void Item::formatValue<bool>(const bool& value);
template <>
void Item::formatValue<G3D::Stopwatch>(const G3D::Stopwatch& value);

template <class T>
class TypedStatsItem : public Item
{
	static const T& deref(const T* value) { return *value; }

protected:
	const boost::function0<T> func;

public:
	TypedStatsItem(boost::function0<T> func) : func(func) {}
	TypedStatsItem(const T* value) : func(boost::bind(&deref, value)) {}

	virtual void update() { formatValue(func()); }
};

class TypedMemItem : public TypedStatsItem<unsigned int>
{
public:
	TypedMemItem(const unsigned int* value) : TypedStatsItem<unsigned int>(value) {}

	virtual void update() { formatMem(func()); }
};

class StatsService : public DescribedNonCreatable<StatsService, Instance, sStats>, public Service
{
public:
	StatsService() : DescribedNonCreatable<StatsService, Instance, sStats>("Stats") {}

protected:
	virtual bool askAddChild(const Instance* instance) const { return dynamic_cast<const Item*>(instance) != NULL; }

	virtual XmlElement* write() { return NULL; }

public:
};

template <class T>
Item* Item::createChildItem(const char* name, boost::function0<T> func)
{
	boost::shared_ptr<Item> item = Creatable<Instance>::create<TypedStatsItem<T> >(func);
	item->setName(name);
	item->setParent(this);
	return item.get();
}

template <class T>
Item* Item::createBoundChildItem(const char* name, const T& v)
{
	boost::shared_ptr<Item> item = Creatable<Instance>::create<TypedStatsItem<T> >(&v);
	item->setName(name);
	item->setParent(this);
	return item.get();
}

} // namespace Stats
} // namespace RBX

#endif // V8DATAMODEL_STATS_H
