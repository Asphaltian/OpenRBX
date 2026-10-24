#ifndef V8DATAMODEL_STATS_H
#define V8DATAMODEL_STATS_H

#include "decomp.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <string>

namespace RBX {

namespace Profiling {
class Profiler;
}

namespace Stats {

extern char sStats[];

extern char sStatsItem[];

// SIZE 0x120
class Item : public DescribedNonCreatable<Item, Instance, sStatsItem>
{
protected:
	double val;         // 0x0f8
	std::string sValue; // 0x100

	virtual void update() {} // vtable+0x40

public:
	// FUNCTION: WEBSERVICE 0x10065a50
	Item() : val(0) {}
	Item(const char* name) : DescribedNonCreatable<Item, Instance, sStatsItem>(name), val(0) {}

	const std::string& getStringValue()
	{
		update();
		return sValue;
	}

	// FUNCTION: WEBSERVICE 0x100b1f10
	double getValue()
	{
		update();
		return val;
	}

	// FUNCTION: WEBSERVICE 0x100b1f30
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
	Item* createBoundChildItem(const Profiling::Profiler& p);
	Item* createBoundMemChildItem(const char* name, const unsigned int& v);

protected:
	// FUNCTION: WEBSERVICE 0x100655c0 FOLDED
	virtual bool askAddChild(const Instance* instance) const { return dynamic_cast<const Item*>(instance) != NULL; }

	// SYNTHETIC: WEBSERVICE 0x100610e0
	// RBX::Stats::Item::~Item
};

DECOMP_SIZE_ASSERT(Item, 0x120)

template <>
void Item::formatValue<double>(const double& value);
template <>
void Item::formatValue<int>(const int& value);
template <>
void Item::formatValue<unsigned int>(const unsigned int& value);
template <>
void Item::formatValue<bool>(const bool& value);

// SIZE 0x140
template <class T>
class TypedStatsItem : public Item
{
	static const T& deref(const T* value) { return *value; }

protected:
	const boost::function0<T> func; // 0x120

public:
	TypedStatsItem(const T* value) : func(boost::bind(&deref, value)) {}

	virtual void update() { formatValue(func()); }

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10065ef0
	// RBX::Stats::TypedStatsItem<unsigned int>::~TypedStatsItem<unsigned int>
	// TEMPLATE: WEBSERVICE 0x10082f50
	// RBX::Stats::TypedStatsItem<unsigned int>::TypedStatsItem<unsigned int>
	// clang-format on
};

// SIZE 0x140
class TypedMemItem : public TypedStatsItem<unsigned int>
{
public:
	// FUNCTION: WEBSERVICE 0x100b2910
	TypedMemItem(const unsigned int* value) : TypedStatsItem<unsigned int>(value) {}

	// FUNCTION: WEBSERVICE 0x100b2980
	virtual void update() { formatMem(func()); }

	// SYNTHETIC: WEBSERVICE 0x10065f60
	// RBX::Stats::TypedMemItem::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(TypedMemItem, 0x140)

// SIZE 0xf8
class StatsService : public DescribedNonCreatable<StatsService, Instance, sStats>, public Service
{
public:
	// FUNCTION: WEBSERVICE 0x10065600
	StatsService() : DescribedNonCreatable<StatsService, Instance, sStats>("Stats") {}

protected:
	// FUNCTION: WEBSERVICE 0x100655c0 FOLDED
	virtual bool askAddChild(const Instance* instance) const { return dynamic_cast<const Item*>(instance) != NULL; }

	// FUNCTION: WEBSERVICE 0x10059d20 FOLDED
	virtual XmlElement* write() { return NULL; }

public:
	virtual ~StatsService() {}
};

DECOMP_SIZE_ASSERT(StatsService, 0xf8)

// SYNTHETIC: WEBSERVICE 0x1021b7f0
// RBX::Stats::`dynamic initializer for 'func_values''
// SYNTHETIC: WEBSERVICE 0x1021b820
// RBX::Stats::`dynamic initializer for 'func_value''

} // namespace Stats
} // namespace RBX

#endif // V8DATAMODEL_STATS_H
