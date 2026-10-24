#include "v8datamodel/Stats.h"

#include "util/Log.h"
#include "util/Profiling.h"

#include <G3D/format.h>
#include <stdarg.h>
#include <stdio.h>

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

} // namespace Stats
} // namespace RBX

// SIZE 0x128
class ProfilingItem : public RBX::Stats::Item
{
	const RBX::Profiling::Profiler& p; // 0x120

public:
	// FUNCTION: WEBSERVICE 0x100b25e0
	ProfilingItem(const RBX::Profiling::Profiler* p) : p(*p) {}

	// FUNCTION: WEBSERVICE 0x100b2650
	virtual void update()
	{
		RBX::Profiling::Bucket b = p.getData(5.0);
		if (b.sampleTimeSpan > 0) {
			val = b.getTotalTime() / b.sampleTimeSpan;
			char buffer[256];
			if (b.frames > 0) {
				std::string t = RBX::Log::formatTime(b.getFrameTime());
				sprintf(buffer, "%s, %.3gfps (%.3g%%)", t.c_str(), b.getNominalFPS(), val * 100);
			}
			else {
				sprintf(buffer, "%.3g%%", val * 100);
			}
			sValue = buffer;
		}
		else {
			val = 0;
			sValue = "?";
		}
	}
};

DECOMP_SIZE_ASSERT(ProfilingItem, 0x128)

namespace RBX {
namespace Stats {

// FUNCTION: WEBSERVICE 0x100b29a0
Item* Item::createChildItem(const char* name)
{
	boost::shared_ptr<Item> item = Creatable<Instance>::create<Item>();
	item->setName(name);
	item->setParent(this);
	return item.get();
}

// FUNCTION: WEBSERVICE 0x100b2a70
Item* Item::createBoundChildItem(const Profiling::Profiler& p)
{
	boost::shared_ptr<Item> item = Creatable<Instance>::create<ProfilingItem>(&p);
	item->setName(p.name.c_str());
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
