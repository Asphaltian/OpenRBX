#include "v8datamodel/Stats.h"

#include "util/Log.h"
#include "util/Profiling.h"

#include <G3D/Stopwatch.h>
#include <G3D/format.h>
#include <stdarg.h>
#include <stdio.h>

namespace RBX {
namespace Stats {

char sStats[] = "Stats";

char sStatsItem[] = "Item";

static Reflection::BoundFuncDesc<Item, std::string(), 0> func_values(
	&Item::getStringValue2, "GetValueString", Reflection::FunctionDescriptor::NeedTrustedCaller
);

static Reflection::BoundFuncDesc<Item, double(), 0> func_value(
	&Item::getValue, "GetValue", Reflection::FunctionDescriptor::NeedTrustedCaller
);

void Item::formatMem(unsigned int bytes)
{
	setValue(bytes, Log::formatMem(bytes));
}

void Item::formatValue(double val, const char* fmt, ...)
{
	va_list argList;
	va_start(argList, fmt);
	setValue(val, G3D::vformat(fmt, argList));
	va_end(argList);
}

template <>
void Item::formatValue<G3D::Stopwatch>(const G3D::Stopwatch& value)
{
	val = value.smoothElapsedTime();
	if (value.smoothElapsedTime() > 0) {
		std::string time = Log::formatTime(value.smoothElapsedTime());
		sValue = G3D::format("%s (%.3gfps)", time.c_str(), 1.0 / value.smoothElapsedTime());
	}
	else {
		sValue = Log::formatTime(value.smoothElapsedTime());
	}
}

template <>
void Item::formatValue<double>(const double& value)
{
	formatValue(value, "%.3g", value);
}

template <>
void Item::formatValue<int>(const int& value)
{
	formatValue(value, "%d", value);
}

template <>
void Item::formatValue<unsigned int>(const unsigned int& value)
{
	formatValue(value, "%d", value);
}

template <>
void Item::formatValue<bool>(const bool& value)
{
	sValue = value ? "true" : "false";
	val = value ? 1 : 0;
}

} // namespace Stats
} // namespace RBX

class ProfilingItem : public RBX::Stats::Item
{
	const RBX::Profiling::Profiler& p;

public:
	ProfilingItem(const RBX::Profiling::Profiler* p) : p(*p) {}

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

namespace RBX {
namespace Stats {

Item* Item::createChildItem(const char* name)
{
	boost::shared_ptr<Item> item = Creatable<Instance>::create<Item>();
	item->setName(name);
	item->setParent(this);
	return item.get();
}

Item* Item::createBoundChildItem(const Profiling::Profiler& p)
{
	boost::shared_ptr<Item> item = Creatable<Instance>::create<ProfilingItem>(&p);
	item->setName(p.name.c_str());
	item->setParent(this);
	return item.get();
}

Item* Item::createBoundMemChildItem(const char* name, const unsigned int& v)
{
	boost::shared_ptr<Item> item = Creatable<Instance>::create<TypedMemItem>(&v);
	item->setName(name);
	item->setParent(this);
	return item.get();
}

} // namespace Stats
} // namespace RBX
