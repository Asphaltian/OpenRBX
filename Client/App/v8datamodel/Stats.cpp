#include "v8datamodel/Stats.h"

#include "util/Log.h"

#include <G3D/format.h>
#include <stdarg.h>

namespace RBX {
namespace Stats {

char sStats[] = "Stats";

char sStatsItem[] = "Item";

static Reflection::BoundFuncDesc<Item, std::string(), 0> func_values(
	&RBX::Stats::Item::getStringValue2,
	"GetValueString",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);
static Reflection::BoundFuncDesc<Item, double(), 0> func_value(
	&RBX::Stats::Item::getValue,
	"GetValue",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);

// FUNCTION: WEBSERVICE 0x100b1f60
void Item::formatMem(unsigned int bytes)
{
	setValue(bytes, Log::formatMem(bytes));
}

// FUNCTION: WEBSERVICE 0x100b1fe0
void Item::formatValue(double val, const char* fmt, ...)
{
	va_list argList;
	va_start(argList, fmt);
	setValue(val, G3D::vformat(fmt, argList));
	va_end(argList);
}

// FUNCTION: WEBSERVICE 0x100b2060
template <>
void Item::formatValue<double>(const double& value)
{
	formatValue(value, "%.3g", value);
}

// FUNCTION: WEBSERVICE 0x100b2090
template <>
void Item::formatValue<int>(const int& value)
{
	formatValue(value, "%d", value);
}

// FUNCTION: WEBSERVICE 0x100b20c0
template <>
void Item::formatValue<unsigned int>(const unsigned int& value)
{
	formatValue(value, "%d", value);
}

// FUNCTION: WEBSERVICE 0x100b20f0
template <>
void Item::formatValue<bool>(const bool& value)
{
	sValue = value ? "true" : "false";
	val = value ? 1 : 0;
}

// FUNCTION: WEBSERVICE 0x100b29a0
Item* Item::createChildItem(const char* name)
{
	boost::shared_ptr<Item> item = Creatable<Instance>::create<Item>();
	item->setName(name);
	item->setParent(this);
	return item.get();
}

// FUNCTION: WEBSERVICE 0x100b2c20
Item* Item::createBoundMemChildItem(const char* name, const unsigned int& v)
{
	boost::shared_ptr<Item> item = Creatable<Instance>::create<TypedMemItem>(&v);
	item->setName(name);
	item->setParent(this);
	return item.get();
}

} // namespace Stats
} // namespace RBX
